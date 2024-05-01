//
// create by wangfei on 2024/5/1.
//

#ifndef TRAJCSIMILAR_UTILS_H
#define TRAJCSIMILAR_UTILS_H
#include <fstream>
#include <algorithm>
#include "distance.h"
#include "constant.h"
#include "iostream"
#include "map"
#include <sstream>
#include <sys/time.h>

using namespace std;

path str2path(const string &s);

extern string matricsType;
extern string pruningType;
extern string dataType;
void file2paths_into_vector(const string &filename,  vector<pair<int, pair<double, double>>> &res, int bound);
void file2paths_save(const string &filename, ofstream &ofs, int bound);
void file2paths_t_drive(const string &dirname, const string& target, int bound);
vector<string> split(string str, string pattern);
void dataAnalize(const string& filename);
vector<path> readfileWithLimit(const string &filename, int count = -1);
#endif //TRAJCSIMILAR_UTILS_H
