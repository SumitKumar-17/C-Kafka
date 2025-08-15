#include "../include/producer.h"
#include "../../logger/include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void produce_message(Broker *broker, char *topic_name, char *message) {
    Topic *topic = get_topic(broker, topic_name);
    if (!topic) {
        log_message(LOG_LEVEL_PRODUCER, "ERROR: Topic '%s' not found.", topic_name);
        return;
    }

    int index = topic->rr_index % topic->partition_count;
    Partition *p = &topic->partitions[index];

    if (p->count < MAX_MESSAGES) {
        p->messages[p->count++] = strdup(message);
        log_message(LOG_LEVEL_PRODUCER, "Message '%s' sent to topic '%s', partition %d.", message, topic_name, index);
        topic->rr_index = (topic->rr_index + 1) % (topic->partition_count);
    } else {
        log_message(LOG_LEVEL_PRODUCER, "ERROR: Partition %d of topic '%s' is full.", index, topic_name);
    }
}