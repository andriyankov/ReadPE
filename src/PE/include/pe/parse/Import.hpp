namespace parse
{

namespace import
{

struct Address final
{
    uint32_t        rva = 0;
    std::streampos  offset = 0;
};

struct Element final
{
    std::string name;
    bool        isForwarded = false;
    uint16_t    hint = 0;
    uint32_t    ordinal = 0;
    Address     thunk;
    Address     hook;
};

struct Module final
{
    std::string name;
    std::string timestampUTC0;
    std::list<Element> elements;
};

} // namespace import


template<typename Arch>
std::list<import::Module> ImportedModules(const Image<Arch>& image);


} // namespace parse
