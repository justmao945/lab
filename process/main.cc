
#include <algorithm>
#include "cmd.h"

using namespace std;
using namespace stdx;

int main() {
    Cmd ls;
    ls.Path = "/bin/ls";
    ls.Args.push_back("ls");
    ls.Dir = "/etc";

    ProcessState ls_st;
    bool ls_ok = ls.Run(ls_st);
    if(! ls_ok) {
        perror("ls");
    }
    cout << "ls: " << (ls_ok ? "OK" : "FAIL") << ", " << ls_st.String() << endl;
    copy(istreambuf_iterator<char>(ls.Stdout),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(cout));

    Cmd cat;
    cat.Path = "/bin/cat";
    cat.Args.push_back("cat");
    ProcessState cat_st;
    bool cat_ok = cat.Start();
    // do write to stdin after started!
    cat.Stdin << "hello world!" << endl;
    cat_ok &= cat.CloseStdin();
    cat_ok &= cat.Wait(cat_st);
    if(! cat_ok) {
        perror("cat");
    }
    cout << "cat: " << (cat_ok ? "OK" : "FAIL") << ", " << cat_st.String() << endl;
    copy(istreambuf_iterator<char>(cat.Stdout),
      istreambuf_iterator<char>(),
      ostreambuf_iterator<char>(cout));

    return 0;
}
