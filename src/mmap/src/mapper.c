#include "../include/mapper.h"
#include "../../compression/include/compressor.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// A robust helper function that loops until all requested bytes are read.
ssize_t read_fully(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t result = read(fd, (char *)buf + bytes_read, count - bytes_read);
        if (result < 0) {
            perror("read error");
            return -1; // An actual error occurred
        }
        if (result == 0) {
            break; // End of file reached prematurely
        }
        bytes_read += result;
    }
    return bytes_read;
}

void read_log_with_mmap(char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open log file: %s\n", filepath);
        perror("open");
        return;
    }

    printf("Reading decompressed messages from: %s\n", filepath);
    int message_count = 0;

    while (1) {
        uint32_t compressed_size;
        ssize_t header_bytes_read = read_fully(fd, &compressed_size, sizeof(compressed_size));

        if (header_bytes_read == 0) {
            // This is a clean end-of-file. We've read all available messages.
            break;
        }

        if (header_bytes_read < sizeof(compressed_size)) {
            fprintf(stderr, "Error: Reached end of file while reading header for message %d. Log file is likely corrupt or truncated.\n", message_count + 1);
            break;
        }
        
        // We have a size, now read the payload.
        Bytef *compressed_buffer = malloc(compressed_size);
        if (!compressed_buffer) {
            perror("malloc for compressed_buffer failed");
            break;
        }

        ssize_t payload_bytes_read = read_fully(fd, compressed_buffer, compressed_size);
        if (payload_bytes_read < compressed_size) {
            fprintf(stderr, "Error: Expected %u bytes for message %d payload, but only read %zd. File is truncated.\n", compressed_size, message_count + 1, payload_bytes_read);
            free(compressed_buffer);
            break;
        }
        
        // Decompression
        uLongf decompressed_size = compressed_size * 10; // A safe starting buffer size
        Bytef *decompressed_buffer = malloc(decompressed_size);
        if (!decompressed_buffer) {
            perror("malloc for decompressed_buffer failed");
            free(compressed_buffer);
            break;
        }

        if (decompress_message(decompressed_buffer, &decompressed_size, compressed_buffer, compressed_size) < 0) {
            fprintf(stderr, "Error: Failed to decompress message %d.\n", message_count + 1);
            free(decompressed_buffer);
            free(compressed_buffer);
            continue; // Skip corrupted message
        }
        
        fwrite(decompressed_buffer, 1, decompressed_size, stdout);
        printf("\n");

        free(decompressed_buffer);
        free(compressed_buffer);
        message_count++;
    }

    printf("Finished reading. Total messages found: %d.\n", message_count);
    close(fd);
}