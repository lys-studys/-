/*************************************************************************
	> File Name: head.h
	> Author:akizuki 
	> Mail:renboyu2333@gmail.com 
	> Created Time: Tue 05 Nov 2019 06:34:23 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>	
#include <complex.h> 
#include <ctype.h>
#include <errno.h>	
#include <fenv.h> 
#include <float.h>	
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>	
#include <locale.h>	
#include <math.h>	
#include <setjmp.h>	
#include <signal.h>	
#include <stdalign.h>
#include <stdarg.h>	
#include <stdatomic.h>
#include <stdbool.h> 
#include <stddef.h>	
#include <stdint.h> 
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>	
#include <tgmath.h>
#include <sys/ioctl.h>
#include <pthread.h>
typedef struct Node {
    struct sockaddr_in addr;
    char name[20];
    struct Node *next;
} Node, *LinkedList;

#ifdef _DEBUG
#define _DBG(fmt, args...) printf(fmt, ##args)
#else
#define _DBG(fmt, args...)
#endif

#endif
