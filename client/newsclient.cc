#include "../library/connection.h"
#include "../library/connectionclosedexception.h"
#include "../library/message.h"
#include "../library/protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <regex>

using namespace std;

const string help_text = "Available commands: (parameters within <> are optional and parameters within '' are mandatory)\nlist <number>\t\tLists newsgroups, or articles of newsgroup with id number\ncreate 'name'\t\tCreates a newsgroup with name\ncreate ''\ndelete\nget";

bool is_number(const string& s) {
	return !s.empty() && find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

void send_command(const shared_ptr <Connection> &conn, string line) {

    string INTEGER = "(0|[1-9][0-9]*)";
    string TEXT = "(?:([^\\s\"]+)|\"([^\"]*)\")";
    string WHITESPACE = "(?:\\s+)";

    string HELP = "(help)";
    string LIST = "(list)(?:" + WHITESPACE + INTEGER + ")?";
    string CREATE = "(create)" + WHITESPACE + "(?:" + TEXT + "|" + INTEGER + WHITESPACE + TEXT + WHITESPACE + TEXT +
                    WHITESPACE + TEXT + ")";
    string DELETE = "(delete)" + WHITESPACE + INTEGER + "(?:" + WHITESPACE + INTEGER + ")?";
    string GET = "(get)" + WHITESPACE + INTEGER + WHITESPACE + INTEGER;

    string LINE =
            "^" + WHITESPACE + "?(?:" + LIST + "|" + CREATE + "|" + DELETE + "|" + GET + "|" + HELP + ")" + WHITESPACE +
            "?$";

	smatch m;
	regex_match(line, m, regex(LINE, regex::icase));

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


int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	shared_ptr<Connection> conn(new Connection(argv[1], port));
	if (!conn->isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	cout << "Connected to " << argv[1] << ":" << port << endl;
	cout << "Type 'help' for a list of available commands." << endl;

	string line;
	while (getline(cin, line)) {
		try {
			send_command(conn, line);
		}
		catch (ConnectionClosedException&) {
			cout << "No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

