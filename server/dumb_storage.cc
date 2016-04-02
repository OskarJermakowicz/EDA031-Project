#include "dumb_storage.h"
#include "storage_exceptions.h"

using namespace std;

vector <pair<int, string>> dumb_storage::list_ng() const {
    vector <pair<int, string>> result;
    return result;

}

void dumb_storage::create_ng(const string &ng) { }

void dumb_storage::delete_ng(int ng) { }

vector <pair<int, string>> dumb_storage::list_art(int ng) const {
    throw NGDoesNotExistsException();
}

void dumb_storage::create_art(int ng, const string &title, const string &author, const string &text) { }

void dumb_storage::delete_art(int ng, int art) { }

article dumb_storage::get_art(int ng, int art) const {
    throw NGDoesNotExistsException();
}

