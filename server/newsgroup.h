#ifndef EDA031_PROJECT_NEWSGROUP_H
#define EDA031_PROJECT_NEWSGROUP_H

#include <vector>
#include "storage.h"
#include <map>
#include <set>

using namespace std;


class Newsgroup {
public:
    Newsgroup(const string &new_name) : art_id(0), name(new_name) {}
    string get_name() const {return name;}
    vector<pair<int, string>> list_art() const;
    void create_art(const string &title, const string &author, const string &text);
    void delete_art(const int &ng);
    article get_art(const int &ng) const;
private:
    int art_id;
    string name;
    map<int, article> art_map;
};

#endif //EDA031_PROJECT_INMEM_STORAGE_H
