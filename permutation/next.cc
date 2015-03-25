#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// s is a number, e.g. 12345678
void permuation_next(string& s) {
    if(s.empty() || s.size() == 1) {
        return;
    }

    int i, j;
    // 125432
    //  ^i
    for(i = s.size() - 2; i >= 0; i--) {
        if(s[i] < s[i+1]) {
            break;
        }
    }
    // 125432
    //     ^j
    for(j = s.size() - 1; j > i; j--) {
        if(s[j] > s[i]) {
            break;
        }
    }
    // 135422
    //  ^i ^j  swapped
    if(i >= 0) {
        swap(s[i], s[j]);
    }
    // 132245 reverse from i+1
    //  ^i
    reverse(s.begin() + i + 1, s.end());
}


int main(int argc, char*argv[]) {
    string s1 = argv[1];
    string s2 = s1;
    permuation_next(s1);
    cout << s1 << endl;

    next_permutation(s2.begin(), s2.end());
    cout << s2 << endl;
    return 0;
}
