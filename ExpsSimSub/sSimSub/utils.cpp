//
// create by wangfei on 2024/5/1.
//


#include <iomanip>
#include "utils.h"
#include "dirent.h"
#include <cstring>


path str2path(const string &s) {
    vector<pair<double, double>> path;
    for (char i : s) {
        path.emplace_back(i, i);
    }
    return path;
}

// get path vector from file
void file2paths_into_vector(const string &filename,  vector<pair<int, pair<double, double>>> &res, int bound = -1) {
    ifstream ifs(filename);
    string s;
    string tmp;
    int driverCount = 0, trajectoryCount = 0;
    int count = 0;
    
    while (getline(ifs, s)) {
        if (bound > 0 && trajectoryCount >= bound) break;
        auto strs = split(s, ",");
        if (strs.size() == 4) {
            double latitude, longitude;
            string t = strs[2] + " " + strs[3];
            istringstream istr1(t);
            istr1 >> longitude >> latitude;
            pair<int, pair<double, double>> tt = {count, {latitude, longitude}};
            res.push_back(tt);
            count ++;
        }
    }
    ifs.close();
}

// exchange file into path style
void file2paths_save(const string &filename, ofstream &ofs, int bound = -1) {
    cout<<filename<<endl;
    vector<pair<int, pair<double, double>>> res;
    file2paths_into_vector(filename, res, bound);
    sort(res.begin(), res.end());
    cout<<"res size : "<<res.size()<<endl;
    ofs << res.size() << endl;
    for (auto item: res) {
        ofs << item.first << " " << setiosflags(ios::fixed) << setprecision(5) << item.second.first << " " << item.second.second << endl;
    }
}

void file2paths_t_drive(const string &dirname, const string& target, int bound = -1) {
    // from dir path get filename
    DIR *dir = opendir(dirname.c_str());
    ofstream ofs(target);
    if (dir == nullptr) {
        std::cerr << "open dir failed" << std::endl;
        return;
    }
    struct dirent* entry;
    int count = 0;
    while((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        count ++;
        file2paths_save(dirname + "/" + entry->d_name, ofs, bound);
    }
    ofs.close();
    closedir(dir);
    cout << "all file number is : " << count << endl;
}

vector<string> split(string str, string pattern) {
    string::size_type pos;
    vector<string> result;
    str += pattern;
    int size = str.size();

    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

vector<path> readfileWithLimit(const string &filename, int count) {
    int point_num;
    vector<path> paths;
    ifstream ifs(filename);
    int idx = 0;
    while (!ifs.eof()) {
        int time_index;
        double lat, lon;
        path p;
        ifs >> point_num;
        for (int i = 0; i < point_num; ++i) {
            ifs >> time_index >> lat >> lon;
            p.emplace_back(lat, lon);
        }
        paths.push_back(p);
        idx ++;
        if (count != -1 && idx >= count) {
            break;
        }
    }
    return paths;
}

void dataAnalize(const string& filename) {
    int count = 0;
    double avg_length = 0;
    double avg_lat = 0;
    double avg_lon = 0;
    double max_lat = -10000;
    double min_lat = 10000;
    double max_lon = -10000;
    double min_lon = 10000;

    int point_num;
    ifstream ifs(filename);
    while (!ifs.eof()) {
        int time_index;
        double lat, lon;
        ifs >> point_num;
        for (int i = 0; i < point_num; ++i) {
            ifs >> time_index >> lat >> lon;

        }
        avg_length = (count * avg_length + point_num) / (count + 1);
        avg_lat = (count * avg_lat + lat) / (count + 1);
        avg_lon = (count * avg_lon + lon) / (count + 1);
        count += 1;
        max_lat = max(max_lat, lat);
        min_lat = min(min_lat, lat);
        max_lon = max(max_lon, lon);
        min_lon = min(min_lon, lon);
    }
    cout << "avgLen:" << avg_length << endl;
    cout << "count:" << count << endl;
    cout << "max_lat:" << max_lat << endl;
    cout << "min_lat:" << min_lat << endl;
    cout << "avg_lat:" << avg_lat << endl;
    cout << "max_lon:" << max_lon << endl;
    cout << "min_lon:" << min_lon << endl;
    cout << "avg_lon:" << avg_lon << endl;
}
