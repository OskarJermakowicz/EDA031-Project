#include "message.h"
#include "protocol.h"
#include "protocolviolationexception.h"

using namespace std;

void send_byte(const shared_ptr <Connection> &conn, int byte) {
    conn->write(byte);
}

void send_code(const shared_ptr <Connection> &conn, int code) {
    send_byte(conn, code);
}

void send_int(const shared_ptr <Connection> &conn, int value) {
    conn->write((value >> 24) & 0xFF);
    conn->write((value >> 16) & 0xFF);
    conn->write((value >> 8) & 0xFF);
    conn->write(value & 0xFF);
}

void send_int_parameter(const shared_ptr <Connection> &conn, int param) {
    send_code(conn, Protocol::PAR_NUM);
    send_int(conn, param);
}

void send_string_parameter(const shared_ptr <Connection> &conn, const string &param) {
    send_code(conn, Protocol::PAR_STRING);
    send_int(conn, param.length());
    for (char c : param) {
        conn->write(c);
    }
}

void consume_code(const shared_ptr <Connection> &conn, int code) {
    if (recv_code(conn) != code)
        throw ProtocolViolationException();

}

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

int recv_int_parameter(const shared_ptr <Connection> &conn) {
    consume_code(conn, Protocol::PAR_NUM);
    return recv_int(conn);
}

string recv_string_parameter(const shared_ptr <Connection> &conn) {
    consume_code(conn, Protocol::PAR_STRING);
    int n = recv_int(conn);
    string s;
    char ch;
    for (int i = 0; i != n; ++i) {
        s += ch;
    }
    return s;

}