#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


using namespace Fw;


const char* strSectionFlags[] =
{
  /* 00 */ // 0, Skipped
  /* 01-05 */ 0,0,0,0,  0,
  /* 06 */    "code",
  /* 07 */    "initialized",
  /* 08 */    "uninitialized",
  /* 09 */    0,
  /* 10 */    "info",
  /* 11 */    0,
  /* 12 */    "remove",
  /* 13 */    "comdat",
  /* 14-15 */ 0,0,
  /* 16 */    "far data",
  /* 17 */    0,
  /* 18 */    "purgeable or 16 bit",
  /* 19 */    "locked",
  /* 20 */    "preload",
  /* 21 */    "align",
  /* 22 */    "align",
  /* 23 */    "align", 
  /* 24 */    "align",
  /* 25 */    "extended relocations",
  /* 26 */    "can be discarded",
  /* 27 */    "not cachable",
  /* 28 */    "not pageable",
  /* 29 */    "shareable",
  /* 30 */    "executable",
  /* 31 */    "readable",
  /* 32 */    "writeable"
};

std::string convertAlignFlagToString(uint8_t alignFlag)
{
    assert(alignFlag > 0 && alignFlag < 0x10);

    int resultNumber = 1 << (alignFlag - 1);
    std::string resultStr("align " + std::to_string(resultNumber) + " bytes");

    assert(!resultStr.empty());
    return resultStr;
}

std::list<std::string> convertSectionFlagsToStrings(uint32_t flags)
{
    std::list<std::string> result;

    assert(flags > 0);
    uint32_t srcFlags = flags;

    enum { bitsCount = sizeof(srcFlags) * 8 };

    for (int flagIndex=0; srcFlags > 0 && flagIndex < bitsCount ; flagIndex++) {
        bool isFlagSet = (srcFlags & 1) > 0 ;
        bool isFlagStrExists = strSectionFlags[flagIndex] > 0 ;
        if (isFlagSet && isFlagStrExists) {
            const int Align1bytesIndex = 20;
            const int Align8192bytesIndex = 23;

            bool isAligned = (flagIndex > Align1bytesIndex) && (flagIndex <= Align8192bytesIndex) ;
            result.emplace_back(isAligned 
                ? convertAlignFlagToString(srcFlags & 0x0F) 
                : strSectionFlags[flagIndex]);
        }
        srcFlags >>= 1;
    }
    assert(!result.empty());

    return std::move(result);
}

template<typename Arch>
Section makeSection(const ImgSectionHeader& header, const Image<Arch>& image)
{
    Section result;

    assert(image.getStream().good());

    const auto& optHeader = image.getNtHeaders().OptionalHeader;
    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);

    result.name = utils::getSectionName(&header.Name[0]);
    result.realOffset = aligner.getFileOffset(header.PointerToRawData);
    result.realSize = aligner.getSectionSize(header);
    result.flags = convertSectionFlagsToStrings(header.Characteristics);

    result.entropy = Math::entropy(image.getStream(),
        static_cast<std::streampos>(result.realOffset),
        static_cast<std::streamsize>(result.realSize)
    );
    assert(image.getStream().good());

    return std::move(result);
}

template<typename Arch>
std::list<Section> Sections(const Image<Arch>& image)
{
    std::list<Section> result;

    for (const auto& header : image.getSectionHeaders())
        result.emplace_back(makeSection(header, image));

    return std::move(result);
}


template  std::list<Section> Sections(const Image<arch32>& image);
template  std::list<Section> Sections(const Image<arch64>& image);


}} // namespace pe::parse
