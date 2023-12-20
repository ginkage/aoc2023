#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <numeric>

using namespace std;

unordered_map<string, int> names;

int remap(string name) {
    auto it = names.find(name);
    if (it != names.end())
        return it->second;
    int id = names.size();
    names[name] = id;
    return id;
}

enum type {
    output,
    broadcast,
    flipflop,
    conjunct
};

struct node {
    type t;
    bool on;
    vector<pair<int, int>> out;
    int state, ins, all;
    bool dep;
};

vector<string> split(const string &s, const string &delimiter) {
    vector<string> res;
    size_t pos_start = 0;
    for (size_t pos_end, delim_len = delimiter.length();
            (pos_end = s.find(delimiter, pos_start)) != string::npos;
            pos_start = pos_end + delim_len)
        res.push_back(s.substr(pos_start, pos_end - pos_start));
    res.push_back(s.substr(pos_start));
    return res;
}

int main() {
    remap("broadcaster");
    const regex linerex("([%&]?)(.*) -> (.*)");
    unordered_map<int, node> nodemap;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 4) {
            string st = linematch[1].str();
            int id = remap(linematch[2].str());
            auto vs = split(linematch[3].str(), ", ");

            node &n = nodemap[id];
            n.t = st == "%" ? flipflop : (st == "&" ? conjunct : broadcast);
            n.on = false;
            n.state = 0;
            n.dep = false;
            for (auto s : vs) {
                int dst = remap(s), in = nodemap[dst].ins++;
                nodemap[dst].all |= (1 << in);
                n.out.emplace_back(dst, in);
            }
        }
    }

    vector<node> nodes(nodemap.size());
    for (auto it : nodemap)
        nodes[it.first] = it.second;

    int rx = remap("rx"), tx = -1;
    for (int i = 0; i < nodes.size() && tx < 0; i++)
        if (i != rx && nodes[i].out[0].first == rx)
            tx = i;

    int ndeps = 0;
    for (int i = 0; i < nodes.size(); i++)
        if (i != rx && nodes[i].out[0].first == tx) {
            nodes[i].dep = true;
            ndeps++;
        }

    long long result = 1;
    vector<pair<pair<int, int>, bool>> q(256);
    int head = 0, tail = 0, sz = q.size() - 1;
    for (int i = 1; i <= 1000000000 && ndeps > 0; i++) {
        q[head++ % q.size()] = make_pair(make_pair(0, 0), false);
        while (head != tail) {
            auto p = q[tail++ & sz];
            int id = p.first.first, in = p.first.second;
            bool pulse = p.second;
            node &n = nodes[id];

            if (!pulse && n.dep) {
                result = lcm(result, i);
                n.dep = false;
                ndeps--;
            }

            switch (n.t) {
            case broadcast:
                for (auto &dst : n.out)
                    q[head++ & sz] = make_pair(dst, pulse);
                break;
            case flipflop:
                if (!pulse) {
                    n.on = !n.on;
                    for (auto &dst : n.out)
                        q[head++ & sz] = make_pair(dst, n.on);
                }
                break;
            case conjunct:
                n.state = (n.state & ~(1 << in)) | (pulse << in);
                bool signal = (n.state != n.all);
                for (auto &dst : n.out)
                    q[head++ & sz] = make_pair(dst, signal);
                break;
            }
        }
    }

    cout << result << endl;

    return 0;
}

