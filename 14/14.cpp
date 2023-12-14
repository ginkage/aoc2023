#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

vector<string> matrix;
int m, n;

void north() {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j] == 'O') {
                int k = i - 1;
                while (k >= 0 && matrix[k][j] == '.')
                    k--;
                swap(matrix[i][j], matrix[k+1][j]);
            }
}

void west() {
    for (int j = 0; j < n; j++)
        for (int i = 0; i < m; i++)
            if (matrix[i][j] == 'O') {
                int k = j - 1;
                while (k >= 0 && matrix[i][k] == '.')
                    k--;
                swap(matrix[i][j], matrix[i][k+1]);
            }
}

void south() {
    for (int i = m - 1; i >= 0; i--)
        for (int j = 0; j < n; j++)
            if (matrix[i][j] == 'O') {
                int k = i + 1;
                while (k < m && matrix[k][j] == '.')
                    k++;
                swap(matrix[i][j], matrix[k-1][j]);
            }
}

void east() {
    for (int j = n - 1; j >= 0; j--)
        for (int i = 0; i < m; i++)
            if (matrix[i][j] == 'O') {
                int k = j + 1;
                while (k < n && matrix[i][k] == '.')
                    k++;
                swap(matrix[i][j], matrix[i][k-1]);
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

    vector<vector<string>> history;

    int t1 = -1, t2 = -1;
    for (int t = 1; t < 1000000000 && t1 < 0; t++) {
        history.push_back(matrix);

        north();
        west();
        south();
        east();

        for (int k = t - 1; k >= 0; k--)
            if (matrix == history[k]) {
                t1 = k;
                t2 = t;
                break;
            }
    }

    cout << t1 << ' ' << t2 << endl;

    long result = 0;
    matrix = history[t1 + (1000000000 - t1) % (t2 - t1)];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j] == 'O')
                result += m - i;

    cout << result << endl;

    return 0;
}

