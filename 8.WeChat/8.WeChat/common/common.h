/*************************************************************************
	> File Name: common.h
	> Author:akizuki 
	> Mail:renboyu2333@gmail.com 
	> Created Time: Tue 05 Nov 2019 06:35:03 PM CST
 ************************************************************************/
#include "head.h"
#ifndef _COMMON_H
#define _COMMON_H

char *get_value(char *path, char *key, char *const ans);

int socket_create(int port);

int socket_();

int shake_hand(struct sockaddr_in host, char *name);

int find_min(int *sum, int ins);

bool check_online(LinkedList *head, struct sockaddr_in cilent, int ins);

#endif
