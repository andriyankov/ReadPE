#include "std.hpp"
#include "pe.hpp"


namespace pe 
{


using namespace Fw;


bool inline needAlign(uint32_t sectionAlignement)
{
    const uint32_t MIN_SECTION_ALIGNMENT = 0x1000;
    return sectionAlignement >= MIN_SECTION_ALIGNMENT;
}

Aligner::Aligner(uint32_t fileAlignment_, uint32_t sectionAlignement_)
    : fileAlignment(fileAlignment_)
    , sectionAlignement(sectionAlignement_)
{
}

uint32_t Aligner::getVirtualSize(uint32_t size)
{
    return needAlign(sectionAlignement) ?
        Math::alignUp(size, sectionAlignement) :
        size;
}

uint32_t Aligner::getVirtualAddress(uint32_t address)
{
    return needAlign(sectionAlignement) ?
        Math::alignDown(address, sectionAlignement) :
        address;
}

uint32_t Aligner::getFileOffset(uint32_t offset)
{
    const uint32_t FORCED_FILE_ALIGNMENT = 0x200;
    return needAlign(sectionAlignement) ?
        Math::alignDown(offset, FORCED_FILE_ALIGNMENT) :
        offset;
}

uint32_t Aligner::getSectionSize(const ImgSectionHeader& header)
{
    uint32_t fileSize = header.SizeOfRawData;
    uint32_t virtualSize = header.Misc.VirtualSize;
    if (needAlign(sectionAlignement)) {
        fileSize = Math::alignUp(fileSize, fileAlignment);
        virtualSize = Math::alignUp(virtualSize, sectionAlignement);
    }
    return std::min(fileSize, virtualSize);
}


} // namespace pe
