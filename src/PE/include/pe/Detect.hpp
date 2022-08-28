namespace detect
{


enum class Platform
{
    Unknown = 0,
    Unsupported,

    Arch32,
    Arch64,
};


Platform detectPlatform(std::istream& stream);
bool     isMachineSupported(uint16_t machine);


} // namespace detect
