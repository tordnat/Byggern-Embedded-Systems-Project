#pragma once
#include <string.h>

void serial_init(std::string port);
int serial_send(std::string msg);