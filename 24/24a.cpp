#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <queue>
#include <unordered_set>
#include <Eigen/Dense>

using namespace std;

typedef Eigen::Vector2d vec2;
typedef Eigen::Matrix2d mat2;

struct hail_t {
    vec2 p, s;
};

int main() {
    vector<hail_t> stones;
    const regex linerex("(.*), (.*), (.*) @ (.*), (.*), (.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 7) {
            stones.push_back({
                .p = vec2(stod(linematch[1].str()), stod(linematch[2].str())),
                .s = vec2(stod(linematch[4].str()), stod(linematch[5].str()))
            });
        }
    }

    int n = stones.size(), result = 0;
    for (int i = 0; i < n; i++) {
        hail_t &hi = stones[i];
        for (int j = 0; j < i; j++) {
            hail_t &hj = stones[j];
            mat2 m;
            m << hi.s, hj.s;
            auto solver = m.colPivHouseholderQr();
            if (solver.absDeterminant() == 0) continue;
            vec2 t = solver.solve(hj.p - hi.p);
            if (t[0] > 0 && t[1] < 0) {
                vec2 v(hi.p + t[0] * hi.s);
                if (v[0] >= 200000000000000ll && v[0] <= 400000000000000ll
                        && v[1] >= 200000000000000ll && v[1] <= 400000000000000ll)
                    result++;
            }
        }
    }
    cout << result << endl;

    return 0;
}

