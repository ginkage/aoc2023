#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <queue>

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
};

unordered_map<int, node> nodemap;
vector<node> nodes;

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

queue<pair<pair<int, int>, bool>> q;

bool push_button(int rx) {
    q.emplace(make_pair(0, 0), false);
    while (!q.empty()) {
        auto p = q.front();
        int id = p.first.first, in = p.first.second;
        bool pulse = p.second;
        q.pop();

        if (!pulse && id == rx)
            return true;

        node &n = nodes[id];
        switch (n.t) {
        case broadcast:
            for (auto &dst : n.out)
                q.emplace(dst, pulse);
            break;
        case flipflop:
            if (!pulse) {
                n.on = !n.on;
                for (auto &dst : n.out)
                    q.emplace(dst, n.on);
            }
            break;
        case conjunct:
            n.state = (n.state & ~(1 << in)) | (pulse << in);
            bool good = (n.state == n.all);
            for (auto &dst : n.out)
                q.emplace(dst, !good);
            break;
        }
    }

    return false;
}

int main() {
    remap("broadcaster");
    const regex linerex("([%&]?)(.*) -> (.*)");
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
            for (auto s : vs) {
                int dst = remap(s), in = nodemap[dst].ins++;
                nodemap[dst].all |= (1 << in);
                n.out.emplace_back(dst, in);
            }
        }
    }

    nodes.resize(nodemap.size());
    for (auto it : nodemap)
        nodes[it.first] = it.second;

    int rx = remap("rx");
    for (int i = 0; i < 100000; i++)
        if (push_button(rx)) {
            cout << (i+1) << endl;
            break;
        }

    return 0;
}

