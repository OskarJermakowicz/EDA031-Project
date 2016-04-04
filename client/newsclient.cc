#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"
#include "newsclient.h"

#include <iostream>

using namespace std;


bool is_number(const string &s) {
    return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = stoi(argv[2]);
    } catch (exception &e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }
    NewsClient nc(argv[1], port);
    nc.run();
}

NewsClient::NewsClient(const char *addr, int port) {
    conn = shared_ptr<Connection>(new Connection(addr, port));
    if (!conn->isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    cout << "Connected to " << addr << ":" << port << endl;
}

void NewsClient::run() {
    cout << "Type 'help' for a list of available commands." << endl;
    string line;
    while (getline(cin, line)) {
        try {
            handle_command(line);
        }
        catch (ConnectionClosedException &) {
            cout << "No reply from server. Exiting." << endl;
            exit(1);
        }
    }
}

vector <string> NewsClient::parse_line(string line) {
    smatch m;
    regex_match(line, m, regex(LINE, regex::icase | regex::ECMAScript));
    vector <string> tokens;
    if (!m.empty()) {
        for (unsigned int i = 1; i < m.size(); ++i) {
            if (m[i].length()) {
                tokens.push_back(m[i]);
            }
        }
        transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::tolower);
    }
    return tokens;
}

void NewsClient::handle_command(string line) {
    vector <string> tokens = parse_line(line);
    if (tokens.empty()) {
        cout << "Unrecognized command, please try again." << endl;
    } else if (tokens[0] == "help") {
        cout << help_text << endl;
    } else if (tokens[0] == "list") {
        if (tokens.size() == 1) list_ng();
        if (tokens.size() == 2) list_art(stoi(tokens[1]));
    } else if (tokens[0] == "create") {
        if (tokens.size() == 2) create_ng(tokens[1]);
        if (tokens.size() == 5) create_art(stoi(tokens[1]), tokens[2], tokens[3], tokens[4]);
    } else if (tokens[0] == "delete") {
        if (tokens.size() == 2) delete_ng(stoi(tokens[1]));
        if (tokens.size() == 3) delete_art(stoi(tokens[1]), stoi(tokens[2]));
    } else if (tokens[0] == "get") {
        get_art(stoi(tokens[1]), stoi(tokens[2]));
    }
}

void NewsClient::list_ng() {
    send_code(conn, Protocol::COM_LIST_NG);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::create_ng(std::string ng) {
    send_code(conn, Protocol::COM_CREATE_NG);
    send_string_parameter(conn, ng);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::delete_ng(int ng) {
    send_code(conn, Protocol::COM_DELETE_NG);
    send_int_parameter(conn, ng);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::list_art(int ng) {
    send_code(conn, Protocol::COM_LIST_ART);
    send_int_parameter(conn, ng);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::create_art(int ng, std::string title, std::string author, std::string text) {
    send_code(conn, Protocol::COM_CREATE_ART);
    send_int_parameter(conn, ng);
    send_string_parameter(conn, title);
    send_string_parameter(conn, author);
    send_string_parameter(conn, text);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::delete_art(int ng, int art) {
    send_code(conn, Protocol::COM_DELETE_ART);
    send_int_parameter(conn, ng);
    send_int_parameter(conn, art);
    send_code(conn, Protocol::COM_END);
}

void NewsClient::get_art(int ng, int art) {
    send_code(conn, Protocol::COM_GET_ART);
    send_code(conn, Protocol::COM_END);
}
