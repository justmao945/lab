#include <cstdio>
#include "json.hh"

using namespace std;

int main(int argc, char* argv[]) {
    json::Value v;
    cerr << "status: " << v.load(cin) << "\n" << "output: " << v.save(cout) << endl;
    return 0;
}
