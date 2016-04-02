#include "../library/connection.h"
#include "../library/connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

// Could maybe be in a seperate txt file
const string help_text = " ";

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string readString(const Connection& conn) {
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
			// regex the input to see if it's a valid cmd, send corresponding nbr in protocol.h to server, print output
			// if input help, print help_text
		}
		catch (ConnectionClosedException&) {
			cout << "No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

