#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <queue>
#include <unordered_set>

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
            string name = linematch[1].str();
            string r = linematch[2].str();
            stringstream ss(r);
            while (true) {
                string ex;
                getline(ss, ex, ',');
                if (!ss) break;

                auto &vr = rules[name];
                smatch rulematch;
                if (regex_match(ex, rulematch, rulerex) && rulematch.size() == 5) {
                    string m1 = rulematch[1].str();
                    string m2 = rulematch[2].str();
                    string m3 = rulematch[3].str();
                    string m4 = rulematch[4].str();

                    rule rl;
                    rl.idx = (m1 == "x" ? 0 : (m1 == "m" ? 1 : m1 == "a" ? 2 : 3));
                    rl.op = m2[0];
                    rl.thr = stoi(m3);
                    rl.dst = m4;
                    vr.push_back(rl);
                }
                else {
                    rule rl;
                    rl.op = 0;
                    rl.dst = ex;
                    vr.push_back(rl);
                }
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
            part[0] = stoi(linematch[1].str());
            part[1] = stoi(linematch[2].str());
            part[2] = stoi(linematch[3].str());
            part[3] = stoi(linematch[4].str());

            string name = "in";
            while (true) {
                for (rule &rl : rules[name]) {
                    if (rl.op == '<') {
                        if (part[rl.idx] < rl.thr) {
                            name = rl.dst;
                            break;
                        }
                    }
                    else if (rl.op == '>') {
                        if (part[rl.idx] > rl.thr) {
                            name = rl.dst;
                            break;
                        }
                    }
                    else {
                        name = rl.dst;
                        break;
                    }
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

