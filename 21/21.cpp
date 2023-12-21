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

vector<string> matrix;
queue<pair<pair<int, int>, int>> q;
int m, n;

struct Hash {
    size_t operator ()(const pair<int, int>& k) const {
        return k.first * n + k.second;
    }
};

unordered_set<pair<int, int>, Hash> pre, cur, nxt;

inline int modulo(int t, int k) {
    int mod = t % k;
    if (mod < 0)
        mod += k;
    return mod;
}

inline void maybe_push(int i, int j) {
    auto coord = make_pair(i, j);
    if (matrix[modulo(i, m)][modulo(j, n)] == '.'
            && pre.find(coord) == pre.end()
            && cur.find(coord) == cur.end())
        nxt.insert(coord);
}

int main() {
    int si = -1, sj = -1;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        if (si < 0) {
            for (int j = 0; j < s.size(); j++)
                if (s[j] == 'S') {
                    si = matrix.size();
                    sj = j;
                    s[j] = '.';
                    break;
                }
        }
        matrix.push_back(s);
    }

    m = matrix.size();
    n = matrix[0].size();

    vector<long> pd2(n), d3(n);
    long result = 0, pn = 0;
    maybe_push(si, sj);
    int lim = n * 10;
    for (int i = 1; i < lim; i++) {
        swap(cur, pre);
        swap(cur, nxt);
        nxt.clear();
        for (auto &c : cur) {
            int i = c.first, j = c.second;
            maybe_push(i - 1, j);
            maybe_push(i + 1, j);
            maybe_push(i, j - 1);
            maybe_push(i, j + 1);
        }
        if (i % 2 != 0) {
            int j = (i / 2) % n;
            long d1 = nxt.size();
            long d2 = d1 - pn;
            d3[j] = d2 - pd2[j];
            result += d1;
            pn = d1;
            pd2[j] = d2;
        }
    }

    for (int i = lim + 1, j = 0; i <= 26501365; i += 2, j = (j + 1) % n) {
        long d2 = pd2[j] + d3[j];
        long d1 = pn + d2;
        result += d1;
        pn = d1;
        pd2[j] = d2;
    }

    cout << result << endl;

    return 0;
}

