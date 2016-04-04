#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"
#include "newclient.h"

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
    shared_ptr <Connection> conn(new Connection(addr, port));
    if (!conn->isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    cout << "Connected to " << addr << ":" << port << endl;
    regex grammar(LINE, std::regex::icase);


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


void NewsClient::handle_command(string line) {

    smatch m;
    regex_match(line, m, grammar);

    if (m.size() == 0) {
        cout << "Unrecognized command, please try again." << endl;
        return;
    }

    string command = m[0];
    transform(command.begin(), command.end(), command.begin(), ::tolower);
    cout << command << endl;

    if (command == "help") {
        cout << help_text << endl;
    }
    else if (command == "list") {
        if (m.size() > 1) {
            send_int(conn, Protocol::COM_LIST_ART);
        }
        else {
            send_int(conn, Protocol::COM_LIST_NG);
        }
    }
    else if (command == "create") {
        if (m.size() > 2) {
            send_int(conn, Protocol::COM_CREATE_ART);
        }
        else {
            send_int(conn, Protocol::COM_CREATE_NG);
        }
    }
    else if (command == "delete") {
        if (m.size() > 2) {
            send_int(conn, Protocol::COM_DELETE_ART);
        }
        else {
            send_int(conn, Protocol::COM_DELETE_NG);
        }
    }
    else if (command == "get") {
        send_int(conn, Protocol::COM_GET_ART);
    }

    for (unsigned int i = 1; i < m.size(); ++i) {
        if (is_number(m[i])) {
            send_int_parameter(conn, stoi(m[i]));
        }
        else {
            send_string_parameter(conn, m[i]);
        }
    }
}

void NewsClient::list_ng(int ng) { }

void NewsClient::create_ng(std::string ng) { }

void NewsClient::delete_ng(int ng) { }

void NewsClient::list_art(int ng) { }

void NewsClient::create_art(int ng, std::string title, std::string author, std::string text) { }

void NewsClient::delete_art(int ng, int art) { }

void NewsClient::get_art(int ng, int art) { }
