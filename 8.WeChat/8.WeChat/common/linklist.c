/*************************************************************************
	> File Name: linklist.c
	> Author:akizuki 
	> Mail:renboyu2333@gmail.com 
	> Created Time: Tue 05 Nov 2019 06:59:40 PM CST
 ************************************************************************/

#include "linklist.h"

int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }

}

