/*************************************************************************
	> File Name: common.c
	> Author:akizuki 
	> Mail:renboyu2333@gmail.com 
	> Created Time: Tue 05 Nov 2019 06:41:03 PM CST
 ************************************************************************/

#include "common.h"

char *get_value(char *path, char *key, char *ans) {
    FILE *fp = NULL;
    if (key == NULL || ans == NULL || path == NULL) {
        printf("error in argument");
        return NULL;
    }

    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
    }

    char *line = NULL;
    size_t len;
    ssize_t nread;
    char *substr = NULL;

    int i = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) {
            continue;
        }
        if (substr[strlen(key)] == '=') {
            strncpy(ans, &substr[strlen(key) + 1], nread - strlen(key) - 1);
            *(ans + strlen(ans) - 1) = '\0';
            break;
        }
    }
    free(line);
    fclose(fp);
    if (substr == NULL) {
        return NULL;
    }
    return ans;
}

int socket_() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket_()");
        return -1;
    }
    return sockfd;
}

int socket_create(int port) {
    int socketfd;
    struct sockaddr_in sock_addr;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        perror("bind");
        close(socketfd);
        return -1;
    }

    if (listen(socketfd, 20) < 0) {
        perror("listen");
        close(socketfd);
        return -1;
    }

    return socketfd;
}

int shake_hand(struct sockaddr_in host, char *name) {
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) {
        return -1;
    }

    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len);
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 1000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len_t);
            if (error != 0) {
                close(sockfd);
            }
            send(sockfd, name, strlen(name), 0);
            memset(name, 0, sizeof(name));
            recv(sockfd,name, sizeof(name), 0);
        } else {
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;

}

int find_min (int *sum, int ins) {
    int sub = 0;
    for (int i = 0; i < ins; i++) {
        if (*(sum + i) < *(sum + sub)) {
            sub = i;
        }
        return sub;
    }
}

bool check_online (LinkedList *head, struct sockaddr_in client, int ins) {
    for (int i = 0; i < ins; i++) {
        Node *p = head[i];
        while (p->next != NULL) {
            if (p->next->addr.sin_addr.s_addr == client.sin_addr.s_addr) {
                return false;
            }
            p = p->next;
        }
    }
    return true;
}

