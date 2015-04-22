#ifndef STDX_CMD_HH
#define STDX_CMD_HH

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "process.h"

namespace stdx {

struct CmdProcess: public Process {
protected:
    virtual bool prepare() {
    }

    virtual bool parent() {
    }

    virtual bool child() {
    }
};

struct Cmd {
    std::string Path;
    std::vector<std::string> Args;
    std::map<std::string,std::string> Env;
    std::string Dir;
    std::istream *Stdin;
    std::ostream *Stdout;
    std::ostream *Stderr;
    CmdProcess Process;
    ProcessState State;

    bool Run() {
        return Start() && Wait();
    }

    bool Start() {
    }

    bool Wait() {
    }
};


}

#endif
