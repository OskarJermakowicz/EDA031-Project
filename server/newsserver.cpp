#include "../library/connection.h"
#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"
#include "dumb_storage.h"
#include "storage_exceptions.h"
#include "newsserver.h"


#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

struct ProtocolException {
};


void NewsServer::handle_request(const shared_ptr <Connection> &conn) {
    int command = recv_code(conn);
    try {
        switch (command) {
            case Protocol::COM_LIST_NG:
                list_ng(conn);
                break;
            case Protocol::COM_CREATE_NG:
                create_ng(conn);
                break;
            case Protocol::COM_DELETE_NG:
                delete_ng(conn);
                break;
            case Protocol::COM_LIST_ART:
                list_art(conn);
                break;
            case Protocol::COM_CREATE_ART:
                create_art(conn);
                break;
            case Protocol::COM_DELETE_ART:
                delete_art(conn);
                break;
            case Protocol::COM_GET_ART:
                get_art(conn);
                break;
            default:
                throw ProtocolException();
        }

    } catch (ProtocolException &) {
        deregisterConnection(conn);
        cout << "Dropped client" << endl;

    }

}

void NewsServer::run() {

    while (true) {
        auto conn = waitForActivity();
        if (conn != nullptr) {
            try {
                handle_request(conn);
            } catch (ConnectionClosedException &) {
                deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
}


void NewsServer::list_ng(const shared_ptr <Connection> &conn) {
    db.list_ng();
    cout << "List NGs" << endl;
}

void NewsServer::create_ng(const shared_ptr <Connection> &conn) {
    cout << "Create NG" << endl;
}

void NewsServer::delete_ng(const shared_ptr <Connection> &conn) {
    cout << "Delete NG" << endl;
}

void NewsServer::list_art(const shared_ptr <Connection> &conn) {
    cout << "List ART" << endl;
}

void NewsServer::create_art(const shared_ptr <Connection> &conn) {
    cout << "Create ART" << endl;
}

void NewsServer::delete_art(const shared_ptr <Connection> &conn) {
    cout << "Delete ART" << endl;
}

void NewsServer::get_art(const shared_ptr <Connection> &conn) {
    cout << "Get ART" << endl;
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

    NewsServer server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }
    server.run();

}
