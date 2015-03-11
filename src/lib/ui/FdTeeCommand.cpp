#include "FdTeeCommand.hpp"

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


namespace po = boost::program_options;
namespace {
    // based on size of linux pipe buffer (2^16 since linux 2.6.11)
    const size_t BUFSZ = 1 << 16;

    ssize_t safe_read(int fd, void* buf, size_t count) {
        ssize_t rv = 0;
        while ((rv = read(fd, buf, count)) == -1 && errno == EINTR)
            ;
        return rv;
    }

    ssize_t safe_write(int fd, void const* buf, size_t count) {
        ssize_t rv = 0;
        while ((rv = write(fd, buf, count)) == -1 && errno == EINTR)
            ;
        return rv;
    }

    ssize_t safe_write_all(int fd, char const* buf, size_t count) {
        ssize_t rv = 0;
        ssize_t remain = count;
        char const* p = buf;
        while (remain > 0 && (rv = safe_write(fd, p, remain)) > 0) {
            remain -= rv;
            p += rv;
        }
        return rv < 0 ? rv : (count - remain);
    }
}

void FdTeeCommand::configureOptions() {
    opts_.add_options()
        ("input-fd,i",
            po::value<>(&in_fd_)->required(),
            "input file descriptor number (first positional arg)")

        ("output-fd,o",
            po::value<>(&out_fds_)->required(),
            "output file descriptor numbers (pos args 2+, can be specified many times)")
        ;

    pos_opts_.add("input-fd", 1);
    pos_opts_.add("output-fd", -1);
}

void FdTeeCommand::exec() {
    std::size_t n_out_fds = out_fds_.size();

    ssize_t bytes_in;
    char buf[BUFSZ];
    while ((bytes_in = safe_read(in_fd_, buf, BUFSZ)) > 0) {

        for (std::size_t i = 0; i < n_out_fds; ++i) {
            int bytes_out;
            if ((bytes_out = safe_write_all(out_fds_[i], buf, bytes_in)) != bytes_in) {
                std::exit(10 + i);
            }
        }
    }

    if (bytes_in == -1)
        std::exit(1);
}
