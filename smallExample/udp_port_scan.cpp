#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

bool ping(const char *ip);
bool scan_udp_port(const char *ip, int port);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("usage: %s [IP] [port]\n", argv[0]);
        exit(0);
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    if (ping(ip)) {
        if (!scan_udp_port(ip, port)) {
            printf("%s, %d ping ok! but udp port unreach\n", ip, port);
            return 0;
        }
    } else {
        printf("%s, ping not reach!\n", ip);
        return 0;
    }
    printf("%s:%d, IP port test OK\n", ip, port);
    return 0;
}

bool ping(const char *ip)
{
    char cmd[32] = {0};
    
    snprintf(cmd, 32, "ping %s -c 1 -w 1", ip);
    FILE *p = popen(cmd, "r");
    if (nullptr == p) {
        printf("popen failed, errno = %d\n", errno);
        return false;
    }

    int n1 = 0, n2 = 0;
    char buf[1024] = {0};
    while (fgets(buf, 1024, p) != nullptr) {
        if (2 == sscanf(buf, "%d packets transmitted, %d received", &n1, &n2)) {
            break;
        }
    }
    pclose(p);
    return n1 !=0 && n2 != 0 && n1 == n2;
}

bool scan_udp_port(const char *ip, int port)
{
    if (port <= 0) {
        printf("port <=0 \n");
        return false;
    }

    struct sockaddr_in server_addr, client_addr;
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd <= 0) {
        printf("create socket failed, errno = %d\n", errno);
        return false;
    }
    bool ret = false;
    do {
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        if (-1 == ret) {
            printf("setsockopt failed, errno = %d\n", errno);
            break;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);

        ret = connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (-1 == ret) {
            printf("connect failed, errno = %d\n", errno);
            break;
        }

        ret = send(fd, "test", strlen("test"), 0);
        if (-1 == ret) {
            printf("send failed, errno = %d\n", errno);
            break;
        }

        char buf[1024] = {0};
        socklen_t len = sizeof(client_addr);

        ret = recvfrom(fd, buf, 1024, 0, (struct sockaddr*)&client_addr, &len);
        if (-1 == ret) {
            if (errno == ECONNREFUSED) {
                //printf("udp port unreachable\n");
                break;
            } else if (errno != EAGAIN) {
                printf("recvfrom failed, errno = %d\n", errno);
                break;
            } else {
                ret = true;
            }
        } else {
            ret = true;
        }
    } while(0);

    close(fd);

    return ret;
}
