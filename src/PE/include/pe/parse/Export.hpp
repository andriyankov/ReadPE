namespace parse
{


class Export final
{
public:
    struct Element final
    {
        std::string     name; // Name is empty if exported only by ordinal
        uint32_t        rva;
        std::streampos  offset;
        uint16_t        ordinal;
        std::string     forwardName;
    };

public:
    template<typename Arch>
    Export(const Image<Arch>& image);
    
    const std::string& timeDateStamp() const;
    const std::list<Element>& elements() const;

private:
    std::string         timeDateStamp_;
    std::list<Element>  elements_;

    std::vector<uint32_t> names_;
    std::vector<uint16_t> ordinals_;
    std::vector<uint32_t> rvas_;

    template<typename Arch>
    bool readNamesOrdinalsRvas(const Image<Arch>& image);
};


} // namespace parse
