/*************************************************************************
	> File Name: common.c
	> Author:liangyunsnong 
	> Mail: 
	> Created Time: Tue 05 Nov 2019 06:41:03 PM CST
 ************************************************************************/
 #include "common.h" 
 #include "linklist.h"
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

   // int i = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((substr = strstr(line, key)) == NULL) {
            continue;
        }
        if (substr[strlen(key)] == '=') {
            strncpy(ans, &substr[strlen(key) + 1], nread - strlen(key) - 1);
            *(ans + strlen(ans) - 1) = '\0';
            printf("ans = =  %s\n",ans);
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
    printf("sicketfd Created\n");
    return socketfd;
}

int shake_hand(struct sockaddr_in host, char *name) {
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) {
        return -1;
    }
  //  printf("socked  %c\n",sockfd);
  //  printf("success2\n");
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len);
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
  //  printf("success3\n");
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) <0) {
  //    printf("success4\n");
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
           // printf("getsockopt =  %d",getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len_t));
    //        printf("success5\n");
            getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&error, (socklen_t *)&len_t);
       //     printf("error  =  %d\n",error);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
     //       printf("success6\n");
           // printf("socked  ==%c",sockfd);
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->len = 0;
            msg->kind = 0;
            strcpy(msg->name, name);
    //        printf("222222\n");
            printf("%ld",
            send(sockfd,msg,sizeof(Msg),0));
            //send(sockfd,name, strlen(name), 0);
     //       printf("1111111111111111111111111\n");
            memset(name, 0, sizeof(name));
            recv(sockfd,msg, sizeof(Msg), 0);
            strcpy(name, msg->name);
            free(msg);
          //   Msg msg;
          //  if(msg->)
            
    //    printf("success7\n");

        } else {
        //printf("select %d\n",select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) ;
          //  printf("error \n");
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
 //    printf("success8\n");
    return 0;
   // printf("success7\n");
   // printf("success7\n");

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
int list_add(LinkedList p,struct sockaddr_in *list, int *ind){
	while(p->next != NULL){
	memcpy(&list[*ind], &p->next->addr, sizeof(p->next->addr));
    *ind+=1;
     p=p->next;  
    }
    return 1;
}
int accessible(LinkedList head , char *name){
    Node *p = head;
    while(p->next !=NULL){
        if( shake_try(p->addr,name) !=0){
            p->heart += 1;
            if(p->heart == 3){
                dele(head,p);
                D(RED_HL([DELLETE])"%s\n",p->name);
                p->heart = 0;
            }
        }
        p = p->next;
    }
    return 0;

}

/*
int shake(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len,int number) { // 交换用户列表
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) { //创建套接字
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len); //改为非阻塞IO
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, 
                       &error, (socklen_t*)&len_t);
            if (error != 0) { //失败
                close(sockfd); 
                return -1;
            }
            len= 0;
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = number;
            msg->len = list_len;
            strcpy(msg->name, name);
            len = 0;
            ioctl(sockfd, FIONBIO, &len); //改为非阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送2类报文
            send(sockfd, list, sizeof(list), 0); //发送用户列表
            //printf("%d\n", __LINE__);
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}
*/
int shake_try(struct sockaddr_in host, char *name) {
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
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len_t);
            if (error != 0) {
                close(sockfd);
                return -1;
            }
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = 1;
            msg->len = 0;
            strcpy(msg->name, name);
            len = 0;
          //  ioctl(sockfd, FIONBIO, &len);  //改为阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送0类报文
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}


int shake_echg(struct sockaddr_in host, char *name, struct sockaddr_in *list, int list_len) { // 交换用户列表
    int sockfd;
    struct timeval tm;
    fd_set wfds;
    if ((sockfd = socket_()) < 0) { //创建套接字
        return -1;
    }
    unsigned long len = 1;
    ioctl(sockfd, FIONBIO, &len); //改为非阻塞IO
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    tm.tv_sec = 0;
    tm.tv_usec = 10000;
    int error = -1;
    int len_t = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&host, sizeof(host)) < 0) {
        if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR,&error,(socklen_t *)&len_t); 
            if (error != 0) { //失败
                close(sockfd); 
                return -1;
            }
            len= 0;
            Msg *msg = (Msg *)malloc(sizeof(Msg));
            msg->kind = 2;
            msg->len = list_len;
            strcpy(msg->name, name);
            len = 0;
          //  ioctl(sockfd, FIONBIO, &len); //改为非阻塞
            send(sockfd, msg, sizeof(Msg), 0); // 发送2类报文
            send(sockfd, list, sizeof(list), 0); //发送用户列表
            //printf("%d\n", __LINE__);
            free(msg);
        } else {
            close(sockfd);
            return -1;
        }
    } 
    close(sockfd);
    return 0;
}
