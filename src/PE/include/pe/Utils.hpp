namespace utils
{


const uint32_t  INVALID_RVA = std::numeric_limits<uint32_t>::max();
const uint32_t  INVALID_RAW = std::numeric_limits<uint32_t>::max();
const uint16_t  INVALID_SECTION_INDEX = std::numeric_limits<uint16_t>::max();


bool isPeOffsetCorrected(std::streampos offset);

std::string getSectionName(const unsigned char* c_str);

template<typename Arch>
uint32_t RvaToRaw(const Image<Arch>& image, uint32_t rva);

template<typename Arch>
uint32_t RawToRva(const Image<Arch>& image, uint32_t raw);

template<typename Arch>
uint16_t RvaToSectionIndex(const Image<Arch>& image, uint32_t rva);

template<typename Arch>
uint16_t RawToSectionIndex(const Image<Arch>& image, uint32_t raw);

template<typename Arch>
uint16_t getLastFileSection(const Image<Arch>& image);

template<typename Arch>
uint16_t getLastVirtualSection(const Image<Arch>& image);

template<typename Arch>
std::streamsize getRealSectionSize(const Image<Arch>& image, uint32_t raw);

template<typename Arch>
std::streamsize correctFileDataSize(const Image<Arch>& image, uint32_t raw, std::streamsize size);

template<typename Arch>
bool isDirectoryExist(const Image<Arch>& image, int dirIndex);


} //namespace utils
