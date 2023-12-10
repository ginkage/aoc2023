#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>

using namespace std;

int M, N;
vector<string> matrix;
string pipes = "|-LJ7F";

bool south(char c) { return c == '|' || c == '7' || c == 'F' || c == 'S'; }
bool north(char c) { return c == '|' || c == 'L' || c == 'J' || c == 'S'; }
bool east(char c) { return c == '-' || c == 'L' || c == 'F' || c == 'S'; }
bool west(char c) { return c == '-' || c == '7' || c == 'J' || c == 'S'; }

bool fits(char c, int row, int col) {
    char s = matrix[row + 1][col];
    char n = matrix[row - 1][col];
    char e = matrix[row][col + 1];
    char w = matrix[row][col - 1];
    switch (c) {
        case '|': return north(s) && south(n);
        case '-': return east(w) && west(e);
        case 'L': return south(n) && west(e);
        case 'J': return south(n) && east(w);
        case '7': return north(s) && east(w);
        case 'F': return north(s) && west(e);
    }
    return false;
}

int remap(int i, int j) {
    return i * N + j;
}

int main() {
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        s = '.' + s + '.';
        matrix.push_back(s);
    }
    string pad(matrix[0].size(), '.');
    matrix.insert(matrix.begin(), pad);
    matrix.push_back(pad);

    int start = -1;
    M = matrix.size();
    N = matrix[0].size();

    unordered_map<int, pair<int, int>> edges;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            char &c = matrix[i][j];
            if (c == 'S') {
                start = remap(i, j);
                for (char p : pipes)
                    if (fits(p, i, j)) {
                        c = p;
                        break;
                    }
            }

            if (c != '.' && fits(c, i, j)) {
                int node = remap(i, j);
                int s = remap(i + 1, j);
                int n = remap(i - 1, j);
                int e = remap(i, j + 1);
                int w = remap(i, j - 1);
                pair<int, int> &ed = edges[node];
                switch (c) {
                    case '|': ed = make_pair(s, n); break;
                    case '-': ed = make_pair(w, e); break;
                    case 'L': ed = make_pair(n, e); break;
                    case 'J': ed = make_pair(n, w); break;
                    case '7': ed = make_pair(s, w); break;
                    case 'F': ed = make_pair(s, e); break;
                }
            }
        }
    }

    unordered_set<int> loop;

    int len = 1;
    int prev = start, cur = edges[start].first;
    loop.insert(start);
    while (cur != start) {
        loop.insert(cur);
        pair<int, int> &ed = edges[cur];
        int next = prev == ed.first ? ed.second : ed.first;
        prev = cur;
        cur = next;
    }

    cout << loop.size()/2 << endl;

    long result = 0;
    for (int i = 0; i < M; i++) {
        int nd = 0;
        char pv = 0;
        for (int j = 0; j < N; j++) {
            char &c = matrix[i][j];
            if (loop.find(remap(i, j)) == loop.end())
                c = '.';

            if (c != '.') {
                if (c == '|')
                    nd++;
                else if (c == '7')
                    nd += (pv == 'L') ? 0 : 1;
                else if (c == 'J')
                    nd += (pv == 'F') ? 0 : 1;
                else
                    nd += (c != '-');

                if (c != '-') pv = c;
            }
            else
                result += nd % 2;
        }
    }

    cout << result << endl;

    return 0;
}

