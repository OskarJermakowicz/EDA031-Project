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

void send_command(const Connection& conn, const string& line) {
	cmatch m;
	regex_match(line, m, regex("^(?:[:s:]+)?(?:(list)(?:(?:[:s:]+)(0|[1-9][0-9]*))?|(create)(?:[:s:]+)(?:(?:([^[:s:]"] + ) | "([^"] * )")|(0|[1-9][0-9]*)(?:[:s:]+)(?:([^[:s:]"] + ) | "([^"] * )")(?:[:s:]+)(?:([^[:s:]"] + ) | "([^"] * )")(?:[:s:]+)(?:([^[:s:]"] + ) | "([^"] * )"))|(delete)(?:[:s:]+)(0|[1-9][0-9]*)(?:(?:[:s:]+)(0|[1-9][0-9]*))?|(get)(?:[:s:]+)(0|[1-9][0-9]*)(?:[:s:]+)(0|[1-9][0-9]*)|(help))(?:[:s:]+)?$"));

	if (m.size() == 0) {
		cout << "Unrecognized command, please try again." << endl;
		return;
	}

	switch(m[0]) {
	case "help":
		cout << help_text << endl;
		break;
	case "list":
		if (m.size() > 1) {
			send_int(conn, Protocol::COM_LIST_ART);
		}
		else {
			send_int(conn, Protocol::COM_LIST_NG);
		}
		break;
	case "create":
		if (m.size() > 2) {
			send_int(conn, Protocol::COM_CREATE_ART);
		}
		else {
			send_int(conn, Protocol::COM_CREATE_NG);
		}
		break;
	case "delete":
		if (m.size() > 2) {
			send_int(conn, Protocol::COM_DELETE_ART);
		}
		else {
			send_int(conn, Protocol::COM_DELETE_NG);
		}
		break
	case "get":
		send_int(conn, Protocol::COM_GET_ART);
		break;
	}
	for (unsigned int i = 1; i < m.size(); ++i) {
		if (is_number(m[i])) {
			send_int_parameter(conn, m[i]);
		}
		else {
			send_string_parameter(conn, m[i]);
		}
	}
	break;
	}
}

string read_string(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
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
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	cout << "Connected to " << argv[1] << ":" << port << endl;
	cout << "Type 'help' for a list of available commands." << endl;

	string line;
	while (getline(cin, line)) {
		try {
			send_command(conn, line);
			cout << read_string(conn) << endl;
		}
		catch (ConnectionClosedException&) {
			cout << "No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

