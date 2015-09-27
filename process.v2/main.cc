
#include "cmd.h"

using namespace std;
using namespace stdx;

int main() {
    cout << boolalpha;

    Cmd ls;
    ls.CombineOutput = true;
    ls.Path = "/bin/ls";
    ls.Args.push_back("ls");
    ls.Dir = "/";
    cout << ls.String() << endl;

    Process::State ls_st;
    bool ls_ok = ls.Run(ls_st);
    if(! ls_ok) {
        perror("ls");
    }
    ls.Redirect(1, cout);
    ls.Redirect(2, cout);
    cout << "ls: " << (ls_ok ? "OK" : "FAIL") << ", " << ls_st.String() << endl;

    Cmd cat;
    cat.CombineOutput = true;
    cat.Path = "/bin/cat";
    cat.Args.push_back("cat");
    cout << "\n" << cat.String() << endl;

    Process::State cat_st;
    bool cat_ok = cat.Start();
    // do write to stdin after started!
    cat.Stdin << "hello world!" << endl;
    cat_ok &= cat.CloseStdin();
    cat.Redirect(1, cout);
    cat_ok &= cat.Wait(cat_st);
    if(! cat_ok) {
        perror("cat");
    }
    cout << "cat: " << (cat_ok ? "OK" : "FAIL") << ", " << cat_st.String() << endl;

    Cmd bash;
    bash.Path = "/bin/bash";
    bash.Args.push_back("bash");
    bash.Args.push_back("-c");
    bash.Args.push_back("while true; do x=1; done");
    bash.CombineOutput = true;
    bash.RLim.CPUTime = 1; // limit CPU time to 1s
    cout << "\n" << bash.String() << endl;

    Process::State bash_st;
    bool bash_ok = bash.Start();
    bash.Redirect(1, cout);
    bash_ok &= bash.Wait(bash_st);
    if(! bash_ok) {
        perror("bash");
    }
    cout << "bash: " << (bash_ok ? "OK" : "FAIL")
         << ", Exceed CPU limit: " << bash_st.ExceedCPUTime()
         << ", " << bash_st.String() << endl;

    return 0;
}
