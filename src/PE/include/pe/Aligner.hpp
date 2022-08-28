class Aligner final
{
public:
    explicit Aligner(uint32_t fileAlignment_, uint32_t sectionAlignement_);

    uint32_t getFileOffset(uint32_t offset);
    uint32_t getVirtualAddress(uint32_t address);
    uint32_t getSectionSize(const ImgSectionHeader& header);
    uint32_t getVirtualSize(uint32_t size);

private:
    uint32_t fileAlignment;
    uint32_t sectionAlignement;
};
