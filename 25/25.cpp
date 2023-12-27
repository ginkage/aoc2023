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

int remap(string s) {
    auto it = names.find(s);
    if (it != names.end())
        return it->second;
    int id = names.size();
    names[s] = id;
    return id;
}

int n;

struct Hash {
    size_t operator ()(const pair<int, int>& k) const {
        return k.first * n + k.second;
    }
};

unordered_map<int, vector<int>> node;
unordered_map<pair<int, int>, long long, Hash> weight;

void descend(unordered_map<int, vector<bool>> &cur_level, vector<bool> &visited) {
    if (cur_level.empty()) return;

    unordered_map<int, vector<bool>> next_level;
    for (auto &cur : cur_level) {
        int from = cur.first;
        for (int to : node[from])
            if (!visited[to]) {
                visited[to] = true;
                next_level[to] = vector<bool>(n);
                next_level[to][to] = true;
            }
    }

    descend(next_level, visited);

    for (auto &cur : cur_level) {
        int from = cur.first;
        for (int to : node[from]) {
            auto it = next_level.find(to);
            if (it != next_level.end()) {
                vector<bool> &nxt = it->second;
                int sz = 0;
                for (int i = 0; i < n; i++)
                    if (nxt[i]) {
                        cur.second[i] = true;
                        sz++;
                    }
                weight[make_pair(min(from, to), max(from, to))] += sz;
            }
        }
    }
}

int main() {
    const regex linerex("(.*): (.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            int from = remap(linematch[1].str());
            stringstream ss(linematch[2].str());
            while (true) {
                getline(ss, s, ' ');
                if (!ss) break;
                int to = remap(s);
                node[from].push_back(to);
                node[to].push_back(from);
            }
        }
    }

    n = names.size();
    for (int i = 0; i < n; i++) {
        vector<bool> visited(n);
        unordered_map<int, vector<bool>> cur;
        cur[i] = vector<bool>(n);
        cur[i][i] = true;
        visited[i] = true;
        descend(cur, visited);
    }

    vector<pair<pair<int, int>, long long>> v(weight.begin(), weight.end());
    sort(v.begin(), v.end(), [](const auto &l, auto &r) { return l.second > r.second; });

    for (int i = 0; i < 3; i++) {
        int from = v[i].first.first, to = v[i].first.second;
        vector<int> &vf = node[from];
        vector<int> &vt = node[to];
        vf.erase(find(vf.begin(), vf.end(), to));
        vt.erase(find(vt.begin(), vt.end(), from));
    }

    vector<int> parent(n), size(n);
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        size[i] = 1;
    }

    for (int i = 0; i < n; i++) {
        int x = i;
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }

        for (int e : node[i]) {
            int y = e;
            while (y != parent[y]) {
                parent[y] = parent[parent[y]];
                y = parent[y];
            }

            if (x != y) {
                parent[y] = x;
                size[x] += size[y];
            }
        }
    }

    long result = 1;
    for (int i = 0; i < n; i++)
        if (parent[i] == i)
            result *= size[i];
    cout << result << endl;

    return 0;
}

