#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include <queue>

using namespace std;

struct brick_t {
    int x1, y1, z1;
    int x2, y2, z2;
    vector<int> above, below;
};

int main() {
    vector<int> index;
    vector<brick_t> bricks;
    const regex linerex("(.*),(.*),(.*)~(.*),(.*),(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 7) {
            index.push_back(bricks.size());
            bricks.push_back({
                .x1 = stoi(linematch[1].str()),
                .y1 = stoi(linematch[2].str()),
                .z1 = stoi(linematch[3].str()),
                .x2 = stoi(linematch[4].str()),
                .y2 = stoi(linematch[5].str()),
                .z2 = stoi(linematch[6].str())
            });
        }
    }

    sort(index.begin(), index.end(), [&](const int l, const int r) { return bricks[l].z1 < bricks[r].z1; });

    map<pair<int, int>, int> ground;
    for (int id : index) {
        brick_t &b = bricks[id];
        set<int> init;
        int dx = (b.x2 - b.x1) > 0, dy = (b.y2 - b.y1) > 0;
        if (dx == 0 && dy == 0) dx = dy = 1;

        for (int x = b.x1, y = b.y1; x <= b.x2 && y <= b.y2; x += dx, y += dy) {
            auto it = ground.find(make_pair(x, y));
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

        b.z2 += mz + 1 - b.z1;
        b.z1 = mz + 1;
        for (int x = b.x1, y = b.y1; x <= b.x2 && y <= b.y2; x += dx, y += dy)
            ground[make_pair(x, y)] = id;
    }

    int result1 = 0, result2 = 0;
    for (int id = 0; id < bricks.size(); id++) {
        set<int> fall;
        fall.insert(id);

        queue<int> q;
        q.push(id);
        while (!q.empty()) {
            brick_t &b = bricks[q.front()];
            q.pop();

            for (int i : b.above)
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

        result1 += (fall.size() == 1);
        result2 += (fall.size() - 1);
    }

    cout << result1 << ' ' << result2 << endl;

    return 0;
}

