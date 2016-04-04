#include "newsgroup.h"
#include "storage_exceptions.h"

using namespace std;

vector<pair<int, string>> Newsgroup::list_art() const {
	vector<pair<int, string>> v;
	for (const auto& pair : art_map) {
		v.push_back(make_pair(pair.first, pair.second.title));
	}
	return v;
}

void Newsgroup::create_art(const string &title, const string &author, const string &text) {
	art_map.insert(make_pair(++art_id, article(title, author, text)));
}

void Newsgroup::delete_art(const int &art) {
	map<int, article>::iterator it = art_map.find(art);
	if (it == art_map.end()) {
		throw ARTDoesNotExistsException();
	}	
	art_map.erase(it);
}

article Newsgroup::get_art(const int &art) const {
	map<int, article>::const_iterator it = art_map.find(art);
	if (it == art_map.end()) {
		throw ARTDoesNotExistsException();
	} 
	return  it->second;
}

