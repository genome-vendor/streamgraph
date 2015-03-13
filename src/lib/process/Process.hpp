#pragma once

#include "process/ProcessResult.hpp"
#include "utility/FdMapping.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

enum ProcessState {
    eNEW,
    eRUNNING,
    eCOMPLETE
};

class Process : public boost::noncopyable {
public:
    typedef boost::shared_ptr<Process> Ptr;

    static Ptr create(std::string const& name, std::vector<std::string> const& args);

    pid_t start();
    bool finish();

    pid_t pid() const               { return pid_; }
    ProcessState state() const      { return state_; }
    int raw_status() const          { return result_.status; }
    rusage resource_usage() const   { return result_.rsrc; }
    FdMapping& fd_map()             { return fd_map_; }
    std::string const& name() const { return name_; }

    void set_result(ProcessResult const& status);
    ProcessResult const& result() const {
        return result_;
    }

    bool succeeded() const {
        return state_ == eCOMPLETE && result_.status == 0;
    }

    int exit_status() const;
    int exit_signal() const;

    std::string args_string() const;


private:
    Process(std::string const& name, std::vector<std::string> const& args);

private:
    std::string name_;
    std::vector<std::string> args_;

    pid_t pid_;
    ProcessState state_;
    ProcessResult result_;
    FdMapping fd_map_;
};
