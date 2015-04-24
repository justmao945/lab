#ifndef STDX_CMD_HH
#define STDX_CMD_HH

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "process.h"
#include "fdbuf.h"

namespace stdx {

struct Cmd: public Process {
    // abs path, e.g. /bin/ls
    std::string Path;
    // args, e.g. ["ls"]
    std::vector<std::string> Args;
    // name=value
    std::vector<std::string> Env;
    // working dir
    std::string Dir;
    // command read data from this stream
    std::ostream Stdin;
    // command write stdout data to this stream
    std::istream Stdout;
    // command write stderr data to this stream
    std::istream Stderr;

    Cmd(): Stdin(&stdinbuf), Stdout(&stdoutbuf), Stderr(&stderrbuf) { init_fds(); }
    ~Cmd() { close_fds(); }

    bool CloseStdin() { Stdin.flush(); return close_write(fds[0]); }

    bool Run(ProcessState& s) { return Start() && Wait(s); }

protected:
    // read from fds[i][0] and write to fds[i][1]
    // fds[0] for Stdin, 1 for Stdout, 2 for Stderr
    int fds[3][2];
    fdbuf stdinbuf;
    fdbuf stdoutbuf;
    fdbuf stderrbuf;

    void init_fds() {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++) {
                fds[i][j] = -1;
            }
        }
    }

    bool close_fds() {
        bool ok = true;
        for(int i = 0; i < 3; i++) {
            ok &= close_read(fds[i]);
            ok &= close_write(fds[i]);
        }
        return ok;
    }

    bool close(int fd) {
        int err = 0;
        do {
            err = ::close(fd);
        } while(err && errno == EINTR);
        return !err;
    }

    bool close_read(int fd[2]) {
        bool ok = true;
        if(fd[0] >= 0) {
            ok &= close(fd[0]);
            fd[0] = -1;
        }
        return ok;
    }

    bool close_write(int fd[2]) {
        bool ok = true;
        if(fd[1] >= 0) {
            ok &= close(fd[1]);
            fd[1] = -1;
        }
        return ok;
    }

    virtual bool prepare() {
        init_fds();
        for(int i = 0; i < 3; i++) {
            if(pipe(fds[i]) < 0) {
                goto error;
            }
        }
        return stdinbuf.attach(fds[0][1])
         && stdoutbuf.attach(fds[1][0])
         && stderrbuf.attach(fds[2][0]);
    error:
        close_fds();
        return false;
    }

    virtual bool parent() {
        return close_read(fds[0])
        && close_write(fds[1])
        && close_write(fds[2]);
    }

    virtual bool child() {
        if(::dup2(fds[0][0], STDIN_FILENO) != STDIN_FILENO) {
            perror("dup2");
            return false;
        }
        if(::dup2(fds[1][1], STDOUT_FILENO) != STDOUT_FILENO) {
            perror("dup2");
            return false;
        }
        if(::dup2(fds[2][1], STDERR_FILENO) != STDERR_FILENO) {
            perror("dup2");
            return false;
        }
        // duplicated, can close all fds
        if(! close_fds()) {
            perror("close");
            return false;
        }

        if(!Dir.empty() && ::chdir(Dir.c_str()) < 0) {
            perror("chdir");
            return false;
        }

        for(size_t i = 0; i < Env.size(); i++) {
            if(::putenv((char*)(Env[i].c_str())) < 0) {
                perror("putenv");
                return false;
            }
        }

        char** argv = new char*[Args.size() + 1];
        for(size_t i = 0; i < Args.size(); i++) {
            argv[i] = (char*)(Args[i].c_str());
        }
        argv[Args.size()] = 0;
        ::execv(Path.c_str(), argv);
        perror("execv");
        delete[] argv;
        return false;
    }
};


}

#endif
