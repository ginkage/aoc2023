#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

typedef pair<string, long> hand_bid;

int type(const string &s) {
    map<char, int> cnt;
    for (char c : s)
        cnt[c]++;

    if (cnt.size() == 1)
        return 0; // Five-of-a-kind

    int sz[5], i = 0;
    for (const auto &c : cnt)
        sz[i++] = c.second;
    sort(sz, sz + i);

    int j = cnt.find('1') == cnt.end() ? 0 : cnt['1'];

    if (cnt.size() == 2) { // 1+4 or 2+3
        if (j > 0) return 0; // (1+4) or (2+3)
        return sz[1] == 4 ? 1 : 2;
    }

    if (cnt.size() == 3) { // 1+1+3 or 1+2+2
        if (j == 1) // 1+(1+3) or (1+2)+2
            return sz[2] == 3 ? 1 : 2;
        if (j >= 2) return 1; // 1+(1+3) or 1+(2+2)
        return sz[2] == 3 ? 3 : 4;
    }

    if (cnt.size() == 4) { // 1+1+1+2
        if (j > 0) return 3; // 1+1+(1+2)
        return 5;
    }

    return (j > 0 ? 5 : 6); // High Card
}

bool comp(const hand_bid &l, const hand_bid &r) {
    const string &lh = l.first, &rh = r.first;
    int lt = type(lh), rt = type(rh);
    return lt > rt || (lt == rt && lh < rh);
}

int main() {
    vector<hand_bid> bids;
    const regex linerex("(.*) (\\d+)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            string hand = linematch[1].str();
            for (char &c : hand) {
                switch (c) {
                    case 'A': c = 'E'; break;
                    case 'K': c = 'D'; break;
                    case 'Q': c = 'C'; break;
                    // case 'J': c = 'B'; break;
                    case 'J': c = '1'; break;
                    case 'T': c = 'A'; break;
                }
            }
            long bid = stol(linematch[2].str());
            bids.emplace_back(hand, bid);
        }
    }

    sort(bids.begin(), bids.end(), comp);

    long result = 0, i = 1;
    for (const auto &b : bids) {
        cout << b.first << endl;
        result += b.second * (i++);
    }
    cout << result << endl;

    return 0;
}

