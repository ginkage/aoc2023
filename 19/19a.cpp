#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

struct rule {
    int idx;
    char op;
    int thr;
    string dst;
};

map<string, vector<rule>> rules;

int main() {
    const regex linerex("(.*)\\{(.*)\\}");
    const regex rulerex("(.)(.)(\\d+):(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            auto &vr = rules[linematch[1].str()];
            stringstream ss(linematch[2].str());
            while (true) {
                string ex;
                getline(ss, ex, ',');
                if (!ss) break;

                rule rl;
                smatch rulematch;
                if (regex_match(ex, rulematch, rulerex) && rulematch.size() == 5) {
                    string m1 = rulematch[1].str();
                    rl.idx = (m1 == "x" ? 0 : (m1 == "m" ? 1 : m1 == "a" ? 2 : 3));
                    rl.op = rulematch[2].str()[0];
                    rl.thr = stoi(rulematch[3].str());
                    rl.dst = rulematch[4].str();
                }
                else {
                    rl.op = 0;
                    rl.dst = ex;
                }
                vr.push_back(rl);
            }
        }
        else break;
    }

    long result = 0;
    const regex partrex("\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, partrex) && linematch.size() == 5) {
            int part[4];
            for (int i = 0; i < 4; i++)
                part[i] = stoi(linematch[i + 1].str());

            string name = "in";
            while (true) {
                for (rule &rl : rules[name]) {
                    if (rl.op == '<') {
                        if (part[rl.idx] < rl.thr) { name = rl.dst; break; }
                    }
                    else if (rl.op == '>') {
                        if (part[rl.idx] > rl.thr) { name = rl.dst; break; }
                    }
                    else { name = rl.dst; break; }
                }

                if (name == "A") {
                    for (int i = 0; i < 4; i++)
                        result += part[i];
                    break;
                }
                else if (name == "R")
                    break;
            }
        }
    }

    cout << result << endl;

    return 0;
}

