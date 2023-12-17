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
int m, n, sz;

typedef pair<int, int> cell;

struct hist {
    cell c[11];
    long d;
};

struct Hash {
    size_t operator ()(const hist& h) const {
        size_t result = 0;
        for (int i = 0; i < 11; i++)
            result = result * sz + h.c[i].first * n + h.c[i].second;
        return result;
    }
};

bool operator < (const hist& a, const hist& b) {
    return a.d > b.d;
}

bool operator == (const hist& a, const hist& b) {
    return a.c[0] == b.c[0];
}

priority_queue<hist, vector<hist>> pq;
unordered_set<hist, Hash> visit;

void maybe_push(int i, int j, hist &h) {
    if (i < 0 || j < 0 || i >= m || j >= n)
        return;

    if (i == h.c[1].first && j == h.c[1].second)
        return;

    int di = h.c[1].first - h.c[0].first, dj = h.c[1].second - h.c[0].second, same = 1;
    for (int t = 1; t < 10; t++)
        if (h.c[t + 1].first - h.c[t].first == di && h.c[t + 1].second - h.c[t].second == dj)
            same++;
        else
            break;

    if (h.c[0].first - i == di && h.c[0].second - j == dj) {
        if (same >= 10)
            return;
    }
    else if (same < 4 && h.c[1].first != -1 && h.c[1].second != -1)
        return;

    hist nh = h;
    for (int t = 10; t > 0; t--)
        nh.c[t] = nh.c[t - 1];
    nh.c[0] = make_pair(i, j);
    nh.d += matrix[i][j] - '0';

    if (visit.insert(nh).second)
        pq.push(nh);
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
    sz = m * n;

    hist h;
    for (int t = 1; t < 11; t++)
        h.c[t].first = h.c[t].second = -1;
    h.c[0] = make_pair(0, 0);
    h.d = 0;
    pq.push(h);

    while (!pq.empty()) {
        hist h = pq.top();
        pq.pop();
        int i = h.c[0].first, j = h.c[0].second;
        // cout << h.d << ' ' << i << ' ' << j << endl;
        if (i == m - 1 && j == n - 1) {
            int di = h.c[1].first - h.c[0].first, dj = h.c[1].second - h.c[0].second, same = 1;
            for (int t = 1; t < 10; t++)
                if (h.c[t + 1].first - h.c[t].first == di && h.c[t + 1].second - h.c[t].second == dj)
                    same++;
                else
                    break;
            if (same < 4)
                continue;
            cout << h.d << endl;
            break;
        }
        maybe_push(i - 1, j, h);
        maybe_push(i + 1, j, h);
        maybe_push(i, j - 1, h);
        maybe_push(i, j + 1, h);
    }

    return 0;
}

