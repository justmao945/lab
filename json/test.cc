#include <cstdio>
#include <fstream>
#include "json.hh"

using namespace std;

int main(int argc, char* argv[]) {
    json::Value v;
    if(argc != 3) {
        cerr << "Usage: app <in file> <out file>" << endl;
        return 1;
    }

    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if(! in || ! out) {
        perror("fopen");
        return 1;
    }
    return v.load(in) && v.save(out) ? 0 : 1;
}
