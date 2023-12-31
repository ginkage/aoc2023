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
int m, n;

struct Hash {
    size_t operator ()(const pair<int, int>& k) const {
        return k.first * n + k.second;
    }
};

typedef pair<int, int> cell;
typedef pair<int, int> dir;
typedef pair<cell, dir> beam;

unordered_map<cell, unordered_set<dir, Hash>, Hash> beams;
queue<beam> q;

void maybe_add(int i, int j, int di, int dj) {
    i += di;
    j += dj;

    if (i < 0 || j < 0 || i >= m || j >= n)
        return;

    cell c(i, j);
    dir d(di, dj);
    if (beams[c].insert(d).second)
        q.emplace(c, d);
}

int start_at(int i0, int j0, int di0, int dj0) {
    beams.clear();
    maybe_add(i0 - di0, j0 - dj0, di0, dj0);

    while (!q.empty()) {
        beam b = q.front();
        cell c = b.first;
        dir d = b.second;
        int i = c.first, j = c.second;
        int di = d.first, dj = d.second;
        q.pop();

        switch (matrix[i][j]) {
            case '.':
                maybe_add(i, j, di, dj);
                break;
            case '/':
                maybe_add(i, j, -dj, -di);
                break;
            case '\\':
                maybe_add(i, j, dj, di);
                break;
            case '|':
                if (dj == 0)
                    maybe_add(i, j, di, dj);
                else {
                    maybe_add(i, j, -1, 0);
                    maybe_add(i, j, 1, 0);
                }
                break;
            case '-':
                if (di == 0)
                    maybe_add(i, j, di, dj);
                else {
                    maybe_add(i, j, 0, -1);
                    maybe_add(i, j, 0, 1);
                }
                break;
        }
    }

    return beams.size();
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

    int result = 0;

    for (int i = 0; i < m; i++) {
        result = max(result, start_at(i, 0, 0, 1));
        result = max(result, start_at(i, n - 1, 0, -1));
    }

    for (int j = 0; j < n; j++) {
        result = max(result, start_at(0, j, 1, 0));
        result = max(result, start_at(m - 1, j, -1, 0));
    }

    cout << result << endl;

    return 0;
}

