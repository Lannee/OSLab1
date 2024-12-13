#pragma once 

#include <unistd.h>
#include <cassert>

namespace oslab1::shell {

class Pipe {
public:
    Pipe() { assert(pipe(fds) != -1); }

    auto get_read_end()  const { return fds[0]; }
    auto get_write_end() const { return fds[1]; }

    auto close_read_end()  { return close(fds[0]); }
    auto close_write_end() { return close(fds[1]); }

private:
    int fds[2];
};

} // oslab1::shell
