#include "../library/connection.h"
#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"
#include "dumb_storage.h"
#include "storage_exceptions.h"
#include "newsserver.h"
#include "../library/protocolviolationexception.h"


#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;


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
                throw ProtocolViolationException();
        }

    } catch (ProtocolViolationException &) {
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
    consume_code(conn, Protocol::COM_END);

    send_code(conn, Protocol::ANS_LIST_NG);

    vector <pair<int, string >> ngs = db.list_ng();
    send_int_parameter(conn, ngs.size());
    for (pair<int, string> p: ngs) {
        send_int_parameter(conn, p.first);
        send_string_parameter(conn, p.second);

    }

    send_code(conn, Protocol::ANS_END);
}

void NewsServer::create_ng(const shared_ptr <Connection> &conn) {
    string ng = recv_string_parameter(conn);
    consume_code(conn, Protocol::COM_END);
    send_code(conn, Protocol::ANS_CREATE_NG);
    try {
        db.create_ng(ng);
        send_code(conn, Protocol::ANS_ACK);
    } catch (NGAlreadyExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_ALREADY_EXISTS);
    }
    send_code(conn, Protocol::ANS_END);
}

void NewsServer::delete_ng(const shared_ptr <Connection> &conn) {
    int ng = recv_int_parameter(conn);
    consume_code(conn, Protocol::COM_END);
    send_code(conn, Protocol::ANS_DELETE_NG);
    try {
        db.delete_ng(ng);
        send_code(conn, Protocol::ANS_ACK);
    } catch (NGDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    send_code(conn, Protocol::ANS_END);
}

void NewsServer::list_art(const shared_ptr <Connection> &conn) {
    int ng = recv_int_parameter(conn);
    consume_code(conn, Protocol::COM_END);
    send_code(conn, Protocol::ANS_LIST_ART);
    try {
        vector <pair<int, string >> arts = db.list_art(ng);
        send_code(conn, Protocol::ANS_ACK);
        send_int_parameter(conn, arts.size());
        for (pair<int, string> p: arts) {
            send_int_parameter(conn, p.first);
            send_string_parameter(conn, p.second);

        }
    } catch (NGDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    send_code(conn, Protocol::ANS_END);
}

void NewsServer::create_art(const shared_ptr <Connection> &conn) {
    int ng = recv_int_parameter(conn);
    string title = recv_string_parameter(conn);
    string author = recv_string_parameter(conn);
    string text = recv_string_parameter(conn);
    consume_code(conn, Protocol::COM_END);

    send_code(conn, Protocol::ANS_CREATE_ART);
    try {
        db.create_art(ng, author, title, text);
        send_code(conn, Protocol::ANS_ACK);
    } catch (NGDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    send_code(conn, Protocol::ANS_END);
}

void NewsServer::delete_art(const shared_ptr <Connection> &conn) {
    int ng = recv_int_parameter(conn);
    int art = recv_int_parameter(conn);
    consume_code(conn, Protocol::COM_END);

    send_code(conn, Protocol::ANS_DELETE_ART);
    try {
        db.delete_art(ng, art);
        send_code(conn, Protocol::ANS_ACK);
    } catch (NGDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (ARTDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    send_code(conn, Protocol::ANS_END);
}

void NewsServer::get_art(const shared_ptr <Connection> &conn) {
    int ng = recv_int_parameter(conn);
    int art = recv_int_parameter(conn);
    consume_code(conn, Protocol::COM_END);

    send_code(conn, Protocol::ANS_GET_ART);
    try {
        article a = db.get_art(ng, art);
        send_code(conn, Protocol::ANS_ACK);
        send_string_parameter(conn, a.title);
        send_string_parameter(conn, a.author);
        send_string_parameter(conn, a.text);
    } catch (NGDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_NG_DOES_NOT_EXIST);
    } catch (ARTDoesNotExistsException &) {
        send_code(conn, Protocol::ANS_NAK);
        send_code(conn, Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    send_code(conn, Protocol::ANS_END);
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
