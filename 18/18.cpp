#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <queue>
#include <unordered_set>

using namespace std;

int main() {
    long x = 0, y = 0, result = 0, perim = 0;
    const regex linerex("(.*) (.*) \\(#(.....)(.)\\)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 5) {
            string dir = linematch[4].str(); //linematch[1].str();
            long dist = stol(linematch[3].str(), nullptr, 16); //stol(linematch[2].str());
            // string color = linematch[3].str();

            perim += dist;
            long px = x, py = y;
            if (dir == "3") y -= dist;
            else if (dir == "1") y += dist;
            else if (dir == "2") x -= dist;
            else if (dir == "0") x += dist;
            result += px * y - py * x;
        }
    }

    result = abs(result) / 2 + perim / 2 + 1;
    cout << result << endl;

    return 0;
}

