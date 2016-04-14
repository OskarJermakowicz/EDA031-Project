#include "disk_storage.h"
#include "storage_exceptions.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ftw.h>


using namespace std;


string disk_storage::get_newsgroup_directory(int ng) {
    return string(DATABASE_DIRECTORY) + string(NEWSGROUP_PREFIX) + to_string(ng) + "/";
}

string disk_storage::get_newsgroup_path(int ng) {
    return get_newsgroup_directory(ng) + NEWSGROUP_FILENAME;
}

string disk_storage::get_article_path(int ng, int art) {
    return get_newsgroup_directory(ng) + string(ARTICLE_PREFIX) + to_string(art);
}

vector <pair<int, string>> disk_storage::list_ng() const {
    DIR* databaseDir = opendir(DATABASE_DIRECTORY);
    if (databaseDir == nullptr)
        return vector <pair<int, string>>();

    vector <pair<int, string>> result;
    while (auto* file = readdir(databaseDir)) {
        if (file->d_type == DT_DIR) {
            string ngName(file->d_name);
            if (ngName.find(NEWSGROUP_PREFIX) == 0) {
                int ng = stoi(ngName.substr(strlen(NEWSGROUP_PREFIX)));
                ifstream ifs(get_newsgroup_path(ng));
                if (ifs) {
                    string name;
                    getline(ifs, name);
                    result.emplace_back(ng, name);
                }
                ifs.close();
            }
        }
    }
    closedir(databaseDir);

    sort(result.begin(), result.end());
    return result;
}

void disk_storage::create_ng(const string &ng) {
    mkdir(DATABASE_DIRECTORY, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    vector <pair<int, string>> newsGroups = list_ng();
    if (any_of(newsGroups.begin(), newsGroups.end(), [&ng](const pair<int, string>& entry){ return entry.second == ng;}))
        throw NGAlreadyExistsException();

    int ngId = 1;
    ifstream difs(DATABASE_DIRECTORY DATABASE_FILENAME);
    difs >> ngId;
    difs.close();
    
    mkdir(get_newsgroup_directory(ngId).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    ofstream ngofs(get_newsgroup_path(ngId));
    ngofs << ng << '\n';
    ngofs << 1 << '\n';
    ngofs.close();

    ngId++;
    ofstream dofs(DATABASE_DIRECTORY DATABASE_FILENAME, ios::trunc);
    dofs << ngId;
    dofs.close();
}

void disk_storage::delete_ng(int ng) {
    string ngPath = get_newsgroup_directory(ng);
    DIR* newsgroupDirectory = opendir(ngPath.c_str());
    if (newsgroupDirectory == nullptr)
        throw NGDoesNotExistException();

    while (auto* file = readdir(newsgroupDirectory))
         remove((ngPath + file->d_name).c_str());
    closedir(newsgroupDirectory);
    remove(ngPath.c_str());
}

vector <pair<int, string>> disk_storage::list_art(int ng) const {
    DIR* newsgroupDirectory = opendir(get_newsgroup_directory(ng).c_str());
    if (newsgroupDirectory == nullptr)
        throw NGDoesNotExistException();

    vector <pair<int, string>> result;
    while (auto* file = readdir(newsgroupDirectory)) {
        if (file->d_type == DT_REG) {
            string artName(file->d_name);
            if (artName.find(ARTICLE_PREFIX) == 0) {
                int art = stoi(artName.substr(strlen(ARTICLE_PREFIX)));
                ifstream ifs(get_article_path(ng, art));
                if (ifs) {
                    string title;
                    getline(ifs, title);
                    result.emplace_back(art, title);
                }
                ifs.close();
            }
        }
    }
    closedir(newsgroupDirectory);

    sort(result.begin(), result.end());
    return result;
}

void disk_storage::create_art(int ng, const string &title, const string &author, const string &text) {
    string ngName;
    int artId;

    ifstream ngifs(get_newsgroup_path(ng));
    if (!ngifs)
        throw NGDoesNotExistException();
    getline(ngifs, ngName);
    ngifs >> artId;
    ngifs.close();

    ofstream artofs(get_article_path(ng, artId));
    artofs << title << '\n' << author << '\n' << text;
    artofs.close();

    artId++;
    ofstream dofs(get_newsgroup_path(ng), ios::trunc);
    dofs << ngName << '\n' << artId << '\n';
    dofs.close();
}

void disk_storage::delete_art(int ng, int art) {
    ifstream ngifs(get_newsgroup_path(ng));
    if (!ngifs)
        throw NGDoesNotExistException();
    ngifs.close();

    if (remove(get_article_path(ng, art).c_str()) != 0)
        throw ARTDoesNotExistException();
}

article disk_storage::get_art(int ng, int art) const {
    // This is what we get to live with until 2017 (see ISO/IEC TS 18822:2015).
    ifstream ngifs(get_newsgroup_path(ng));
    if (!ngifs)
        throw NGDoesNotExistException();
    ngifs.close();

    ifstream ifs(get_article_path(ng, art));
    if (!ifs)
        throw ARTDoesNotExistException();
    article article{"", "", ""};
    getline(ifs, article.title);
    getline(ifs, article.author);
    getline(ifs, article.text, '\0');
    ifs.close();
    return article;
}
