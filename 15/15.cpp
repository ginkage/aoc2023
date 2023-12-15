#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

typedef pair<string, int> tlens;
typedef vector<tlens> tbox;

int hsh(const string &s) {
    int hash = 0;
    for (char c : s)
        hash = ((hash + c) * 17) % 256;
    return hash;
}

int main() {
    vector<tbox> boxes(256);
    const regex linerex("(.*)=(.*)");

    string s;
    getline(cin, s);
    stringstream ss(s);

    while (true) {
        string op;
        getline(ss, op, ',');
        if (!ss || !op.size()) break;

        smatch linematch;
        if (regex_match(op, linematch, linerex) && linematch.size() == 3) {
            string label = linematch[1].str();
            int len = stoi(linematch[2].str());
            tbox &box = boxes[hsh(label)];

            auto it = find_if(box.begin(), box.end(), [&](tlens &l) { return l.first == label; });
            if (it == box.end())
                box.emplace_back(label, len);
            else
                it->second = len;
        }
        else {
            string label = op.substr(0, op.size() - 1);
            tbox &box = boxes[hsh(label)];
            box.erase(remove_if(box.begin(), box.end(), [&](tlens &l) { return l.first == label; }), box.end());
        }
    }

    long result = 0;
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < boxes[i].size(); j++)
            result += (i + 1) * (j + 1) * boxes[i][j].second;
    cout << result << endl;

    return 0;
}

