#include "inmem_storage.h"
#include "storage_exceptions.h"
#include <algorithm>

using namespace std;

void inmem_storage::create_ng(const string &ng) { 
	//bool is true if insert successful
	pair<set<string>::iterator, bool> p = ng_names.insert(ng);
	if (p.second) {
		ng_map.insert(make_pair(++ng_id, Newsgroup(ng)));
	} else {
		throw NGAlreadyExistsException();
	}
}	

void inmem_storage::delete_ng(int ng) { 
	map<int, Newsgroup>::iterator it = ng_map.find(ng);

	if (it != ng_map.end()) {
		ng_names.erase(it->second.get_name());
		ng_map.erase(it);
	} else {
		throw NGDoesNotExistsException();
	}
}

vector <pair<int, string>> inmem_storage::list_ng() const {
	vector <pair<int, string>> v;
	for (const auto& pair : ng_map) {
		v.push_back(make_pair(pair.first, pair.second.get_name()));
	}
    return v;
}

vector <pair<int, string>> inmem_storage::list_art(int ng) const {
	map<int, Newsgroup>::const_iterator it = ng_map.find(ng);

	if (it == ng_map.end()) {
		throw NGDoesNotExistsException();
	}
	return it->second.list_art();
}

void inmem_storage::create_art(int ng, const string &title, const string &author, const string &text) {
	map<int, Newsgroup>::iterator it = ng_map.find(ng);

	if (it == ng_map.end()) {
		throw NGDoesNotExistsException();
	}
	it->second.create_art(title, author, text);
}

void inmem_storage::delete_art(int ng, int art) { 
	map<int, Newsgroup>::iterator it = ng_map.find(ng);

	if (it == ng_map.end()) {
		throw NGDoesNotExistsException();
	}
	it->second.delete_art(art);
}

article inmem_storage::get_art(int ng, int art) const {
	map<int, Newsgroup>::const_iterator it = ng_map.find(ng);

	if (it == ng_map.end()) {
	    throw NGDoesNotExistsException();
	} 
	return it->second.get_art(art);
}

