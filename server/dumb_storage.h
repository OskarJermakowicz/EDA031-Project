#ifndef EDA031_PROJECT_DUMB_STORAGE_H
#define EDA031_PROJECT_DUMB_STORAGE_H

#include "storage.h"

using namespace std;

class dumb_storage : public storage {
public:

    vector <pair<int, string>> list_ng() const;

    void create_ng(const string &ng);

    void delete_ng(int ng);

    vector <pair<int, string>> list_art(int ng) const;

    void create_art(int ng, const string &title, const string &author, const string &text);

    void delete_art(int ng, int art);

    article get_art(int ng, int art) const;

};


#endif //EDA031_PROJECT_DUMB_STORAGE_H
