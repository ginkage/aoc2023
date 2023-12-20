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
    vector<int> out;
    unordered_map<int, bool> in;
};

unordered_map<int, node> nodes;

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

long lo = 0, hi = 0;

bool push_button(int rx) {
    queue<pair<pair<int, int>, bool>> q;
    q.emplace(make_pair(-1, 0), false);
    while (!q.empty()) {
        auto p = q.front();
        int src = p.first.first, id = p.first.second;
        bool pulse = p.second;
        q.pop();

        if (pulse) hi++;
        else {
            lo++;
            if (id == rx)
                return true;
        }

        node &n = nodes[id];
        if (n.t == broadcast) {
            for (int dst : n.out)
                q.emplace(make_pair(id, dst), pulse);
        }
        else if (n.t == flipflop) {
            if (!pulse) {
                n.on = !n.on;
                for (int dst : n.out)
                    q.emplace(make_pair(id, dst), n.on);
            }
        }
        else if (n.t == conjunct) {
            n.in[src] = pulse;
            bool good = true;
            for (auto in : n.in)
                good &= in.second;
            for (int dst : n.out)
                q.emplace(make_pair(id, dst), !good);
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

            node &n = nodes[id];
            n.t = st == "%" ? flipflop : (st == "&" ? conjunct : broadcast);
            n.on = false;
            for (auto s : vs) {
                int dst = remap(s);
                n.out.push_back(dst);
                nodes[dst].in[id] = 0;
            }
        }
    }

    int rx = remap("rx");
    for (int i = 1; i <= 1000; i++)
        if (push_button(rx)) {
            cout << i << endl;
            break;
        }

    cout << lo << ' ' << hi << ' ' << (lo * hi) << endl;

    return 0;
}

