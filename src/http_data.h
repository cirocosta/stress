#ifndef STRESS__HTTP_DATA_H
#define STRESS__HTTP_DATA_H

#include <string.h>

const char* stress_http_response = "HTTP/1.1 200 Ok\r\n"
                                   "Content-Length: 2\r\n"
                                   "\r\n"
                                   "OK\r\n";

size_t stress_http_response_len = 42;

const char* stress_http_request = "GET /path HTTP/1.1\r\n"
                                  "Host: localhost\r\n"
                                  "\r\n";

size_t stress_http_request_len = 39;

#endif
