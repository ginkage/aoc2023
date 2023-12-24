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

typedef Eigen::Vector3d vec3;
typedef Eigen::Matrix3d mat3;

struct hail_t {
    vec3 p, s;
};

mat3 crossMatrix(vec3 v)
{
    mat3 result;
    result <<
            0, -v[2],  v[1],
         v[2],     0, -v[0],
        -v[1],  v[0],     0;
    return result;
}

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
                .p = vec3(stod(linematch[1].str()), stod(linematch[2].str()), stod(linematch[3].str())),
                .s = vec3(stod(linematch[4].str()), stod(linematch[5].str()), stod(linematch[6].str()))
            });
        }
    }

    for (int j = 0; j < 3; j++) {
        hail_t h1 = stones[j], h2 = stones[j+1], h3 = stones[j+2];

        Eigen::MatrixXd M(6, 6);
        Eigen::VectorXd rhs(6);
        rhs.segment<3>(0) = -h1.p.cross(h1.s) + h2.p.cross(h2.s);
        rhs.segment<3>(3) = -h1.p.cross(h1.s) + h3.p.cross(h3.s);
        M.block<3, 3>(0, 0) = crossMatrix(h1.s) - crossMatrix(h2.s);
        M.block<3, 3>(3, 0) = crossMatrix(h1.s) - crossMatrix(h3.s);
        M.block<3, 3>(0, 3) = -crossMatrix(h1.p) + crossMatrix(h2.p);
        M.block<3, 3>(3, 3) = -crossMatrix(h1.p) + crossMatrix(h3.p);
        auto solve = M.inverse() * rhs;
        vec3 pos = solve.segment<3>(0), spd = solve.segment<3>(3);

        double result = 0;
        for (int i = 0; i < 3; i++)
            result += pos[i];
        cout.precision(32);
        cout << result << endl;
    }

    return 0;
}

