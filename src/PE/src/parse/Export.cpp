#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


using namespace Fw;


bool areArraysExists(uint32_t namesPos, uint32_t ordinalsPos, uint32_t rvasPos)
{
    return (namesPos != utils::INVALID_RVA && namesPos > 0) && 
        (ordinalsPos != utils::INVALID_RVA && ordinalsPos > 0) && 
        (rvasPos != utils::INVALID_RVA && rvasPos > 0);
}

template<typename ItemType>
std::vector<ItemType> readData(std::istream& stream, std::streampos itemsPos, size_t count)
{
    std::vector<ItemType> result;
    if (count > 0) {
        result.resize(count);
        try {
            stream.seekg(itemsPos);
            stream.read(reinterpret_cast<char*>(&result[0]), count * sizeof(ItemType));
        }
        catch(const std::ios::failure&) {
            result.clear();
        }
    }
    return result;
}


template<typename Arch>
bool Export::readNamesOrdinalsRvas(const Image<Arch>& image)
{
    const auto& expDir = image.getExportDirHeader();
    uint32_t namesPos = utils::RvaToRaw( image, expDir.AddressOfNames );
    uint32_t ordinalsPos = utils::RvaToRaw( image, expDir.AddressOfNameOrdinals );
    uint32_t rvasPos = utils::RvaToRaw( image, expDir.AddressOfFunctions );

    if (!areArraysExists(namesPos, ordinalsPos, rvasPos))
        return false;

    std::istream& stream = image.getStream();

    names_ = readData<uint32_t>(stream, namesPos, expDir.NumberOfNames);
    if (names_.empty())
        goto NAME_IS_EMPTY;
    ordinals_ = readData<uint16_t>(stream, ordinalsPos, expDir.NumberOfNames);
    if (ordinals_.empty())
        goto ORDINALS_IS_EMPTY;
    rvas_ = readData<uint32_t>(stream, rvasPos, expDir.NumberOfFunctions);
    if (!rvas_.empty())
        return true;

    ordinals_.clear();
ORDINALS_IS_EMPTY:
    names_.clear();
NAME_IS_EMPTY:
    return false;
}


template<typename Arch>
Export::Export(const Image<Arch>& image)
{
    assert(image.getStream().good());

    const auto& expDir = image.getExportDirHeader();
    std::istream& stream = image.getStream();

    if (!readNamesOrdinalsRvas(image))
        return;

    const auto* expDataDir = &image.getNtHeaders().OptionalHeader.DataDirectory[0];

    uint32_t exportRva = expDataDir->VirtualAddress;
    uint32_t exportEnd = exportRva + expDataDir->Size;
    uint32_t elementsCount = expDir.NumberOfFunctions;

    Element element;

    for (uint16_t elementIndex = 0; elementIndex < elementsCount; ++elementIndex) {
        element.forwardName.clear();
        element.name.clear();

        bool byOrdinal = (elementIndex < expDir.NumberOfNames);
        uint16_t ordinal = byOrdinal ? ordinals_[elementIndex] : elementIndex;

        element.ordinal = ordinal + (uint16_t)expDir.Base;

        uint32_t elementRva = rvas_[ordinal];
        if (elementRva == 0)
            continue;

        if (byOrdinal) {
            uint32_t nameRva = names_[elementIndex];
            std::streampos namePos = utils::RvaToRaw(image,nameRva);
            if (utils::isPeOffsetCorrected(namePos))
                element.name = Io::StreamUtils::readStringAt(stream, namePos);
        }
        else
            element.name = "n/a";

        std::streampos elementPos = utils::RvaToRaw(image,elementRva);
        bool isForwardFound = (exportRva < elementRva) && (elementRva < exportEnd);
        if (isForwardFound) {
            element.rva = utils::INVALID_RVA;
            element.offset = Io::INVALID_OFFSET;
            if (utils::isPeOffsetCorrected(elementPos))
                element.forwardName = Io::StreamUtils::readStringAt(stream, elementPos);
        }
        else {
            element.rva = elementRva;
            element.offset = elementPos;
        }
        elements_.emplace_back(element);
    }
}

const std::string& Export::timeDateStamp() const
{
    return timeDateStamp_;
}

const std::list<Export::Element>& Export::elements() const
{
    return elements_;
}


template   Export::Export(const Image<arch32>& image);
template   Export::Export(const Image<arch64>& image);


}} // namespace pe::parse
