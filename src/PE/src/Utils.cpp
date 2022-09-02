#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace utils {


using namespace Fw;


size_t getSectionNameSize(const char * ptr)
{
    const size_t MAX_SECTION_NAME_SIZE = 8;
    auto begin = std::reverse_iterator<const char*>(ptr);
    auto end = std::reverse_iterator<const char*>(ptr + MAX_SECTION_NAME_SIZE);
    auto rfound = std::find_if(end, begin, 
        [](const char ch) { return ch != 0x0; }
    );
    return MAX_SECTION_NAME_SIZE - (rfound - end);
}

bool isPeOffsetCorrected(std::streampos offset)
{
    return (offset != Io::INVALID_OFFSET) && (offset > 0);
}

std::string getSectionName(const unsigned char* c_str)
{
    auto sz = getSectionNameSize(reinterpret_cast<const char*>(c_str));
    return StringUtils::escape_sequence(c_str, sz);
}

template<typename Arch>
uint32_t RvaToRaw(const Image<Arch>& image, uint32_t rva)
{
    const auto& optHeader = image.getNtHeaders().OptionalHeader;
    if (rva < optHeader.SizeOfHeaders)
        return rva;

    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

    uint16_t sectionsCount = image.getNtHeaders().FileHeader.NumberOfSections;
    if (sectionsCount > 0) {
        for (const auto& sectionHeader : image.getSectionHeaders()) {
            if (sectionHeader.PointerToRawData > 0) {
                uint32_t VA = aligner.getVirtualAddress(sectionHeader.VirtualAddress);
                uint32_t sectionSize = aligner.getSectionSize(sectionHeader);

                if (VA <= rva && rva < (VA + sectionSize)) {
                    uint32_t FO = aligner.getFileOffset(sectionHeader.PointerToRawData);
                    FO += rva-VA;

                    if (FO > (uint32_t)image.sizeOfFile())
                        return INVALID_RAW;
                    return FO;
                }
            }
        } // for
    }
    else if (rva < aligner.getVirtualSize(optHeader.SizeOfImage)) {
        return rva;
    }
    return INVALID_RAW;
}

template<typename Arch>
uint32_t RawToRva(const Image<Arch>& image, uint32_t raw)
{
    const auto& optHeader = image.getNtHeaders().OptionalHeader;
    if (raw < optHeader.SizeOfHeaders)
        return raw;

    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

    uint32_t imgSize = aligner.getVirtualSize(optHeader.SizeOfImage);

    uint16_t sectionsCount = image.getNtHeaders().FileHeader.NumberOfSections;
    if (sectionsCount > 0) {
        for (const auto& sectionHeader : image.getSectionHeaders()) {
            if (sectionHeader.PointerToRawData > 0) {
                uint32_t FO = aligner.getFileOffset(sectionHeader.PointerToRawData);
                uint32_t sectionSize = aligner.getSectionSize(sectionHeader);

                if (raw >= FO && raw < FO + sectionSize) {
                    uint32_t VA = aligner.getVirtualAddress(sectionHeader.VirtualAddress);
                    VA += raw-FO;

                    return (VA > imgSize) ? INVALID_RVA : VA;
                }
            }
        } // for
    }
    else if(raw < imgSize)
        return raw;

    return INVALID_RVA;
}

template<typename Arch>
uint16_t RvaToSectionIndex(const Image<Arch>& image, uint32_t rva)
{
    uint16_t resultIndex = 0;

    uint16_t sectionsCount = image.getNtHeaders().FileHeader.NumberOfSections;
    if (sectionsCount > 0) {

        const auto& optHeader = image.getNtHeaders().OptionalHeader;
        Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

        for (const auto& sectionHeader : image.getSectionHeaders()) {
            uint32_t VA = aligner.getVirtualAddress(sectionHeader.VirtualAddress);
            uint32_t sectionSize = aligner.getSectionSize(sectionHeader);

            if (rva >= VA && rva < VA + sectionSize)
                break;
            else
                resultIndex++;
        } // for
    }
    return resultIndex < sectionsCount ? resultIndex : INVALID_SECTION_INDEX;
}

template<typename Arch>
uint16_t RawToSectionIndex(const Image<Arch>& image, uint32_t raw)
{
    uint16_t resultIndex = 0;

    uint16_t sectionsCount = image.getNtHeaders().FileHeader.NumberOfSections;
    if (sectionsCount > 0) {

        const auto& optHeader = image.getNtHeaders().OptionalHeader;
        Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

        for (uint16_t i = 0; i < sectionsCount; i++) {
            const auto& sectionHeader = image.getSectionHeaders()[i];
            if (sectionHeader.PointerToRawData > 0) {
                uint32_t FO = aligner.getFileOffset(sectionHeader.PointerToRawData);
                uint32_t sectionSize = aligner.getSectionSize(sectionHeader);

                if (raw >= FO && raw < FO + sectionSize)
                    resultIndex = i;
            }
        } // for
    }
    return resultIndex < sectionsCount ? resultIndex : INVALID_SECTION_INDEX;
}

