#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

string color[] = { "red", "green", "blue" };
long limit[] = { 12, 13, 14 };

using cubes = pair<int, int>;
using subset = vector<cubes>;
using game = pair<int, vector<subset>>;

vector<string> split(const string &s, const string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

int main() {
    const regex linerex("Game (\\d+): (.*)");
    vector<game> games;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            long id = stol(linematch[1].str());
            string g = linematch[2].str();

            game gg;
            gg.first = id;
            for (string ss : split(g, "; ")) {
                subset sub;
                for (string cs : split(ss, ", ")) {
                    vector<string> sp = split(cs, " ");
                    sub.push_back(make_pair(stol(sp[0]), find(color, color + 3, sp[1]) - color));
                }
                gg.second.push_back(sub);
            }
            games.push_back(gg);
        }
    }

    long result = 0;
    for (game g : games) {
        /*
        bool good = true;
        for (subset s : g.second)
            for (cubes c : s)
                if (c.first > limit[c.second])
                    good = false;
        if (good)
            result += g.first;
        */
        long mins[] = { 0, 0, 0 };
        for (subset s : g.second)
            for (cubes c : s)
                mins[c.second] = max(mins[c.second], (long)c.first);
        result += mins[0] * mins[1] * mins[2];
    }

    cout << result << endl;

    return 0;
}

