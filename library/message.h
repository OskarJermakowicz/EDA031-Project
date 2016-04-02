#ifndef EDA031_PROJECT_MESSAGE_H
#define EDA031_PROJECT_MESSAGE_H

#include "connection.h"

#include <memory>

using namespace std;

void send_byte(const shared_ptr <Connection> &conn, int byte);

void send_code(const shared_ptr <Connection> &conn, int code);

void send_int(const shared_ptr <Connection> &conn, int value);

void send_int_parameter(const shared_ptr <Connection> &conn, int param);

void send_string_parameter(const shared_ptr <Connection> &conn, const std::string &param);

int recv_byte(const shared_ptr <Connection> &conn);

int recv_code(const shared_ptr <Connection> &conn);

int recv_int(const shared_ptr <Connection> &conn);

int recv_int_parameter(const shared_ptr <Connection> &conn);

string recv_string_parameter(const shared_ptr <Connection> &conn);

#endif //EDA031_PROJECT_MESSAGE_H
