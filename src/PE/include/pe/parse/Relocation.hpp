namespace parse
{


struct Relocation final
{
    int    type;
    uint32_t  rva;
    uint32_t  offset;
};


template<typename Arch>
std::list<Relocation> Relocations(const Image<Arch>& image);


} // namespace parse
