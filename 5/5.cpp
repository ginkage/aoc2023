#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>

using namespace std;

long result = 1 << 30;

map<long, long> seeds;
map<pair<long, long>, long> maps[7];

void remap(long start, long end, int cat) {
    if (cat == 7) {
        result = min(result, start);
        return;
    }

    long last_s = start;
    for (auto m : maps[cat]) {
        long src_start = m.first.first;
        long src_end = m.first.second;
        long delta = m.second;
        if (start <= src_end && src_start <= end) {
            long s = max(start, src_start), e = min(end, src_end);

            if (s > last_s)
                remap(last_s, (s - 1), cat + 1);

            remap(s + delta, e + delta, cat + 1);
            last_s = e + 1;
        }
    }

    if (last_s <= end)
        remap(last_s, end, cat + 1);
}

int main() {
    string s;
    getline(cin, s);
    const regex seedrex("seeds: (.*)");
    smatch linematch;
    if (regex_match(s, linematch, seedrex) && linematch.size() == 2) {
        stringstream ss(linematch[1]);
        while (true) {
            long seed, len;
            ss >> seed >> len;
            if (!ss) break;
            seeds[seed] = len;
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
                maps[i][make_pair(src_start, src_start + len - 1)] = dst_start - src_start;
            }
            else break;
        }
    }

    for (auto seed : seeds) {
        //remap(seed.first, seed.first, 0);
        //remap(seed.second, seed.second, 0);
        remap(seed.first, seed.first + seed.second - 1, 0);
    }

    cout << result << endl;

    return 0;
}