template<typename Arch>
uint16_t getLastFileSection(const Image<Arch>& image)
{
    uint16_t resultIndex = 0;

    const auto& ntHeaders = image.getNtHeaders();
    uint16_t sectionsCount = ntHeaders.FileHeader.NumberOfSections;
    if (sectionsCount > 0) {

        const auto& optHeader = image.getNtHeaders().OptionalHeader;
        Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

        uint32_t FO = aligner.getFileOffset(image.getSectionHeaders()[0].PointerToRawData);

        for (uint16_t i = 1; i < sectionsCount; ++i) {
            const auto& sectionHeader = image.getSectionHeaders()[i];
            uint32_t cur = aligner.getFileOffset(sectionHeader.PointerToRawData);

            if (FO < cur && sectionHeader.PointerToRawData > 0) {
                FO = cur;
                resultIndex = i;
            }
        }
    }
    return resultIndex < sectionsCount ? resultIndex : INVALID_SECTION_INDEX;
}

template<typename Arch>
uint16_t getLastVirtualSection(const Image<Arch>& image)
{
    uint16_t resultIndex = 0;

    const auto& ntHeaders = image.getNtHeaders();
    uint16_t sectionsCount = ntHeaders.FileHeader.NumberOfSections;
    if (sectionsCount > 0) {

        const auto& optHeader = image.getNtHeaders().OptionalHeader;
        Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

        uint32_t VA = aligner.getVirtualAddress(image.getSectionHeaders()[0].VirtualAddress);

        for (uint16_t i = 1; i < sectionsCount; ++i) {
            const auto& sectionHeader = image.getSectionHeaders()[i];
            uint32_t cur = aligner.getVirtualAddress(sectionHeader.VirtualAddress);

            if (VA < cur && sectionHeader.PointerToRawData > 0) {
                VA = cur;
                resultIndex = i;
            }
        }
    }
    return resultIndex < sectionsCount ? resultIndex : INVALID_SECTION_INDEX;
}

template<typename Arch>
std::streamsize getRealSectionSize(const Image<Arch>& image, uint32_t raw)
{
    std::streamsize resultSize = Io::MAX_SIZE_OF_STREAM;
    uint16_t i = RawToSectionIndex(image, raw);

    if (i != INVALID_SECTION_INDEX) {
        const auto& sectionHeader = image.getSectionHeaders()[i];

        const auto& optHeader = image.getNtHeaders().OptionalHeader;
        Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

        resultSize = aligner.getSectionSize(sectionHeader);

        assert(resultSize < image.sizeOfFile());
    }
    return resultSize;
}

template<typename Arch>
std::streamsize correctFileDataSize(const Image<Arch>& image, uint32_t dataPos, std::streamsize dataSize)
{
    const auto& optHeader = image.getNtHeaders().OptionalHeader;
    uint16_t i = RawToSectionIndex(image, dataPos);

    if (i == INVALID_SECTION_INDEX) {
        bool isPosInHeaders = (dataPos < optHeader.SizeOfHeaders);
        return isPosInHeaders ? dataSize : Io::MAX_SIZE_OF_STREAM ;
    }

    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

    const auto& sectionHeader = image.getSectionHeaders()[i];
    uint32_t sectionPos = aligner.getFileOffset(sectionHeader.PointerToRawData);
    uint32_t sectionSize = aligner.getSectionSize(sectionHeader);

    // rfo is : Related File offset From Sections Start offset
    uint32_t rfo = dataPos - sectionPos;
    std::streamsize finishPortionSize = (std::streamsize) (sectionSize - rfo);
    
    std::streamsize resultSize = dataSize;
    if (dataSize > finishPortionSize)
        resultSize -= (dataSize - finishPortionSize);

    return resultSize;
}

template<typename Arch>
bool isDirectoryExist(const Image<Arch>& image, int dirIndex)
{
    assert(dirIndex < NumberOfDirectoryEntries);

    const auto& dataDirs = image.getNtHeaders().OptionalHeader.DataDirectory;
    uint32_t raw = RvaToRaw(image, dataDirs[dirIndex].VirtualAddress);

    return (raw > 0) && (raw != INVALID_RAW);
}


template uint32_t RvaToRaw(const Image<arch32>& image, uint32_t rva);
template uint16_t RvaToSectionIndex(const Image<arch32>& image, uint32_t rva);
template uint16_t RawToSectionIndex(const Image<arch32>& image, uint32_t raw);
template uint16_t getLastFileSection(const Image<arch32>& image);
template uint16_t getLastVirtualSection(const Image<arch32>& image);
template std::streamsize getRealSectionSize( const Image<arch32>& image, uint32_t raw );
template std::streamsize correctFileDataSize(const Image<arch32>& image, uint32_t dataPos, std::streamsize dataSize);
template bool isDirectoryExist(const Image<arch32>& image, int dirIndex);

template uint32_t RvaToRaw(const Image<arch64>& image, uint32_t rva);
template uint16_t RvaToSectionIndex(const Image<arch64>& image, uint32_t rva);
template uint16_t RawToSectionIndex(const Image<arch64>& image, uint32_t raw);
template uint16_t getLastFileSection(const Image<arch64>& image);
template uint16_t getLastVirtualSection(const Image<arch64>& image);
template std::streamsize getRealSectionSize( const Image<arch64>& image, uint32_t raw );
template std::streamsize correctFileDataSize(const Image<arch64>& image, uint32_t dataPos, std::streamsize dataSize);
template bool isDirectoryExist(const Image<arch64>& image, int dirIndex);


}} // namespace pe::utils
