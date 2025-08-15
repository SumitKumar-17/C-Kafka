#pragma once

int connect_to_broker(const char *server_ip, int port);
void send_message_to_broker(int sock, const char *message);
void close_broker_connection(int sock);