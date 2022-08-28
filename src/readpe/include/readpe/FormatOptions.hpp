
namespace detailed 
{
    struct HeaderOptions
    {
        bool ntSections;
        bool exportHeader;
        bool importHeaders;
        bool boundHeaders;
        bool tlsHeader;
        bool fixupHeaders;
        bool loadConfigHeader;
    };
} // namespace detailed {

struct RawOptions final : public detailed::HeaderOptions
{
    bool dosHeader;
    bool ntHeaders;
};

struct DetailedOptions final : public detailed::HeaderOptions
{
};

struct FormatOptions final
{
    RawOptions rawOptions;
    DetailedOptions detailedOptions;
};
