/*************************************************************************
	> File Name: linklist.c
	> Author: 
	> Mail: 
	> Created Time: Tue 05 Nov 2019 06:59:40 PM CST
 ************************************************************************/

#include "linklist.h"

int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return 0;

}

int dele(LinkedList head, Node *node) {
    Node *p = head;
    Node *q = head;
    p->len--;
    while (p->next != NULL && p->next != node) {
        q = p;
        p = p->next;
    }
    q->next = p->next;
    free(p);
    return 0;
}

