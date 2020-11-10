/*************************************************************************
	> File Name: wechat.c
	> Author:liangyunsong 
	> Mail: 
	> Created Time: Tue 05 Nov 2019 07:06:19 PM CST
 ************************************************************************/
#include "./common/head.h"
#include "./common/common.h"
#include "./common/linklist.h"
char value[50] = {0};
char name[20] = {0};
int port, ins;
char path[] = "./wechat.conf";

void add_usr(struct sockaddr_in *s, int *sum, LinkedList *linkedlist) {
    if (check_online(linkedlist, *s, ins)) {
        Node *new = (Node *)malloc(sizeof(Node));
        new->addr = *s;
        new->addr.sin_port = htons(port); //端口是对方的，要改
        new->next = NULL;
        new->heart = 0;
        if (shake_hand(*s, name) != 0) {
            return ;
        }
        strcpy(new->name, name);
        int sub = find_min(sum, ins);
        insert(linkedlist[sub], new);
        sum[sub++];
    }
}



void *echg_list(void *arg) { // 交换用户列表
    while (1) {
        sleep(20);
        LinkedList* p = (LinkedList *)arg;
        int sum = 0; 
        for (int i = 0; i < ins; i++) {
            sum += p[i]->len; 
        }
        int ind = 0;
        int list_len = sizeof(struct sockaddr_in) * sum;
        struct sockaddr_in *list = (struct sockaddr_in *)malloc(list_len);
        for (int i = 0; i < ins; i++) {
            list_add(p[i], list, &ind);
        }
        for (int i = 0; i < ins; i++) {
            LinkedList usr = p[i];
            while (usr->next != NULL) {
                shake_echg(usr->next->addr, name, list, list_len);
                usr = usr->next;
            }
        }
        D(YELLOW(EXG LIST)"\n");
    }
    return NULL;
}


void *send_heart(void *arg) { // 发送心跳
    while (1) {
        sleep(1);
        D(YELLOW(HEART)"\n");
        LinkedList* p = (LinkedList *)arg;
        for (int i = 0; i < ins; i++) {
            accessible(p[i], name);
        }
    }
    return NULL;
}//辅助功能如果线程太多，核心不够，还得调度


void *print(void *arg) {
    while (1) {
        LinkedList p = (LinkedList)arg;
        sleep(1);
     //   printf("Sdadsadadada\n");
        D("print:\n");
        while (p->next != NULL) {
            printf("%s->%s\n", inet_ntoa(p->next->addr.sin_addr), p->next->name);
            p = p->next;
        }
    }
    return NULL;
}

int main() {

    printf("程序开始\n");

    get_value(path, "name", name);
    port = atoi(get_value(path, "listenport", value));
    ins = atoi(get_value(path, "ins", value));
//获取信息
    unsigned int s, f;

    s = ntohl(inet_addr(get_value(path, "startip", value)));//点分十进制
    printf("s====%u\n",s);
    f = ntohl(inet_addr(get_value(path, "endip", value)));
    printf("f = %u\n",f);
    LinkedList *linkedlist = (LinkedList *)malloc(sizeof(LinkedList) * ins);
    int *sum = (int *)malloc(ins * sizeof(int));
	D(BLUE_HL([Before init ]"\n"));
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    initaddr.sin_port = htons(port);
    printf("初始化链表\n");
    for (int i = 0; i < ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        p->len = 0;
        strcpy(p->name, "init");
        p->next = NULL;
        linkedlist[i] = p;
    }
    printf("开始握手\n");
    for (unsigned int i = s; i <= f; i++) {
     //   printf("dasdad  1111111111111\n");
        if ((i & 255) == 255 || (i << 24) == 0) continue;
        //不操作 的网段
        initaddr.sin_addr.s_addr = htonl(i);
        char tmp_name[20] = {0};
     //   printf("dasdsdasd\n");
        strcpy(tmp_name,name);
  //   printf("sdadss\n");
 //       printf("握手值 %d\n",shake_hand(initaddr,tmp_name));
        if (shake_hand(initaddr, tmp_name) == 0) {
            printf("shake_hand sucess\n");

            printf("%s->%s\n",name,inet_ntoa(initaddr.sin_addr));
            Node *new = (Node *)malloc(sizeof(Node));
            new->next = NULL;
			new->heart = 0;
            new->addr = initaddr;
            strcpy(new->name, tmp_name);
            int sub = find_min(sum, ins);
            insert(linkedlist[sub], new);
            sum[sub]++;
        }
    }
//printf("adsasdasd \n");

    pthread_t work[ins];
    printf("pthread_t\n");
    for (int i = 0; i < ins; i++) {
        pthread_create(&work[i], NULL, print, (void *)linkedlist[i]);
    }
   
   pthread_t heart;
    pthread_create(&heart, NULL, send_heart, (void *)linkedlist);
 
    pthread_t echg;
    pthread_create(&echg, NULL, echg_list, (void *)linkedlist);
    int server_listen, sockfd;

    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        return 1;
    }

    while (1) {
        struct sockaddr_in cilent;
        socklen_t s_len = sizeof(cilent);
        if ((sockfd=accept(server_listen, (struct sockaddr *)&cilent, &s_len)) < 0) {
            perror("accept()");
            close(sockfd);
            continue;
        }
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        char logname[20] = {0};
        if (select(sockfd + 1, &set, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(sockfd, &set)) {
				Msg *msg = (Msg *)malloc(sizeof(Msg));
    			
                recv(sockfd, msg, sizeof(Msg), 0);
				if(msg -> kind == 1){
					printf("一类型 \n");
					close(sockfd);
					continue ;

				}
				
				else if(msg->kind == 2){
				  D(YELLOW(TYPE2)"\n");
                    int num = msg->len / sizeof(struct sockaddr_in);
                    struct sockaddr_in *s = (struct sockaddr_in*)malloc(msg->len);
                    int now = 0;
                    while (recv(sockfd, &s[now++], sizeof(struct sockaddr_in), 0) != 0); //单个sockaddr_in接收同时计数
                    if (now != num) {
                        close(sockfd);
                        continue;
                    }
                    for (int i = 0; i < num; i++) {
                        add_usr(&s[i], sum, linkedlist);
                        D(YELLOW_HL([LIST EXG]));
                    }
                    close(sockfd);
                    continue;
				}
                 else if(msg->kind == 0){ 
                strcpy(logname, msg->name);
                strcpy(msg->name, name);
                msg->len = 0;
                send(sockfd, msg, sizeof(Msg), 0);
                printf("%s:%s Login\n", logname, inet_ntoa(cilent.sin_addr));
                if (check_online(linkedlist, cilent, ins)) {
                    Node *new = (Node *)malloc(sizeof(Node));
                    new->addr = cilent;
                    new->addr.sin_port = htons(port);
                    new->next = NULL;
                    new->heart = 0;
                    strcpy(new->name, logname);
                    int sub = find_min(sum, ins);
                    insert(linkedlist[sub], new);
                }
                close(sockfd);
             }
                else{
                     continue;
             }
            
            }else {
                close(sockfd);
                continue;
                 }
        } 
        else {
            close(sockfd);
        }
    }
    return 0;
}





