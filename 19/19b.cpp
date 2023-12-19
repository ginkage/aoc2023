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

unordered_map<string, int> nmap;

int remap(string r) {
    auto it = nmap.find(r);
    if (it != nmap.end())
        return it->second;
    int id = nmap.size();
    nmap[r] = id;
    return id;
}

struct rule {
    int8_t idx;
    char op;
    int16_t thr;
    int dst;
};

unordered_map<int, vector<rule>> rules;

struct lims {
    int16_t mn[4], mx[4];
};

long long result = 0;

void iterate(int name, lims lim) {
    if (name == 0
            || lim.mn[0] > lim.mx[0]
            || lim.mn[1] > lim.mx[1]
            || lim.mn[2] > lim.mx[2]
            || lim.mn[3] > lim.mx[3])
        return;

    if (name == 1) {
        long long delta = 1;
        for (int i = 0; i < 4; i++)
            delta *= lim.mx[i] - lim.mn[i] + 1;
        result += delta;
        return;
    }

    for (rule &rl : rules[name]) {
        if (rl.op == '<') {
            lims cl = lim;
            cl.mx[rl.idx] = min(cl.mx[rl.idx], int16_t(rl.thr - 1));
            iterate(rl.dst, cl);

            lim.mn[rl.idx] = max(lim.mn[rl.idx], rl.thr);
        }
        else if (rl.op == '>') {
            lims cl = lim;
            cl.mn[rl.idx] = max(cl.mn[rl.idx], int16_t(rl.thr + 1));
            iterate(rl.dst, cl);

            lim.mx[rl.idx] = min(lim.mx[rl.idx], rl.thr);
        }
        else
            iterate(rl.dst, lim);
    }
}

int main() {
    nmap["R"] = 0;
    nmap["A"] = 1;

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
            auto &vr = rules[remap(name)];
            stringstream ss(r);
            while (true) {
                string ex;
                getline(ss, ex, ',');
                if (!ss) break;

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
                    rl.dst = remap(m4);
                    vr.push_back(rl);
                }
                else {
                    rule rl;
                    rl.op = 0;
                    rl.dst = remap(ex);
                    vr.push_back(rl);
                }
            }
        }
        else break;
    }

    lims lim;
    lim.mn[0] = lim.mn[1] = lim.mn[2] = lim.mn[3] = 1;
    lim.mx[0] = lim.mx[1] = lim.mx[2] = lim.mx[3] = 4000;
    iterate(remap("in"), lim);

    cout << result << endl;

    return 0;
}

