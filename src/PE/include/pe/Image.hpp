
template<typename Arch>
using NtHeaders_t = typename headers_traits<Arch>::ImgNtHeaders;

template<typename Arch>
using TlsDirectory_t = typename headers_traits<Arch>::ImgTlsDirectory;

template<typename Arch>
using LoadConfigDirectory_t = typename headers_traits<Arch>::ImgLoadConfigDirectory;

using  SectionHeaders_t = std::vector<ImgSectionHeader>;
using  ImportDescriptors_t = std::vector<ImgImportDescriptor>;
using  BoundImportDescriptors_t = std::vector<ImgBoundImportDescriptor>;
using  BaseRelocationList_t = std::list<ImgBaseRelocation>;


template<typename Arch>
class Image final
{
public:
    Image(const Image&) = delete;
    const Image& operator=(const Image&) = delete;

    explicit Image(std::iostream& stream, bool lazyMode=true);
    ~Image();

    std::streamsize sizeOfFile() const;

    std::iostream& getStream() const;
    std::iostream& getStream();

    void clear();
    void read();
    void write();


    const ImgDosHeader& getDosHeader() const;
    ImgDosHeader& getDosHeader();

    const NtHeaders_t<Arch>& getNtHeaders() const;
    NtHeaders_t<Arch>& getNtHeaders();

    const SectionHeaders_t& getSectionHeaders() const;
    SectionHeaders_t& getSectionHeaders();

    const ImgExportDirectory& getExportDirHeader() const;
    ImgExportDirectory& getExportDirHeader();

    const ImportDescriptors_t& getImportDirHeaders() const;
    ImportDescriptors_t& getImportDirHeaders();

    const TlsDirectory_t<Arch>& getTlsDirHeader() const;
    TlsDirectory_t<Arch>& getTlsDirHeader();

    const BoundImportDescriptors_t& getBoundDirHeaders() const;
    BoundImportDescriptors_t& getBoundDirHeaders();

    const BaseRelocationList_t& getRelocationDirHeaders() const;
    BaseRelocationList_t& getRelocationDirHeaders();

    const LoadConfigDirectory_t<Arch>& getImgLoadConfigDirectory() const;
    LoadConfigDirectory_t<Arch>& ImgLoadConfigDirectory();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};


typedef  Image<arch32>  Image32;
typedef  Image<arch64>  Image64;
