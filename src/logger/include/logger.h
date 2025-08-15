#pragma once

// Enum to define different log levels and their associated colors
typedef enum {
    LOG_LEVEL_SYSTEM,  // White
    LOG_LEVEL_BROKER,  // Yellow
    LOG_LEVEL_PRODUCER,// Green
    LOG_LEVEL_CONSUMER // Blue
} LogLevel;

void log_message(LogLevel level, const char *format, ...);