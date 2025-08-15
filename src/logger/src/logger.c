#include "../include/logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

// ANSI color codes
#define COLOR_RESET   "\x1B[0m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

void log_message(LogLevel level, const char *format, ...) {
    time_t now = time(NULL);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    const char *level_str;
    const char *color_code;

    switch (level) {
        case LOG_LEVEL_BROKER:
            level_str = "BROKER";
            color_code = COLOR_YELLOW;
            break;
        case LOG_LEVEL_PRODUCER:
            level_str = "PRODUCER";
            color_code = COLOR_GREEN;
            break;
        case LOG_LEVEL_CONSUMER:
            level_str = "CONSUMER";
            color_code = COLOR_BLUE;
            break;
        default:
            level_str = "SYSTEM";
            color_code = COLOR_RESET;
            break;
    }

    printf("%s[%s] [%s]: ", color_code, time_buf, level_str);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("%s\n", COLOR_RESET);
    fflush(stdout);
}