#include "../library/server.h"
#include "../library/connection.h"
#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"


#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

void handle_request(const shared_ptr <Connection> &conn) {
    int command = recv_code(conn);
    cout << command << endl;
    switch (command) {
        case Protocol::COM_LIST_NG:
            break;
        case Protocol::COM_CREATE_NG:
            break;
        case Protocol::COM_DELETE_NG:
            break;
        case Protocol::COM_LIST_ART:
            break;
        case Protocol::COM_CREATE_ART:
            break;
        case Protocol::COM_DELETE_ART:
            break;
        case Protocol::COM_GET_ART:
            break;
        case Protocol::COM_END:
            break;
        default:
            break;
    }

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = stoi(argv[1]);
    } catch (exception &e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }

    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }

    while (true) {
        auto conn = server.waitForActivity();
        if (conn != nullptr) {
            try {
                handle_request(conn);
            } catch (ConnectionClosedException &) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
}
