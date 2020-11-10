#!/bin/bash 
gcc -D _DEBUG wechat.c ./common/common.c ./common/linklist.c -I ./common/ -o wechat -l pthread
