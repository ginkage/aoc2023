#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>

using namespace std;

set<long> seeds;
map<long, pair<long, long>> maps[7];

long remap(long src, int cat) {
    map<long, pair<long, long>> &m = maps[cat];
    auto it = m.upper_bound(src);
    if (it != m.begin()) {
        it--;

        long src_start = it->first;
        long dst_start = it->second.first;
        long len = it->second.second;
        if (src < src_start + len)
            return dst_start + (src - src_start);
    }

    return src;
}

int main() {
    string s;
    getline(cin, s);
    const regex seedrex("seeds: (.*)");
    smatch linematch;
    if (regex_match(s, linematch, seedrex) && linematch.size() == 2) {
        long seed;
        stringstream ss(linematch[1]);
        while (ss) {
            ss >> seed;
            seeds.insert(seed);
        }
    }

    getline(cin, s); // Empty line

    const regex maprex("(\\d+) (\\d+) (\\d+)");
    for (int i = 0; i < 7; i++) {
        getline(cin, s); // Header
        if (!cin) break;

        while (true) {
            getline(cin, s);
            if (!cin) break;

            smatch linematch;
            if (regex_match(s, linematch, maprex) && linematch.size() == 4) {
                long dst_start = stol(linematch[1]);
                long src_start = stol(linematch[2]);
                long len = stol(linematch[3]);
                maps[i][src_start] = make_pair(dst_start, len);
            }
            else break;
        }
    }

    long result = 1 << 30;
    for (long seed : seeds) {
        long dst = seed;
        for (int i = 0; i < 7; i++)
            dst = remap(dst, i);
        result = min(result, dst);
    }
    cout << result << endl;

    return 0;
}

