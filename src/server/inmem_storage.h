#ifndef EDA031_PROJECT_INMEM_STORAGE_H
#define EDA031_PROJECT_INMEM_STORAGE_H

#include "storage.h"
#include "newsgroup.h"
#include <map>
#include <set>


class inmem_storage : public storage {
public:

	inmem_storage() {}

    void create_ng(const std::string &ng);

    void delete_ng(int ng);

    std::vector <std::pair<int, std::string>> list_ng() const;

    std::vector <std::pair<int, std::string>> list_art(int ng) const;

    void create_art(int ng, const std::string &title, const std::string &author, const std::string &text);

    void delete_art(int ng, int art);

    article get_art(int ng, int art) const;
private:
	int ng_id;
	std::map<int, Newsgroup> ng_map;
	std::set<std::string> ng_names;
};

#endif //EDA031_PROJECT_INMEM_STORAGE_H
