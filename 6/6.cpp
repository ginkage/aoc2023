#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

int main() {
    long num;
    vector<long> time, dist;
    smatch linematch;
    string s;

    const regex trex("Time:\\s+(.*)");
    getline(cin, s);
    if (regex_match(s, linematch, trex) && linematch.size() == 2) {
        string str = linematch[1].str();
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        stringstream ss(str);
        while (true) {
            ss >> num;
            if (!ss) break;
            time.push_back(num);
        }
    }

    const regex drex("Distance:\\s+(.*)");
    getline(cin, s);
    if (regex_match(s, linematch, drex) && linematch.size() == 2) {
        string str = linematch[1].str();
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        stringstream ss(str);
        while (true) {
            ss >> num;
            if (!ss) break;
            dist.push_back(num);
        }
    }

    long result = 1;
    for (int i = 0; i < time.size(); i++) {
        long t = time[i], d = dist[i], ways = 0;
        for (int j = 1; j < t; j++)
            if (j * (t - j) > d)
                ways++;
        result *= ways;
    }
    cout << result << endl;

    return 0;
}

