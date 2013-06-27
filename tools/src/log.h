#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>

#include <sys/time.h>

using namespace std;

#define MY_LOG_UNCOND 0
#define MY_LOG_INFO 1
#define MY_LOG_ERR 2

#ifndef CUR_LOG_LEVEL
#define CUR_LOG_LEVEL 0
#endif

#define LOG(level,msg) 

#define LOG_INFO(msg) 
#define LOG_DISK(ofs,msg)
#define LOG_ERR(msg) LOG(2,msg)

#endif
