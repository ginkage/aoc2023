#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

int main() {
    vector<string> matrix;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        matrix.push_back(s);
    }

    int m = matrix.size(), n = matrix[0].size();
    vector<bool> er(m, true), ec(n, true);
    vector<pair<int, int>> galaxy;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j] == '#') {
                er[i] = false;
                ec[j] = false;
                galaxy.emplace_back(i, j);
            }

    vector<int> cr(m, 0), cc(n, 0);
    cr[0] = er[0];
    for (int i = 1; i < n; i++)
        cr[i] = er[i] + cr[i - 1];

    cc[0] = ec[0];
    for (int i = 1; i < n; i++)
        cc[i] = ec[i] + cc[i - 1];

    long result = 0;
    int k = galaxy.size();
    for (int i = 1; i < k; i++) {
        int ri = galaxy[i].first, ci = galaxy[i].second;
        for (int j = 0; j < i; j++) {
            int rj = galaxy[j].first, cj = galaxy[j].second;
            result += abs(ri - rj) + abs(ci - cj) + (abs(cr[ri] - cr[rj]) + abs(cc[ci] - cc[cj])) * 999999;
        }
    }
    cout << result << endl;

    return 0;
}

