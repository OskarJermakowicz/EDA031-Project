#ifndef EDA031_PROJECT_DUMB_STORAGE_H
#define EDA031_PROJECT_DUMB_STORAGE_H

#include "storage.h"

using namespace std;

class dumb_storage : public storage {
public:

    std::vector <std::pair<int, std::string>> list_ng() const;

    void create_ng(const string &ng);

    void delete_ng(int ng);

    std::vector <std::pair<int, std::string>> list_art(int ng) const;

    void create_art(int ng, const std::string &title, const std::string &author, const std::string &text);

    void delete_art(int ng, int art);

    article get_art(int ng, int art) const;

};


#endif //EDA031_PROJECT_DUMB_STORAGE_H
