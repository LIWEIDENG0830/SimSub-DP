//
// create by wangfei on 2024/5/1.
//

#ifndef TRAJCSIMILAR_DISTANCE_H
#define TRAJCSIMILAR_DISTANCE_H
#include "iostream"
#include <vector>
#include <cmath>
#include <numeric>
#include "constant.h"
using namespace std;

//extern int maxLen;
//extern int minLen;
double distance(point p1, point p2);

double pointDistance(point p1, point p2);
double levenshteinDistance(point p1, point p2);
double edr(point p1, point p2, double e=0);
double dtwDistance(path path1, path path2, int start, int end);
double Discret_Frechet(path path1, path path2, int start, int end);
#endif //TRAJCSIMILAR_DISTANCE_H
