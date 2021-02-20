#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Utils
{
public:
    Utils();
    static vector<string> split(string str, string pattern);
    static vector<string> split(string str, string pattern1, string pattern2);
    static string trim(string &s);
    static bool isNum(string str);
    static string trim(string str, string del);
};

#endif // UTILS_H
