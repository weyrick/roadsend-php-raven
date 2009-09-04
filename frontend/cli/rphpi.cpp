
#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>
#include <llvm/System/Path.h>

#include "rphp/pConfig.h"
#include "rphp/runtime/pRuntimeError.h"
#include "rphp/JIT/pCachedJIT.h"

using namespace llvm;
using namespace rphp;

void rphpVersion(void) {
    // TODO get version info from runtime
    std::cout << "Roadsend PHP JIT" << std::endl;
}

void consoleNoticeHandler(pUInt level, pMsgString msg) {
    std::cerr << "]] " << msg << std::endl;
}

int main( int argc, char* argv[] )
{

    // command line options
    cl::opt<std::string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
    cl::opt<bool> iSF ("f", cl::desc("Execute source file immediately"));
    cl::opt<int> verbosity ("v", cl::desc("Verbosity level (0=Silent/1=Info/2=Full/3+=Debug)"));
    cl::opt<std::string> encoding ("encoding",cl::desc("Character encoding of the source file"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    // default encoding
    if (encoding.empty())
        encoding = "ASCII";

    assert(!inputFile.empty() && "empty input file");

    pConfig* config = new pConfig();
    // TODO: read php.ini type file, do command line options

    pSourceFileDesc inFile = boost::make_tuple(inputFile, encoding);

    pCachedJIT engine(config);

    try {
        if (verbosity > 0)
            engine.setVerbosity(E_ALL);
        engine.cacheAndJITFileOnDisk(inFile);
    }
    catch (pRuntimeError& e) {
        // runtime errors go to output buffer by default, so we don't display here
        delete config;
        return 1;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete config;
        return 1;
    }

    // success
    delete config;
    return 0;

}
