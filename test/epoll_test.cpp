#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstring>

void set_nonblocking(int fd) {
    int old = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old | O_NONBLOCK);
}

int main() {
    int epfd = epoll_create(1);
    int pipefd[2];
    pipe(pipefd);  // pipefd[0] 读, pipefd[1] 写

    // 写入一些数据
    write(pipefd[1], "hello world", 11);

    set_nonblocking(pipefd[0]); // ET模式必须非阻塞

    struct epoll_event ev, events[10];
    ev.data.fd = pipefd[0];

    // 切换此行可测试 LT（无 EPOLLET） vs ET（加 EPOLLET）
    ev.events = EPOLLIN | EPOLLET;  // ← 这里换成 EPOLLIN 试 LT 模式

    epoll_ctl(epfd, EPOLL_CTL_ADD, pipefd[0], &ev);

    std::cout << "wait for events...\n";

    while (true) {
        int n = epoll_wait(epfd, events, 10, -1);
        for (int i = 0; i < n; ++i) {
            if (events[i].events & EPOLLIN) {
                std::cout << "EPOLLIN triggered!\n";

                char buf[5];
                int cnt;
                // 多次读取，模拟缓冲区未清空
                while ((cnt = read(pipefd[0], buf, sizeof(buf) - 1)) > 0) {
                    buf[cnt] = '\0';
                    std::cout << "read: " << buf << std::endl;
                }
                std::cout << "--- read done ---\n";
            }
        }
        sleep(1);
    }

    return 0;
}
