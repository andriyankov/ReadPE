#include "std.hpp"
#include "readpe.hpp"


namespace readpe {
namespace cmdline {


namespace po = boost::program_options;


const char * strApplicationName("readpe");
const char * strApplicationVersion( "0.1.0-r" );
const char * strApplicationAuthor("2012 - 2022 (c) e-mail: ntvisigoth@gmail.com");
const char * strAppDescription("Show information about PE32/PE32+ format files");
const char * strAppOpenSrcRepository("Repository URL: https://github.com/andriyankov/ReadPE");


std::pair<std::string, std::string> atResponseFileParser(const std::string &s)
{
    if ('@' == s[0])
        return std::make_pair(std::string("response-file"), s.substr(1));
    else
        return std::pair<std::string, std::string>();
}

void readResponseFile(const char * filename, std::vector<std::string> * args)
{
    using namespace std;

    ifstream ifs(filename);
    if (!ifs)
        throw ConfigureException("Could not open the response file");

    // Read the whole file into a string
    stringstream ss;
    ss << ifs.rdbuf();
    // Split the file content
    boost::char_separator<char> sep(" \n\r");
    string sstr = ss.str();
    boost::tokenizer< boost::char_separator<char> > tok(sstr, sep);
    copy(tok.begin(), tok.end(), std::back_inserter(*args));
}

template<typename Options_type>
void changeOptions(Options_type* options, bool state)
{
    options->ntSections = state;
    options->exportHeader = state;
    options->importHeaders = state;
    options->boundHeaders = state;
    options->fixupHeaders = state;
    options->tlsHeader= state;
    options->loadConfigHeader = state;
}

void changeProgramOptions(ProgramOptions * options, bool state)
{
    options->formatOptions.rawOptions.dosHeader = state;
    options->formatOptions.rawOptions.ntHeaders = state;
    changeOptions(&options->formatOptions.rawOptions, state);
    changeOptions(&options->formatOptions.detailedOptions, state);
}

po::options_description declareBasicOptions()
{
    po::options_description basic("Basic options");
    basic.add_options()
        ("donate", "Show electronic purses")
        ("help", "Produce help message")
        ("nologo", "Suppress copyright message")
        ("version", "Product version message")
        ("raw", "Show general \"raw\" options, Equals to -d -n -s" )
        ("detailed", "Show general \"detailed\" options, Equals to -S, -E, -I, -T, -B")
        ("all", "Equals to all \"raw\" and \"detailed\" options")
        ;
    return basic;
}

po::options_description declareRawOptions()
{
    po::options_description headers("Raw info options");
    headers.add_options()
        ("dos-header,d", "Show DOS Header")
        ("nt-headers,n", "Show NT Headers")
        ("sections,s", "Show Sections Headers")
        ("export,e", "Show Export Header" )
        ("imports,i", "Show Import Headers" )
        ("tls", "Show TLS Header")
        ("bounds", "Show Bound Headers")
        ("relocations",  "Show Relocations Headers")
        ("load-config", "Show Load Config Directory Header")
        ;
    return headers;
}

po::options_description declareDetailedOptions()
{
    po::options_description details("Detailed info options");
    details.add_options()
        ("sections-detailed,S", "Show Section (name, entropy, flags, real offset and size)")
        ("export-detailed,E", "Show Export (name, rva, offset and ordinal)")
        ("imports-detailed,I", "Show Import (name, rva, offset and hint and hook-function)")
        ("tls-detailed,T", "Show TLS (callback address and offset)")
        ("bounds-detailed,B", "Show Bounds (module name, timestamp, rva and offset)")
        ("relocations-detailed",  "Show Relocations (item rva, offset and type)")
        ;
    return details;
}

po::options_description declareHiddenOptions()
{
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("response-file", po::value<std::string>(),"Can be specified with '@name', too")
        ("input-file,f", po::value<std::string>(), "Input PE file")
        ;
    return hidden;
}

void printLogoMessage()
{
    using namespace std;

    string about(strApplicationName);
    about += " [ " + string(strApplicationVersion) + " ]" + '\n';
    about += string(strApplicationAuthor) + '\n';
    about += string(strAppOpenSrcRepository) + '\n';
    cout << about << endl;
    cout.flush();
}

void printHelpMessage()
{
    using namespace std;

    stringstream helpMsg;
    helpMsg << string(strAppDescription) << endl << endl;
    helpMsg << "Usage :" << endl; 
    helpMsg << "    " << string(strApplicationName) << " @response-file" << endl;
    helpMsg << "    " << string(strApplicationName) << " [ option(s) ] < pe-file >" << endl << endl;
    helpMsg << declareBasicOptions() << endl;
    helpMsg << declareRawOptions() << endl;
    helpMsg << declareDetailedOptions() << endl;
    cout << helpMsg.rdbuf();
    cout.flush();
}

void printVersionMessage()
{
    using namespace std;
    string version(strApplicationVersion);
    cout << version << endl;
    cout.flush();
}

void printWallets()
{
    using namespace std;

    stringstream stdoutMsg;
    stdoutMsg << "Yandex-Money:  " << "410012415964253" << '\n';
    stdoutMsg << "MIR Card:      " << "2204 1201 0546 5948" << '\n';
    stdoutMsg << "Ethereum(ETH): " << "0x97194f6534e904A2416fC0b02894F1782c65B1c5" << '\n';
    stdoutMsg << "Bitcoin(BTC):  " << "bc1qc94slcwvde9uafcc64ktue6q840jrjtxgtsme4" << '\n';
    stdoutMsg << "DAI(DAI):      " << "0x97194f6534e904A2416fC0b02894F1782c65B1c5" << '\n';
    cout << stdoutMsg.rdbuf();
    cout.flush();
}

template<typename Options_type>
bool isOptionsSet(const Options_type& options)
{
    return
        options.ntSections ||
        options.exportHeader ||
        options.importHeaders ||
        options.boundHeaders ||
        options.tlsHeader ||
        options.fixupHeaders ||
        options.loadConfigHeader
        ;
}

bool isProgramOptionsSet(const ProgramOptions& options)
{
    return
        options.formatOptions.rawOptions.dosHeader ||
        options.formatOptions.rawOptions.ntHeaders ||
        isOptionsSet(options.formatOptions.rawOptions) ||
        isOptionsSet(options.formatOptions.detailedOptions)
        ;
}

po::variables_map parseCmdline(int argc, char * argv[])
{
    po::positional_options_description inFile;
    inFile.add("input-file", 1);

    po::options_description allOptions("All options");
    allOptions.add(declareBasicOptions()).
        add(declareRawOptions()).
        add(declareDetailedOptions()).
        add(declareHiddenOptions());

    po::variables_map result;

    po::store(po::command_line_parser(argc, argv).
        extra_parser(atResponseFileParser).
        options(allOptions).positional(inFile).run(), result);
    po::notify(result);

    return result;
}

const po::variables_map parseResponseFile(po::variables_map& previousParseResult)
{
    using namespace std;

    po::variables_map result = previousParseResult;

    if (previousParseResult.count("response-file")) {
        po::positional_options_description inFile;
        inFile.add( "input-file", 1 );

        po::options_description allOptions("All options");
        allOptions.add(declareBasicOptions()).
            add(declareRawOptions()).
            add(declareDetailedOptions()).
            add(declareHiddenOptions());

        string filename = previousParseResult["response-file"].as<string>();
        vector<string> args;
        readResponseFile(filename.c_str(), &args);
        po::store(po::command_line_parser(args).
            options(allOptions).positional(inFile).run(), result);
    }

    return result;
}

void setDefaultRawOptions(RawOptions * options)
{
    options->dosHeader = true;
    options->ntHeaders = true;
    options->ntSections = true;
}

void setDefaultDetailedOptions(DetailedOptions * options)
{
    options->ntSections = true;
    options->exportHeader = true;
    options->importHeaders = true;
    options->boundHeaders = true;
    options->tlsHeader = true;
}

void recognizeBasicOptions(const po::variables_map& parseResult, ProgramOptions * options)
{
    if (!parseResult.count("input-file"))
        throw ConfigureException("Input PE-file is not set");

    options->inputFilename = parseResult["input-file"].as<std::string>();
}

void recognizeRawOptions(const po::variables_map& parseResult, RawOptions * options)
{
    changeOptions(options, false);
    if (parseResult.count("raw"))
        setDefaultRawOptions(options);
    else {
        options->dosHeader = parseResult.count("dos-header") > 0;
        options->ntHeaders = parseResult.count("nt-headers") > 0;
        options->ntSections = parseResult.count("sections") > 0;
        options->exportHeader = parseResult.count("export") > 0;
        options->importHeaders = parseResult.count("imports") > 0;
        options->tlsHeader = parseResult.count("tls") > 0;
        options->boundHeaders = parseResult.count("bounds") > 0;
        options->fixupHeaders = parseResult.count("relocations") > 0;
        options->loadConfigHeader = parseResult.count("load-config") > 0;
    }
}

void recognizeDetailedOptions(const po::variables_map& parseResult, DetailedOptions * options)
{
    changeOptions(options, false);
    if( parseResult.count("detailed"))
        setDefaultDetailedOptions(options);
    else {
        options->ntSections = parseResult.count("sections-detailed") > 0;
        options->exportHeader = parseResult.count("export-detailed") > 0;
        options->importHeaders = parseResult.count("imports-detailed") > 0;
        options->tlsHeader = parseResult.count("tls-detailed") > 0;
        options->boundHeaders = parseResult.count("bounds-detailed") > 0;
        options->fixupHeaders = parseResult.count("relocations-detailed") > 0;
    }
}

const ProgramOptions recognizeOptions(const po::variables_map& parseResult)
{
    ProgramOptions result;
    changeProgramOptions(&result, false);

    recognizeBasicOptions(parseResult, &result);
    if (parseResult.count("all"))
        changeProgramOptions(&result, true);
    else {
        recognizeRawOptions(parseResult, &result.formatOptions.rawOptions);
        recognizeDetailedOptions(parseResult, &result.formatOptions.detailedOptions);

        if (!isProgramOptionsSet(result)) {
            setDefaultRawOptions(&result.formatOptions.rawOptions);
            setDefaultDetailedOptions(&result.formatOptions.detailedOptions);
        }
    }
    return result;
}

const ProgramOptions parse(int argc, char * argv[])
{
    try {
        po::variables_map parseResult = parseCmdline(argc, argv);
        parseResult = parseResponseFile(parseResult);

        ProgramOptions emptyOptions;
        changeProgramOptions(&emptyOptions, false);

        if (parseResult.count("version")) {
            printVersionMessage();
            return emptyOptions;
        }
        else if(parseResult.count("donate")) {
            printWallets();
            return emptyOptions;
        }
        else {
            if (!parseResult.count("nologo")) {
                printLogoMessage();
            }

            if (parseResult.count("help")) { 
                printHelpMessage();
                return emptyOptions;
            }
            return recognizeOptions(parseResult);
        }
    }
    catch (const po::error& e) {
        throw ConfigureException(e.what());
    }
}


}} // namespace readpe::cmdline
