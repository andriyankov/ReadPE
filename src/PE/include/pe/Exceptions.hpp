class Exception
    : public std::runtime_error
{
public:
    Exception(const char * errTxt)
        : std::runtime_error(errTxt)
    {
    }
};

class FormatException final
    : public Exception
{
public:
    FormatException(const char * errTxt)
        : Exception(errTxt)
    {
    }
};

class CorruptedException final
    : public Exception
{
public:
    CorruptedException(const char * errTxt)
        : Exception(errTxt)
    {
    }
};

class UnsupportedException final
    : public Exception
{
public:
    UnsupportedException(const char * errTxt)
        : Exception(errTxt)
    {
    }
};
