namespace headers_utils
{


struct Info final
{
    uint32_t pos;
    size_t   size;
};


template<typename Arch>
const Info getSectionHeadersInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getExportDirInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getImportDirsInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getTlsDirInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getBoundImportDirInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getRelocationDirInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getTlsCallbacksInfo(const Image<Arch>& image_);

template<typename Arch>
const Info getImportedElementsInfo(const Image<Arch>& image_, uint32_t rva);

template<typename Arch>
const Info getImgLoadConfigDirectoryInfo(const Image<Arch>& image_);


} // namespace headers_utils
