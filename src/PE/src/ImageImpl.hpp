
namespace pe
{


template<typename Arch>
class Image<Arch>::Impl final
{
public:
    explicit Impl(std::iostream& stream, Image<Arch>& image_);

    Image<Arch>&      image;
    mutable uint64_t  readOperations;
    std::streamsize   imgSize;
    std::iostream&    imgStream;
    ImgDosHeader      dosHeader;
    NtHeaders_t<Arch> ntHeaders;
    SectionHeaders_t  sectionHeaders;
    ImportDescriptors_t   importsDirectories;
    ImgExportDirectory    exportDirectory;
    TlsDirectory_t<Arch>  tlsDirectory;
    BaseRelocationList_t      relocationDirectories;
    BoundImportDescriptors_t  boundDirectories;
    LoadConfigDirectory_t<Arch> loadConfigDirectory;

    void clear();

    void readDosHeader();
    void readNtHeaders();
    void readNtSections();
    void readExportDirectoryHeader();
    void readImportDirectoryHeaders();
    void readTlsDirectoryHeader();
    void readBoundDirectoryHeaders();
    void readRelocationDirectories();
    void readLoadConfigDirectory();

    void writeDosHeader();
    void writeNtHeaders();
    void writeNtSections();
    void writeExportDirectoryHeader();
    void writeImportDirectoryHeaders();
    void writeTlsDirectoryHeader();
    void writeBoundDirectoryHeaders();
    void writeLoadConfigDirectory();
};


} // namespace pe
