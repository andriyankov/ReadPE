
class ReadpeException 
    : public std::runtime_error
{
public:
    ReadpeException(const char * txt, ErrorCode code_)
        : std::runtime_error( txt)
        , code(code_)
    {
    }

    ErrorCode error_code() const
    {
        return code;
    }

private:
    ErrorCode code;
};


class ConfigureException final
    : public ReadpeException
{
public:
    ConfigureException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::Configure)
    {
    }
};


class CorruptedException final
    : public ReadpeException
{
public:
    CorruptedException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::Corrupted)
    {
    }
};


class FormatException final
    : public ReadpeException
{
public:
    FormatException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::Format)
    {
    }
};


class UnsupportedException final
    : public ReadpeException
{
public:
    UnsupportedException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::Unsupported)
    {
    }
};


class SystemException final
    : public ReadpeException
{
public:
    SystemException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::System)
    {
    }
};


class NotImplementedException final
    : public ReadpeException
{
public:
    NotImplementedException(const std::string& s)
        : ReadpeException(s.c_str(), ErrorCode::NotImplemented)
    {
    }
};
