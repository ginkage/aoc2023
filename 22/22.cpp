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

struct brick_t {
    int id;
    int x1, y1, z1;
    int x2, y2, z2;
    vector<int> above, below;
};

int m, n;

typedef pair<int, int> coord_t;

struct Hash {
    size_t operator ()(const coord_t& k) const {
        return k.first * n + k.second;
    }
};

inline int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int main() {
    vector<brick_t> bricks;
    vector<int> index;
    const regex linerex("(.*),(.*),(.*)~(.*),(.*),(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 7) {
            brick_t brick {
                .id = (int)bricks.size(),
                .x1 = stoi(linematch[1].str()),
                .y1 = stoi(linematch[2].str()),
                .z1 = stoi(linematch[3].str()),
                .x2 = stoi(linematch[4].str()),
                .y2 = stoi(linematch[5].str()),
                .z2 = stoi(linematch[6].str())
            };
            index.push_back(brick.id);
            bricks.push_back(brick);
        }
    }

    int mnx = bricks[0].x1, mny = bricks[0].y1, mxx = bricks[0].x2, mxy = bricks[0].y2;
    for (brick_t &b : bricks) {
        mnx = min(mnx, b.x1);
        mny = min(mny, b.y1);
        mxx = max(mxx, b.x2);
        mxy = max(mxy, b.y2);
    }

    m = mxx - mnx + 1;
    n = mxy - mny + 1;
    sort(index.begin(), index.end(), [&](const int l, const int r) { return bricks[l].z1 < bricks[r].z1; });

    unordered_map<coord_t, int, Hash> ground;
    for (int id : index) {
        brick_t &b = bricks[id];
        set<int> init;
        int dx = sign(b.x2 - b.x1), dy = sign(b.y2 - b.y1);
        if (dx == 0 && dy == 0) dx = dy = 1;
        for (int x = b.x1, y = b.y1; x <= b.x2 && y <= b.y2; x += dx, y += dy) {
            coord_t c(x, y);
            auto it = ground.find(c);
            if (it != ground.end())
                init.insert(it->second);
        }

        int mz = 0;
        for (int i : init)
            mz = max(mz, bricks[i].z2);

        for (int i : init) {
            brick_t &bb = bricks[i];
            if (bb.z2 == mz) {
                bb.above.push_back(id);
                b.below.push_back(i);
            }
        }

        int nz1 = mz + 1, dz = b.z2 - b.z1;
        b.z1 = nz1;
        b.z2 = nz1 + dz;
        for (int x = b.x1, y = b.y1; x <= b.x2 && y <= b.y2; x += dx, y += dy) {
            coord_t c(x, y);
            ground[c] = id;
        }
    }

    int result1 = 0;
    long result2 = 0;
    for (brick_t &brick : bricks) {
        unordered_set<int> fall;
        fall.insert(brick.id);

        queue<int> q;
        q.push(brick.id);

        while (!q.empty()) {
            int id = q.front();
            brick_t &b = bricks[id];
            q.pop();

            for (int i : b.above) {
                if (fall.find(i) == fall.end()) {
                    bool sup = false;
                    for (int d : bricks[i].below)
                        if (fall.find(d) == fall.end())
                            sup = true;

                    if (!sup) {
                        fall.insert(i);
                        q.push(i);
                    }
                }
            }
        }

        if (fall.size() == 1)
            result1++;
        else
            result2 += fall.size() - 1;
    }

    cout << result1 << ' ' << result2 << endl;

    return 0;
}

