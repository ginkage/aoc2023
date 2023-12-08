#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>

using namespace std;

string lr;
map<string, int> remap;
unordered_map<int, pair<int, int>> nodes;

int lookup(const string &s) {
    auto it = remap.find(s);
    if (it != remap.end())
        return it->second;

    int id = remap.size();
    remap[s] = id;
    return id;
}

long gcd(long a, long b) {
    while (a != 0) {
        b %= a;
        swap(a, b);
    }
    return b;
}

long lcm(long a, long b) {
    if (a > b)
        return (a / gcd(a, b)) * b;
    else
        return (b / gcd(a, b)) * a;
}

int main() {
    getline(cin, lr);

    string s;
    getline(cin, s);

    const regex linerex("(.*) = \\((.*), (.*)\\)");
    while (true) {
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 4) {
            string src = linematch[1], dl = linematch[2], dr = linematch[3];
            nodes[lookup(src)] = make_pair(lookup(dl), lookup(dr));
        }
    }

    vector<int> start;
    start.reserve(remap.size());

    vector<bool> finish(remap.size(), false);

    for (auto &m : remap) {
        const string &s = m.first;
        int id = m.second;
        if (s[2] == 'A') start.push_back(id);
        if (s[2] == 'Z') finish[id] = true;
    }

    int n = start.size();
    vector<int> loop(n, 0), ends(n, 0), pivot(n, 0);

    for (int i = 0; i < n; i++) {
        int node = start[i], steps = 0, idx = 0;
        map<pair<int, int>, int> visit;
        while (visit.find(make_pair(node, idx)) == visit.end()) {
            visit[make_pair(node, idx)] = steps;
            if (finish[node] && !ends[i]) ends[i] = steps;
            node = lr[idx] == 'L' ? nodes[node].first : nodes[node].second;
            idx = (++steps) % lr.size();
        }
        pivot[i] = visit[make_pair(node, idx)];
        start[i] = node;
        ends[i] -= pivot[i];
        loop[i] = steps - pivot[i];
    }

    long result = 1;
    for (int i = 0; i < n; i++) {
        cout << start[i] << ' ' << ends[i] << ' ' << pivot[i] << ' ' << loop[i] << endl;
        result = lcm(result, loop[i]);
    }
    cout << result << endl;

    return 0;
}

