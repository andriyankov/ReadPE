
/*
{
  "basic" : {
	"logfilesDir" : "./",
	"outputPostfix" : "output",
	"tmpfilesDir" : "./"
  },
  "debug" : {
	"maxSectionsCount" : "20",
	"prngSeedValue" : "2048",
	"tmpfileFmt" : "mk%04X.tmp"
  }
}
*/

#include "std.hpp"


int main( int argc, char* argv[] )
{
    using namespace std;

    if( argc != 2 ) {
        cerr << "Invalid command line" << endl;
        return -1;
    }

    ifstream fileStream( argv[1] );
    if( !fileStream )  {
        cerr << "Open Json-file is failed" << endl;
        return -1;
    }

    try {
        json::Object root;
        json::Reader::Read( root, fileStream );
        
        if( !root.Empty() ) {
            json::Object& basicObj = root["basic"];
            if( !basicObj.Empty() ) {
                json::String outputPostfix(basicObj["outputPostfix"]);
                json::String logfilesDir(basicObj["logfilesDir"]);
                json::String tmpfilesDir(basicObj["tmpfilesDir"]);
                
                cout << "basic-options" << endl;
                cout << "  outputPostfix : " << outputPostfix.Value() << endl;
                cout << "  logfilesDir   : " << logfilesDir.Value() << endl;
                cout << "  tmpfilesDir   : " << tmpfilesDir.Value() << endl;
            }

            json::Object& debugObj = root["debug"];
            if( !debugObj.Empty() ) {
                json::String maxSectionsCount = debugObj["maxSectionsCount"];
                json::String prngSeedValue    = debugObj["prngSeedValue"];
                json::String tmpfileFmt    = debugObj["tmpfileFmt"];

                cout << "debug-options" << endl;
                cout << "  maxSectionsCount : " << maxSectionsCount.Value() << endl;
                cout << "  prngSeedValue    : " << prngSeedValue.Value() << endl;
                cout << "  tmpfileFmt       : " << tmpfileFmt.Value() << endl;
            }
        }
    }
    catch( runtime_error& ) {
        cerr << "Json reading is failed" << endl;
        return -1;
    }
    return 0;
}
