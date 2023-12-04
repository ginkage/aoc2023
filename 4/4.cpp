#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>

using namespace std;

int main() {
    long result = 0;
    map<int, long> copies;
    const regex linerex("Card\\s+*(\\d+):\\s+(.*)\\s+\\|\\s+(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 4) {
            long id = stol(linematch[1].str());
            stringstream ss1(linematch[2].str());
            stringstream ss2(linematch[3].str());

            copies[id]++;

            int num;
            set<int> win, have;
            while (ss1) {
                ss1 >> num;
                win.insert(num);
            }
            while (ss2) {
                ss2 >> num;
                have.insert(num);
            }

            int match = 0;
            for (int k : have)
                if (win.find(k) != win.end())
                    match++;

            for (int i = 1; i <= match; i++)
                copies[id + i] += copies[id];

            //if (match > 0)
            //    result += (1 << (match - 1));
        }
    }

    for (auto &t : copies)
        result += t.second;

    cout << result << endl;

    return 0;
}

