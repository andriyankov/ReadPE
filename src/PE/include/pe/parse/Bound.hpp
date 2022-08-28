namespace parse
{


struct BoundDescriptor final
{
    std::string    timestamp;
    std::string    moduleName;
    uint32_t       moduleNameRva;
    std::streampos moduleNamePos;
};


template<typename Arch>
std::list<BoundDescriptor> BoundImport(const Image<Arch>& image);


} // namespace parse
