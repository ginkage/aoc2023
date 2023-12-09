#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

long unwrap(vector<long> row) {
    bool zero = true;
    int n = row.size();
    for (int i = 0; i < n && zero; i++)
        zero = row[i] == 0;

    if (zero)
        return 0;

    vector<long> deriv(n - 1, 0);
    for (int i = 1; i < n; i++)
        deriv[i - 1] = row[i] - row[i - 1];

    deriv.push_back(unwrap(deriv));

    return row.back() + deriv.back();
}

int main() {
    long result = 0;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        vector<long> row;
        stringstream ss(s);
        while (true) {
            long num;
            ss >> num;
            if (!ss) break;
            row.push_back(num);
        }

        reverse(row.begin(), row.end());

        row.push_back(unwrap(row));
        result += row.back();
    }

    cout << result << endl;

    return 0;
}

