#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


std::vector<uint16_t> readRelocations(std::istream& stream, std::streampos pos, size_t size)
{
    std::vector<uint16_t> result(size / sizeof(uint16_t));
    if (result.empty())
        return std::move(result);

    try {
        stream.seekg(pos + static_cast<std::streamsize>(sizeof(ImgBaseRelocation)));
        stream.read(reinterpret_cast<char*>(&result[0]), size);
    }
    catch (const std::ios::failure&) {
        result.clear();
    }
    return std::move(result);
}

template<typename Arch>
std::list<Relocation> Relocations(const Image<Arch>& image)
{
    std::list<Relocation> result;

    assert(image.getStream().good());

    const auto& dirHeaders = image.getRelocationDirHeaders();
    if (dirHeaders.empty())
        return std::move(result);

    std::istream& stream = image.getStream();
    std::streampos relocationsPos = headers_utils::getRelocationDirInfo(image).pos;

    for (const auto& header : dirHeaders) {
        size_t  sizeOfRelocations = header.SizeOfBlock - sizeof(ImgBaseRelocation);
        auto relocations = readRelocations(stream, relocationsPos, sizeOfRelocations);

        for (const auto& reloc : relocations) {
            const uint16_t RELOC_OFFSET_MASK = 0xFFF;
            const uint16_t RELOC_TYPE_MASK = 12;

            const auto& type = (reloc >> RELOC_TYPE_MASK);
            const auto& rva = header.VirtualAddress + (reloc & RELOC_OFFSET_MASK);
            const auto& offset = utils::RvaToRaw(image, rva);
            result.emplace_back(Relocation{type, rva, offset});
        }
        relocationsPos += header.SizeOfBlock;
    }
    assert(image.getStream().good());

    return std::move(result);
}


template  std::list<Relocation> Relocations(const Image<arch32>& image);
template  std::list<Relocation> Relocations(const Image<arch64>& image);


}} // namespace pe::parse
