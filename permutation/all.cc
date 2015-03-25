#include <iostream>
#include <string>
#include <vector>

using namespace std;

// @j do start from j
void dfs(vector<string>& f, string& e, string& s, int j) {
    if(e.size() == s.size()) {
        f.push_back(e);
        return;
    }
    for(int i = j; i < s.length(); i++) {
        // pin i
        swap(s[j], s[i]);
        e.push_back(s[j]);
        // do start from j+1
        dfs(f, e, s, j+1);
        e.pop_back();
        // backtracking
        swap(s[j], s[i]);
    }
}

// no dup in s
void permutation(vector<string>& res, string& s) {
    string e;
    dfs(res, e, s, 0);
}

int main(int argc, char* argv[]) {
    
    vector<string> res;
    string s = argv[1];

    permutation(res, s);

    for(auto& x: res) {
        cout << x << endl;
    }

    return 0;
}
