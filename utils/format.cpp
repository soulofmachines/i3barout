//g++ format.cpp --std=c++11 -ljsoncpp -o format
#include <fstream>
#include <getopt.h>
#include <jsoncpp/json/json.h>

using namespace std;

int main (int argc, char *argv[]) {
    Json::Value config;
    Json::Reader reader;
    Json::StyledWriter writer;
    ifstream input_file;
    ofstream output_file;
    string input_path = "", output_path = "";
    if (argc > 1) {
        int cmd;
        while ((cmd = getopt (argc, argv, "i:o:")) != -1) {
            switch (cmd) {
            case 'i':
                input_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            }
        }
    }
    if (!input_path.empty()) {
        input_file.open (input_path);
        if (!input_file.is_open()) {
            cerr << input_path << ": can't open" << endl;
            return -1;
        }
        if (!reader.parse (input_file, config, false)) {
            cerr << input_path << ": " << reader.getFormattedErrorMessages() << endl;
            return 1;
        }
        input_file.close();
	if (output_path.empty()) {
            cout << writer.write(config);
	} else {
	    output_file.open (output_path,ofstream::out);
	    if (!output_file.is_open()) {
                cerr << output_path << ": can't open" << endl;
                return -1;
            }
	    output_file << writer.write(config);
	    output_file.close();
	}
    }
}
