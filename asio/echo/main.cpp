#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif

void client_test1(int thread_count, char const* host, char const* port,
    size_t block_size, size_t session_count, int timeout);

void server_test1(int thread_count, char const* host, char const* port,
    size_t block_size);


int usage() {
    std::cerr << "Usage: asio_test client1 <host> <port> <threads>"
        " <blocksize> <sessions> <time>\n";
    std::cerr << "Usage: asio_test server1 <address> <port> <threads>"
        " <blocksize>\n";
    return 1;
}


#ifdef _WIN32
void change_limit() {
}
#else
void change_limit() {
    rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) < 0) {
        std::cout << "getrlimit failed, error: " << strerror(errno) << "\n";
        return;
    }
    if (rlim.rlim_cur < rlim.rlim_max) {
        auto old_cur = rlim.rlim_cur;
        rlim.rlim_cur = rlim.rlim_max;
        if (setrlimit(RLIMIT_NOFILE, &rlim) < 0) {
            std::cout << "setrlimit failed, error: " << strerror(errno) <<
                " " << std::to_string(old_cur) + " to " <<
                std::to_string(rlim.rlim_max) << "\n";
        } else {
            std::cout << "setrlimit success: " << std::to_string(old_cur) <<
                " to " << std::to_string(rlim.rlim_max) << "\n";
        }
    }
}
#endif

int main(int argc, char* argv[])
{
    change_limit();

    using namespace std; // For atoi.

    if (argc < 2)
        return usage();

    if (!strcmp(argv[1], "client1") && argc != 8)
        return usage();
    if (!strcmp(argv[1], "server1") && argc != 6)
        return usage();

   if (!strcmp(argv[1], "client1")) {
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        size_t block_size = atoi(argv[5]);
        size_t session_count = atoi(argv[6]);
        int timeout = atoi(argv[7]);
        client_test1(thread_count, host, port, block_size, session_count, timeout);
    } else if (!strcmp(argv[1], "server1")) {
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        size_t block_size = atoi(argv[5]);
        server_test1(thread_count, host, port, block_size);
    } else {
        return usage();
    }

    return 0;
}