#ifndef STDX_PROCESS_H
#define STDX_PROCESS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

namespace stdx {

struct Process {
    // Resource limit
    struct RLimit {
        ::rlim_t CPUTime; // in seconds, excluding idle time

        RLimit(): CPUTime(RLIM_INFINITY) {}
    };

    // Running state
    struct State {
        // from syscall wait
        int Status;
        // from wait4
        ::rusage Usage;
        // copied from the process waited
        RLimit RLim;

        State(): Status(0) { ::memset(&Usage, 0, sizeof(Usage)); }

        bool Exited() const { return WIFEXITED(Status); }
        bool Signalled() const { return WIFSIGNALED(Status); }
        bool Success() const { return Exitcode() == EXIT_SUCCESS; }
        long SystemTime() const { return ms(Usage.ru_stime); } // in milliseconds
        long UserTime() const { return ms(Usage.ru_utime); } // in milliseconds
        long MaxRSS() const { return Usage.ru_maxrss; } // in KB

        int Exitcode() const {
            if(Exited()) {
                return WEXITSTATUS(Status);
            } else if(Signalled()) {
                return 128 + WTERMSIG(Status);
            } else {
                assert(0 && "invalid status");
                return -1;
            }
        }
        int Signal() const {
            assert(Signalled());
            return WTERMSIG(Status);
        }

        bool ExceedCPUTime() const {
            long time = SystemTime() + UserTime();
            return (Signalled() && Signal() == SIGXCPU) ||
                (RLim.CPUTime != RLIM_INFINITY && time >= RLim.CPUTime * 1000);
        }

        std::string String() const {
            std::stringstream ss;
            ss << "{Exitcode: " << Exitcode() << ", "
                << "UserTime: " << UserTime() << "ms, "
                << "SysTime: " << SystemTime() << "ms, "
                << "MaxRSS: " << MaxRSS() << "K}";
            return ss.str();
        }

    private:
        long ms(::timeval t) const { return t.tv_sec * 1000 + t.tv_usec / 1000; }
    };
    

    // Process members
    ::pid_t Pid;
    RLimit RLim;

    Process(): Pid(-1) {}
    virtual ~Process() {}

    bool Start() {
        if(! prepare()) {
            return false;
        }

        Pid = ::fork();
        if(Pid < 0) {
            return false;
        } else if(Pid > 0) {
            return parent();
        } else {
            // limit cpu usage
            if(RLim.CPUTime != RLIM_INFINITY) {
                ::rlimit cpulim;
                if(::getrlimit(RLIMIT_CPU, &cpulim) < 0) {
                    ::perror("getrlimit");
                }
                cpulim.rlim_cur = RLim.CPUTime;
                if(::setrlimit(RLIMIT_CPU, &cpulim) < 0) {
                    ::perror("setrlimit");
                }
            }
            
            // run
            if(child()) {
                ::exit(EXIT_SUCCESS);
            } else {
                ::exit(EXIT_FAILURE);
            }
        }
    }

    bool Kill(int sig) {
        assert(Pid != -1);
        return ::kill(Pid, sig) >= 0;
    }

    bool Wait(State& s) {
        assert(Pid != -1);
        pid_t pid = -1;
        s.RLim = RLim;
        do {
            pid = ::wait4(Pid, &s.Status, 0, &s.Usage);
        } while (pid < 0 && errno == EINTR);
        
        return pid == Pid;
    }

protected:
    virtual bool prepare() = 0;
    virtual bool parent() = 0;
    virtual bool child() = 0;
};


}
#endif
// vim: set ft=cpp :
