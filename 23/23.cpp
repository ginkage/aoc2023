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

typedef pair<int, int> coord_t;

vector<string> matrix;
int m, n;

struct Hash {
    size_t operator ()(const coord_t& k) const {
        return k.first * n + k.second;
    }
};

struct edge_t {
    coord_t from, to;
    int len;
};

struct wave_t {
    edge_t edge;
    coord_t last;
};

struct node_t {
    vector<pair<int, int>> edge;
    int max_dist;
};

vector<node_t> nodes;

void iterate(int cur, uint64_t visit, int len) {
    for (pair<int, int> &edge : nodes[cur].edge) {
        uint64_t mask = (1lu << edge.first);
        if (!(visit & mask)) {
            node_t &to = nodes[edge.first];
            int nl = len + edge.second;
            to.max_dist = max(to.max_dist, nl);
            iterate(edge.first, visit | mask, nl);
        }
    }
}

int main() {
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        matrix.push_back(s);
    }

    m = matrix.size();
    n = matrix[0].size();
    coord_t st(0, 1), en(m - 1, n - 2);

    unordered_map<coord_t, int, Hash> remap;
    vector<edge_t> edges;

    int di[4] = { -1, 0, 1, 0 };
    int dj[4] = { 0, -1, 0, 1 };
    coord_t dirs[4];

    queue<wave_t> q;
    q.push({ .edge = { .from = st, .to = make_pair(1, 1), .len = 1 }, .last = st });
    remap[st] = 0;
    remap[en] = 1;
    while (!q.empty()) {
        wave_t wave = q.front();
        q.pop();

        coord_t cur = wave.edge.to;
        char ch = matrix[cur.first][cur.second];
        if (ch == '>') {
            wave.edge.to = make_pair(cur.first, cur.second + 1);
            wave.edge.len++;
            wave.last = cur;
            q.push(wave);
        }
        else if (ch == 'v') {
            wave.edge.to = make_pair(cur.first + 1, cur.second);
            wave.edge.len++;
            wave.last = cur;
            q.push(wave);
        }
        else if (cur == en) {
            edges.push_back(wave.edge);
        }
        else {
            int nd = 0, np = 0;
            for (int t = 0; t < 4; t++) {
                coord_t c(cur.first + di[t], cur.second + dj[t]);
                char cd = matrix[c.first][c.second];
                if (c != wave.last && cd != '#') {
                    np++;
                    if (!((di[t] == -1 && cd == 'v') || (dj[t] == -1 && cd == '>')))
                        dirs[nd++] = c;
                }
            }

            if (np == 1) {
                wave.edge.to = dirs[0];
                wave.edge.len++;
                wave.last = cur;
                q.push(wave);
            }
            else {
                edges.push_back(wave.edge);
                if (remap.find(cur) == remap.end()) {
                    int sz = remap.size();
                    remap[cur] = sz;
                    for (int t = 0; t < nd; t++)
                        q.push({ .edge = { .from = cur, .to = dirs[t], .len = 1 }, .last = cur });
                }
            }
        }
    }

    nodes.resize(remap.size());
    for (edge_t &e : edges)
        nodes[remap[e.from]].edge.emplace_back(remap[e.to], e.len);

    iterate(0, 0, 0);
    cout << nodes[1].max_dist << endl;

    for (edge_t &e : edges)
        if (e.from != st && e.to != en)
            nodes[remap[e.to]].edge.emplace_back(remap[e.from], e.len);

    iterate(0, 0, 0);
    cout << nodes[1].max_dist << endl;

    return 0;
}

