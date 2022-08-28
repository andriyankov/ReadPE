namespace parse
{


struct TlsCallback final
{
    uint32_t     rva;
    std::streampos offset;
};


template<typename Arch>
std::list<TlsCallback> TlsCallbacks(const Image<Arch>& image);


} // namespace parse
