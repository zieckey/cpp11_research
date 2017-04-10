#include <folly/MPMCQueue.h>

int main(int argc, char* argv[])
{
    folly::MPMCQueue<int> queue;
    queue.write(1);
    queue.write(2);
    queue.write(3);
    queue.write(4);

    int v;
    for (int i = 1; i <= 4; i++) {
        queue.read(v);
        assert(v == i);
    }
    return 0;
}