#include "std.hpp"
#include "readpe.hpp"


using namespace pe;


void writeToLog(const std::string& errName, const std::string& errText)
{
    std::string s(errName);
    s += " : " + errText + "\n";
    std::cerr << s;
    std::cerr.flush();
}

void appTerminatedHandler()
{
    exit(static_cast<int>(readpe::ErrorCode::Unhandled));
}

void appUnexpectedExceptionHandler()
{
    throw std::bad_exception("Unexpected exception");
}

int main(int argc, char* argv[])
{
    using namespace readpe;

    set_terminate(appTerminatedHandler);
    set_unexpected(appUnexpectedExceptionHandler);

    ErrorCode exitCode = ErrorCode::Success;
    try {
        const ProgramOptions options = cmdline::parse(argc,argv);
        if (cmdline::isProgramOptionsSet(options)) {
            writeToLog("File", options.inputFilename);
            PeDumper::dump(options);
        }
    }
    catch (const ReadpeException& e) {
        writeToLog("Application Error", e.what());
        exitCode = e.error_code();
    }
    catch (const std::exception& e) {
        writeToLog("Std Library error", e.what());
        exitCode = ErrorCode::StdLibrary;
    }
    return static_cast<int>(exitCode);
}
