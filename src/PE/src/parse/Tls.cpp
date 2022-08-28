#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace parse {


template<typename Arch>
using VirtualAddress_t = typename headers_traits<Arch>::VirtualAddress_type;

template<typename Arch>
std::vector<VirtualAddress_t<Arch>> readTlsCallbacks(const Image<Arch>& image)
{
    std::vector<VirtualAddress_t<Arch>> result;

    const auto& info = headers_utils::getTlsCallbacksInfo(image);
    if (info.pos == utils::INVALID_RAW)
        return std::move(result);

    using VirtualAddress_t = headers_traits<Arch>::VirtualAddress_type;

    auto count = info.size / sizeof(VirtualAddress_t);
    if (count == 0)
        return std::move(result);

    result.resize(count);
    try {
        std::istream& stream = image.getStream();
        stream.seekg(info.pos);
        stream.read(reinterpret_cast<char*>(&result[0]), info.size);
    }
    catch (const std::ios::failure&) {
        result.clear();
        return std::move(result);
    }
    return std::move(result);
}

template<typename Arch>
std::list<TlsCallback> TlsCallbacks(const Image<Arch>& image)
{
    std::list<TlsCallback> result;

    assert(image.getStream().good());
  
    using VirtualAddress_t = headers_traits<Arch>::VirtualAddress_type;

    VirtualAddress_t imageBase = image.getNtHeaders().OptionalHeader.ImageBase;

    const auto callbacks = readTlsCallbacks(image);
    for (const auto& address : callbacks) {
        const auto rva = static_cast<uint32_t>(address - imageBase);
        const auto offset = utils::RvaToRaw(image, rva);
        result.emplace_back(TlsCallback{ rva, offset });
    }
    assert(image.getStream().good());

    return std::move(result);
}


template  std::list<TlsCallback> TlsCallbacks(const Image<arch32>& image);
template  std::list<TlsCallback> TlsCallbacks(const Image<arch64>& image);


}} // namespace pe::parse
