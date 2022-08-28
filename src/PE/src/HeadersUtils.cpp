#include "std.hpp"
#include "pe.hpp"


namespace pe {
namespace headers_utils {


template<typename T>
size_t computeSizeOfArray(std::iostream& stream, std::streampos pos,
    std::function<bool(const T&)> predicat)
{
    size_t result = 0;
    std::array<T, 100> tmp;
    auto find = [&result, &tmp, &predicat]() {
        auto found = std::find_if(tmp.begin(), tmp.end(), predicat);
        result += (found - tmp.begin());
        return found != tmp.end();
    };
    try {
        auto sz = sizeof(T) * tmp.max_size();
        stream.seekg(pos);
        while (stream.good()) {
            stream.read(reinterpret_cast<char*>(tmp.data()), sz);
            if (find())
                break;
        }
    }
    catch (const std::ios::failure&) {
        find();
        stream.clear();
    }
    return result * sizeof(T);
}

template<typename Arch>
const Info getInfo(const Image<Arch>& image_, uint32_t rva)
{
    Info result = {0, 0};
    result.pos = utils::RvaToRaw(image_, rva);

    using  VirtualAddress_t=headers_traits<Arch>::VirtualAddress_type;
    auto lastCallback = [&](const VirtualAddress_t& rva) {
        return (rva == 0);
    };
    result.size = computeSizeOfArray<VirtualAddress_t>(image_.getStream(),
        result.pos, lastCallback);
    return result;
}

template<typename Arch>
uint32_t getSectionHeadersOffset(const Image<Arch>& image_)
{
    uint32_t result = image_.getDosHeader().e_lfanew;
    result += (4 + sizeof(ImgFileHeader));
    result += image_.getNtHeaders().FileHeader.SizeOfOptionalHeader;
    return result;
}

template<typename Arch>
size_t getSectionHeadersSize(const Image<Arch>& image_)
{
    return image_.getNtHeaders().FileHeader.NumberOfSections *
        sizeof(ImgSectionHeader);
}

template<typename Arch>
size_t getImportDirsSize(const Image<Arch>& image_, uint32_t pos)
{
    const auto& optHeader = image_.getNtHeaders().OptionalHeader;
    Aligner aligner(optHeader.FileAlignment, optHeader.SectionAlignment);
    auto imageSize = aligner.getVirtualSize(optHeader.SizeOfImage);
    auto lastDescriptor = [&, imageSize](const ImgImportDescriptor& descriptor) {
        bool incorrectIat = (descriptor.FirstThunk > imageSize);
        bool incorrectNameRva = (descriptor.Name > imageSize);
        bool notsetNameRva = (descriptor.Name == 0);
        return incorrectIat || incorrectNameRva || notsetNameRva;
    };
    return computeSizeOfArray<ImgImportDescriptor>(image_.getStream(), pos, lastDescriptor);
}

template<typename Arch>
size_t getBoundImportDirSize(const Image<Arch>& image_, uint32_t pos)
{
    auto lastDescriptor = [&](const ImgBoundImportDescriptor& descriptor) {
        return (descriptor.OffsetModuleName == 0) && (descriptor.TimeDateStamp == 0);
    };
    return computeSizeOfArray<ImgBoundImportDescriptor>(image_.getStream(), pos, lastDescriptor);
}


template<typename Arch>
const Info getSectionHeadersInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    result.pos = getSectionHeadersOffset(image_);
    result.size = getSectionHeadersSize(image_);
    return result;
}

template<typename Arch>
const Info getExportDirInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::ExportEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = sizeof(ImgExportDirectory);
    }
    return result;
}

template<typename Arch>
const Info getImportDirsInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::ImportEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = getImportDirsSize(image_, result.pos);
    }
    return result;
}

template<typename Arch>
const Info getTlsDirInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::TlsEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = sizeof(headers_traits<Arch>::ImgTlsDirectory);
    }
    return result;
}

template<typename Arch>
const Info getBoundImportDirInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::BoundImportEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = getBoundImportDirSize(image_, result.pos);
    }
    return result;
}

template<typename Arch>
const Info getRelocationDirInfo(const Image<Arch>& image_)
{
    Info result = {0, 0};
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::BaserelocEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = dataDirs[Directory::BaserelocEntry].Size;
    }
    return result;
}

template<typename Arch>
const Info getTlsCallbacksInfo(const Image<Arch>& image_)
{
    auto rva = static_cast<uint32_t>(image_.getTlsDirHeader().AddressOfCallBacks -
        image_.getNtHeaders().OptionalHeader.ImageBase);
    return getInfo(image_, rva);
}

template<typename Arch>
const Info getImportedElementsInfo(const Image<Arch>& image_, uint32_t rva)
{
    return getInfo(image_, rva);
}

template<typename Arch>
const Info getImgLoadConfigDirectoryInfo(const Image<Arch>& image_)
{
    Info result = { 0, 0 };
    const auto& dataDirs = image_.getNtHeaders().OptionalHeader.DataDirectory;
    auto rva = dataDirs[Directory::LoadConfigEntry].VirtualAddress;
    if (rva > 0) {
        result.pos = utils::RvaToRaw(image_, rva);
        result.size = sizeof(headers_traits<Arch>::ImgLoadConfigDirectory);
    }
    return result;
}


template  const Info getSectionHeadersInfo(const Image<arch32>& image_);
template  const Info getExportDirInfo(const Image<arch32>& image_);
template  const Info getImportDirsInfo(const Image<arch32>& image_);
template  const Info getTlsDirInfo(const Image<arch32>& image_);
template  const Info getBoundImportDirInfo(const Image<arch32>& image_);
template  const Info getRelocationDirInfo(const Image<arch32>& image_);
template  const Info getRelocationDirInfo(const Image<arch32>& image_);
template  const Info getTlsCallbacksInfo(const Image<arch32>& image_);
template  const Info getImportedElementsInfo(const Image<arch32>& image_, uint32_t rva);
template  const Info getImgLoadConfigDirectoryInfo(const Image<arch32>& image_);


template  const Info getSectionHeadersInfo(const Image<arch64>& image_);
template  const Info getExportDirInfo(const Image<arch64>& image_);
template  const Info getImportDirsInfo(const Image<arch64>& image_);
template  const Info getTlsDirInfo(const Image<arch64>& image_);
template  const Info getBoundImportDirInfo(const Image<arch64>& image_);
template  const Info getRelocationDirInfo(const Image<arch64>& image_);
template  const Info getTlsCallbacksInfo(const Image<arch64>& image_);
template  const Info getImportedElementsInfo(const Image<arch64>& image_, uint32_t rva);
template  const Info getImgLoadConfigDirectoryInfo(const Image<arch64>& image_);


}} // namespace pe::headers_utils
