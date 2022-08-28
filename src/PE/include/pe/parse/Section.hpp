namespace parse
{


struct Section final
{
    double   entropy;
    uint32_t realOffset;
    uint32_t realSize;
    std::string name;
    std::list<std::string> flags;
};


template<typename Arch>
std::list<Section> Sections(const Image<Arch>& image);


} // namespace parse
