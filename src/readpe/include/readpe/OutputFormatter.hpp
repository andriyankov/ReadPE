
template<typename Arch>
class OutputFormatter final
{
public:
    OutputFormatter(const pe::Image<Arch>& image, const FormatOptions& options);
    ~OutputFormatter() {}

    const std::string& format();

private:
    FormatOptions formatOptions;
    const pe::Image<Arch>& image_;
    json::Object jsonObj;
    mutable std::string  resultStr;

    const std::string& str() const;

    void computeAndSetRealFormatOptions();

    void formatDosHeader();
    void formatNtHeaders();
    void formatSectionHeaders();
    void formatExportHeader();
    void formatImportHeaders();
    void formatBoundHeaders();
    void formatTlsHeader();
    void formatRelocationHeaders();
    void formatLoadConfigDirHeader();

    void formatSectionsDetails();
    void formatExportDetails();
    void formatImportsDetails();
    void formatBoundsDetails();
    void formatTlsDetails();
    void formatRelocationsDetails();

    void formatRawOptions();
    void formatDetailedOptions();

    void formatFileHeader();
    void formatOptionalHeader();

    void formatImportDescriptor(const pe::ImgImportDescriptor& import, 
        json::UnknownElement& jsonImportDir);

    void formatExportElement(const pe::parse::Export::Element& element,
        json::UnknownElement & jsonExpDir);

    void formatBoundDescriptor(const pe::parse::BoundDescriptor& descriptor,
        json::UnknownElement& jsonBound);
};
