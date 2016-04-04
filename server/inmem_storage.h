#ifndef EDA031_PROJECT_INMEM_STORAGE_H
#define EDA031_PROJECT_INMEM_STORAGE_H

#include "storage.h"
#include "newsgroup.h"
#include <map>
#include <set>

using namespace std;


class inmem_storage : public storage {
public:

	inmem_storage() {}

    void create_ng(const string &ng);

    void delete_ng(int ng);

    vector <pair<int, string>> list_ng() const;

    vector <pair<int, string>> list_art(int ng) const;

    void create_art(int ng, const string &title, const string &author, const string &text);

    void delete_art(int ng, int art);

    article get_art(int ng, int art) const;
private:
	int ng_id;
	map<int, Newsgroup> ng_map;
	set<string> ng_names;
};

#endif //EDA031_PROJECT_INMEM_STORAGE_H
