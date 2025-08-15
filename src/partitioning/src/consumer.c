#include "../include/consumer.h"
#include "../../logger/include/logger.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *consume_messages(void *args) {
    ConsumerArgs *cargs = (ConsumerArgs *)args;
    Broker *broker = cargs->broker;
    Topic *topic = get_topic(broker, cargs->topic_name);

    if (!topic) {
        log_message(LOG_LEVEL_CONSUMER, "Thread %d: ERROR: Topic '%s' not found.", cargs->thread_id, cargs->topic_name);
        return NULL;
    }
    
    log_message(LOG_LEVEL_CONSUMER, "Thread %d starting to consume from topic '%s', partition %d.", cargs->thread_id, cargs->topic_name, cargs->partition_id);
    Partition *p = &topic->partitions[cargs->partition_id];
    int last_read = 0;

    while (1) {
        if (last_read < p->count) {
            log_message(LOG_LEVEL_CONSUMER, "Thread %d read message: '[%s]' from partition %d.", cargs->thread_id, p->messages[last_read], cargs->partition_id);
            last_read++;
        }
        sleep(1);
    }
    return NULL;
}