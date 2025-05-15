#pragma once

#define LOG(...)    // 日志宏, 打印日志
#define ASSERT(x, ...) { if(!(x)) { LOG(__VA_ARGS__); __debugbreak(); } }   // 断言宏, 若x为假, 则打印日志并中断程序
#define BREAKIF(x) { if((x)) { __debugbreak(); } }   // 若x为真, 则中断程序