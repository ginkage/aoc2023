#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

const char *digit[] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int main() {
    long result = 0;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        long d1 = -1, d2 = -1;
        const char *line = s.c_str();
        for (int i = 0; line[i]; i++) {
            for (int j = 0; j < 20; j++) {
                if (!memcmp(line + i, digit[j], strlen(digit[j]))) {
                    if (d1 >= 0)
                        d2 = j % 10;
                    else
                        d1 = d2 = j % 10;

                    break;
                }
            }
        }

        if (d1 >= 0)
            result += 10 * d1 + d2;
    }

    cout << result << endl;

    return 0;
}
