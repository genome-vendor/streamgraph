#pragma once

#include "process/Process.hpp"
#include "process/ProcessGroup.hpp"

#include <glog/logging.h>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <stdexcept>

#include <string>
#include <vector>

class ProcessGraph {
public: // types
    typedef std::vector<Process::Ptr> NodeList;
    typedef NodeList::size_type NodeId;

    struct FdSpec {
        NodeId node_id;
        int fd;

        friend bool operator<(FdSpec const& x, FdSpec const& y) {
            if (x.node_id < y.node_id)
                return true;
            if (y.node_id < x.node_id)
                return false;
            return x.fd < y.fd;
        }

        friend bool operator==(FdSpec const& x, FdSpec const& y) {
            return x.node_id == y.node_id && x.fd == y.fd;
        }

        friend bool operator!=(FdSpec const& x, FdSpec const& y) {
            return !(x == y);
        }
    };

    typedef boost::unordered_set<FdSpec> FdSpecSet;
    typedef boost::unordered_map<FdSpec, FdSpecSet> OutPipeMap;
    typedef boost::unordered_map<FdSpec, FdSpec> InPipeMap;

public: // functions
    explicit ProcessGraph(std::vector<std::string> fdtee_cmd = std::vector<std::string>{"streamgraph", "fdtee"});

    void set_fdtee_cmd(std::vector<std::string> cmd) {
        fdtee_cmd_.swap(cmd);
    }

    NodeId add(Process::Ptr const& proc);
    NodeId add(std::string const& name, std::vector<std::string> const& args);
    NodeId size() const;
    Process::Ptr& process(NodeId id);
    Process::Ptr const& process(NodeId id) const;
    void validate_node(NodeId id) const;
    void connect(NodeId src, int src_fd, NodeId dst, int dst_fd);
    void connect_input_file(std::string const& src, NodeId dst, int dst_fd);
    void connect_output_file(NodeId src, int src_fd, std::string const& dst,
        int mode = 0644, bool append = false);
    bool execute();

private:
    void create_pipe(int rwpipe[2]);
    Process::Ptr make_fdtee_cmd(int read_fd, std::size_t n_dst) const;

private: // data
    NodeList nodes_;
    ProcessGroup pgroup_;

    std::vector<int> pipe_fds_;
    OutPipeMap pipes_;
    InPipeMap in_pipes_;

    std::vector<std::string> fdtee_cmd_;
};

inline
std::size_t hash_value(ProcessGraph::FdSpec const& spec) {
    std::size_t seed = boost::hash_value(spec.node_id);
    boost::hash_combine(seed, spec.fd);
    return seed;
}
