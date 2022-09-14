#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


using namespace Fw;


template<typename Arch>
void initializeIatTable(
    const Image<Arch> & image,
    uint32_t iatRva,
    uint32_t thinksCount,
    typename std::vector<typename headers_traits<Arch>::VirtualAddress_type> * iatTable)
{
    using VirtualAddress_t=headers_traits<Arch>::VirtualAddress_type;

    const auto& info = headers_utils::getImportedElementsInfo(image, iatRva);
    if (iatRva == 0 || info.pos == utils::INVALID_RAW)
        return;

    size_t iatCount = thinksCount;
    if (iatCount == 0)
        iatCount = info.size / sizeof(VirtualAddress_t);

    if (iatCount > 0) {
        iatTable->resize(iatCount);
        image.getStream().seekg(info.pos);
        image.getStream().read(reinterpret_cast<char*>(&(*iatTable)[0]), info.size);
    }
}


template<typename Arch>
void extractModuleElements( 
    const Image<Arch> & image,
    const ImgImportDescriptor & impDescr,
    uint32_t imageSize,
    std::list<import::Element> * elements)
{
    assert(elements > 0 && elements->empty());

    using VirtualAddress_t=headers_traits<Arch>::VirtualAddress_type;

    std::vector<VirtualAddress_t> bindingIat;
    initializeIatTable(image, impDescr.OriginalFirstThunk, 0, &bindingIat);

    std::vector<VirtualAddress_t> typicalIat;
    initializeIatTable(image, impDescr.FirstThunk, (uint32_t)bindingIat.size(), &typicalIat);

    bool doesBindingUsed = utils::detectBindingImportType(impDescr.TimeDateStamp) !=
        utils::BindingImportType::None;

    VirtualAddress_t * importLookupTable = 0;
    if (!bindingIat.empty())
        importLookupTable = (VirtualAddress_t *)(&bindingIat[0]);
    else if (!typicalIat.empty() && doesBindingUsed)
        importLookupTable = (VirtualAddress_t *)(&typicalIat[0]);
    else
        return;

    VirtualAddress_t imageBase = image.getNtHeaders().OptionalHeader.ImageBase;
    uint32_t elementRva = impDescr.FirstThunk;

    enum { numForwarderEnd = 0xFFFFFFFF };
    uint32_t forwarderIndex = (impDescr.ForwarderChain == 0) ? numForwarderEnd : impDescr.ForwarderChain;

    std::istream& stream = image.getStream();

    VirtualAddress_t ordinalFlag = ((VirtualAddress_t)1 << ( (8*sizeof(VirtualAddress_t) ) - 1));

    size_t elementsCount = typicalIat.size();
    for (size_t elementIndex = 0; elementIndex < elementsCount; ++elementIndex) {
        import::Element newElement;

        if (forwarderIndex != numForwarderEnd) {
            newElement.isForwarded = true;
            forwarderIndex = (uint32_t)(typicalIat[elementIndex]);
        }

        newElement.thunk.rva = elementRva;
        newElement.thunk.offset = utils::RvaToRaw(image, elementRva);
        elementRva += sizeof(VirtualAddress_t);

        VirtualAddress_t thunkValue = importLookupTable[elementIndex];

        bool importedByName = (thunkValue & ordinalFlag) == 0;
        if (importedByName) {
            std::streampos thunkOffset = utils::RvaToRaw(image, static_cast<uint32_t>(thunkValue));

            stream.seekg(thunkOffset);
            stream.read((char*)(&newElement.hint), sizeof(newElement.hint));
            newElement.name = Io::StreamUtils::readString(stream);            
        }
        else
            newElement.ordinal = static_cast<uint32_t>(thunkValue);

        // Detect static hook function
        if (doesBindingUsed && !typicalIat.empty()) {
            VirtualAddress_t addr = typicalIat[elementIndex];

            bool isItHook = (addr >= imageBase) && (addr < (imageBase+imageSize));
            if (isItHook) {
                newElement.hook.rva = static_cast<uint32_t>(addr - imageBase);
                newElement.hook.offset = utils::RvaToRaw(image, newElement.hook.rva);
            }
        }
        elements->emplace_back(newElement);
    }
}

template<typename Arch>
std::list<import::Module> ImportedModules(const Image<Arch>& image)
{
    std::list<import::Module> result;

    assert(image.getStream().good());

    const auto& optHeader = image.getNtHeaders().OptionalHeader;
    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);
    uint32_t imageSize = aligner.getVirtualSize(optHeader.SizeOfImage);

    for (const auto& import : image.getImportDirHeaders()) {
        result.emplace_back(import::Module());

        auto& newModule = *(result.rbegin());

        std::streampos namePos = utils::RvaToRaw(image, import.Name);
        newModule.name = Io::StreamUtils::readStringAt(image.getStream(), namePos);

        if (utils::detectBindingImportType(import.TimeDateStamp) == utils::BindingImportType::Old)
            newModule.timestampUTC0 = Time::unixtimeToString(import.TimeDateStamp);

        extractModuleElements(image, import, imageSize, &newModule.elements);
    }
    assert(image.getStream().good());

    return result;
}


template  std::list<import::Module> ImportedModules(const Image<arch32>& image);
template  std::list<import::Module> ImportedModules(const Image<arch64>& image);


}} // namespace pe::parse
