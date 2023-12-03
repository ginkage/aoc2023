#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

vector<string> input;
int m, n;

char sym(int i, int j) {
    if (i < 0 || j < 0 || i >= m || j >= n) return '.';
    return input[i][j];
}

int main() {
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        input.emplace_back(std::move(s));
    }

    long result = 0;
    m = input.size();
    n = input[0].size();

    map<pair<int, int>, vector<long>> gears;

    for (int i = 0; i < m; i++) {
        int start = -1;
        for (int j = 0; j <= n; j++) {
            if (j < n && isdigit(input[i][j])) {
                if (start < 0)
                    start = j;
            }
            else {
                if (start >= 0) {
                    int len = j - start;
                    long num = stol(input[i].substr(start, len));
                    //bool adj = false;
                    for (int di = -1; di <= 1; di++)
                        for (int dj = -1; dj <= len; dj++)
                            if (di != 0 || dj < 0 || dj >= len) {
                                char c = sym(i + di, start + dj);
                                //if (c != '.' && !isdigit(c))
                                //    adj = true;
                                if (c == '*') {
                                    gears[make_pair(i + di, start + dj)].push_back(num);
                                }
                            }
                    //if (adj)
                    //    result += num;
                }
                start = -1;
            }
        }
    }

    for (auto &g : gears) {
        if (g.second.size() == 2) {
            result += g.second[0] * g.second[1];
        }
    }

    cout << result << endl;

    return 0;
}

