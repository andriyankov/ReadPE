#include "std.hpp"


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable:4996)
#endif


namespace Framework {
namespace Time {


std::string unixtimeToString(time_t timeStamp)
{
    enum  
    { 
        numCStringSize = 0x80 
    };
    char timeCStr[numCStringSize];
    tm * tmInfo = gmtime(&timeStamp);

    std::string resultStr;
    if (tmInfo > 0) {
        char * strBegin = &timeCStr[0];
        char * strEnd = strBegin + numCStringSize;

        const char strTimestampFmt[] = "%a, %Y-%b-%d, %H:%M:%S";

        strftime(strBegin, numCStringSize, strTimestampFmt, tmInfo);
        char * nullCharPtr = std::find(strBegin, strEnd, 0);
        size_t timeStrLen = nullCharPtr - strBegin;
        resultStr.assign(timeCStr, timeStrLen);
    }
    return std::string(resultStr);
}


}} // namespace Framework::Time


#if _MSC_VER == 1800
#   pragma warning(pop)
#endif
