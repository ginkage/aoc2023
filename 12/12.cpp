#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

long iterate(int soff, const vector<int> &bads, int boff, const string &pattern, int sum, unordered_map<long, long> &memo) {
    long idx = soff * (bads.size() + 1) + boff;
    auto it = memo.find(idx);
    if (it != memo.end())
        return it->second;

    if (boff >= bads.size()) {
        bool good = true;
        for (int i = soff; i < pattern.size() && good; i++)
            good &= pattern[i] != '#';
        memo[idx] = good;
        return good;
    }

    int cur = bads[boff];
    int rem = sum + (bads.size() - boff) - 1; // Max width of the remaining bad ones
    int lo = soff, hi = pattern.size() - rem;
    long result = 0;
    for (int i = lo; i <= hi; i++) {
        bool good = true;
        for (int j = 0; j < cur && good; j++)
            good &= pattern[i + j] != '.';

        if (good && pattern[i + cur] != '#')
            result += iterate(i + cur + 1, bads, boff + 1, pattern, sum - cur, memo);

        if (pattern[i] == '#')
            break;
    }
    memo[idx] = result;
    return result;
}

int main() {
    long result = 0;
    const regex linerex("(.*) (.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            string pattern = linematch[1].str();
            stringstream ss(linematch[2].str());

            int sum = 0;
            vector<int> bads;
            while (true) {
                getline(ss, s, ',');
                if (!ss) break;
                bads.push_back(stoi(s));
                sum += bads.back();
            }

            pattern = pattern + '?' + pattern + '?' + pattern + '?' + pattern + '?' + pattern;
            vector<int> bads5;
            for (int i = 0; i < 5; i++)
                bads5.insert(bads5.end(), bads.begin(), bads.end());

            unordered_map<long, long> memo;
            result += iterate(0, bads5, 0, pattern, sum * 5, memo);
        }
    }

    cout << result << endl;

    return 0;
}

