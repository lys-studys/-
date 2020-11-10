/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail:
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
    int heart;
    int len;
} Node, *LinkedList;
/*
typedef struct Msg{
    
    int type;
    int size;
    char name[20];
}Msg;
*/
#ifdef _DEBUG
#define D(fmt, args...) printf(fmt, ##args)
#else
#define D(fmt, args...)
#endif


#define COLOR(a, b) "\033[" #b "m" #a "\033[0m"
#define COLOR_HL(a, b) "\033[1m\033[1m\033[" #b "m" #a "\033[0m"

#define GREEN(a) COLOR(a, 32)
#define RED(a) COLOR(a, 31)
#define BLUE(a) COLOR(a, 34)
#define YELLOW(a) COLOR(a, 33)

#define GREEN_HL(a) COLOR_HL(a, 32)
#define RED_HL(a) COLOR_HL(a, 31)
#define BLUE_HL(a) COLOR_HL(a, 34)
#define YELLOW_HL(a) COLOR_HL(a, 33)

#endif
