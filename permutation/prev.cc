#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void permutation_prev(string& s) {
    if(s.empty() || s.size() == 1) {
        return;
    }
    
    int i, j;
    // 132245
    //  ^i
    for(i = s.length() - 2; i >=0; i--) {
        if(s[i] > s[i+1]) {
            break;
        }
    }
    // 132245
    //    ^j
    for(j = s.length() - 1; j > i; j--) {
        if(s[j] < s[i]) {
            break;
        }
    }
    // 122345
    //  ^i^j swapped
    if(i >= 0) {
        swap(s[i], s[j]);
    }
    reverse(s.begin() + i + 1, s.end());
}

int main(int argc, char* argv[]) {
    string s1 = argv[1];
    string s2 = s1;

    permutation_prev(s1);
    cout << s1 << endl;

    prev_permutation(s2.begin(), s2.end());
    cout << s2 << endl;

    return 0;
}
