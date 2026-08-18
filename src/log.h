#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG_ERROR(type, msg) fprintf(stderr, "%s:%d - \e[91m%s Error\e[0m: %s\n", __FILE__, __LINE__, type, msg);

#endif
