#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

int vert(vector<string> &matrix, bool fix) {
    int m = matrix.size(), n = matrix[0].size();
    for (int j = 1; j < n; j++) {
        int k = min(j, n - j);
        bool good = true, err = fix;
        for (int t = 0; t < k && good; t++) {
            int j1 = j - t - 1, j2 = j + t;
            for (int i = 0; i < m && good; i++)
                if (matrix[i][j1] != matrix[i][j2]) {
                    if (err)
                        err = false;
                    else
                        good = false;
                }
        }
        if (good && !err)
            return j;
    }
    return -1;
}

int horz(vector<string> &matrix, bool fix) {
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 1; i < m; i++) {
        int k = min(i, m - i);
        bool good = true, err = fix;
        for (int t = 0; t < k && good; t++) {
            int i1 = i - t - 1, i2 = i + t;
            for (int j = 0; j < n && good; j++)
                if (matrix[i1][j] != matrix[i2][j]) {
                    if (err)
                        err = false;
                    else
                        good = false;
                }
        }
        if (good && !err)
            return i;
    }
    return -1;
}

int main() {
    long result = 0;
    while (true) {
        vector<string> matrix;

        while (true) {
            string s;
            getline(cin, s);
            if (!cin || !s.size()) break;
            matrix.push_back(s);
        }

        int m = matrix.size(), n = matrix[0].size();
        int v = vert(matrix, true);
        if (v > 0)
            result += v;
        else
            result += horz(matrix, true) * 100;

        if (!cin) break;
    }

    cout << result << endl;

    return 0;
}

