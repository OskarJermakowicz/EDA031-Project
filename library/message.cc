#include "message.h"

using namespace std;

void send_code(const shared_ptr <Connection> &conn, int code) {
    conn->write(code);
}

void send_int(const shared_ptr <Connection> &conn, int value) { }

void send_int_parameter(const shared_ptr <Connection> &conn, int param) { }

void send_string_parameter(const shared_ptr <Connection> &conn, const string &param) { }

int recv_byte(const shared_ptr <Connection> &conn) {
    return conn->read();
}

int recv_code(const shared_ptr <Connection> &conn) {
    return recv_byte(conn);
}

int recv_int(const shared_ptr <Connection> &conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int recv_int_parameter(const shared_ptr <Connection> &conn) { }

string recv_string_parameter(const shared_ptr <Connection> &conn) { }