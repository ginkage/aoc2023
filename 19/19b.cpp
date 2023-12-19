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

unordered_map<string, vector<rule>> rules;

struct lims { int mn[4], mx[4]; };

long long iterate(const string &name, lims lim) {
    if (name == "R" || lim.mn[0] > lim.mx[0] || lim.mn[1] > lim.mx[1] || lim.mn[2] > lim.mx[2] || lim.mn[3] > lim.mx[3])
        return 0;

    if (name == "A") {
        long long delta = 1;
        for (int i = 0; i < 4; i++)
            delta *= lim.mx[i] - lim.mn[i] + 1;
        return delta;
    }

    long long result = 0;
    for (rule &rl : rules[name]) {
        if (rl.op == '<') {
            lims cl = lim;
            cl.mx[rl.idx] = min(cl.mx[rl.idx], rl.thr - 1);
            result += iterate(rl.dst, cl);
            lim.mn[rl.idx] = max(lim.mn[rl.idx], rl.thr);
        }
        else if (rl.op == '>') {
            lims cl = lim;
            cl.mn[rl.idx] = max(cl.mn[rl.idx], rl.thr + 1);
            result += iterate(rl.dst, cl);
            lim.mx[rl.idx] = min(lim.mx[rl.idx], rl.thr);
        }
        else
            result += iterate(rl.dst, lim);
    }
    return result;
}

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

    lims lim { .mn = { 1, 1, 1, 1}, .mx = { 4000, 4000, 4000, 4000 } };
    cout << iterate("in", lim) << endl;

    return 0;
}

