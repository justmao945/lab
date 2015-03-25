#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

class UFS {
public:

    // N sites with int from 0 to N-1
    UFS(int N) {
        // data[i] means the parent index
        // data[i] == i means a standalone components
        data.resize(N);
        for(int i = 0; i < N; i++) {
            data[i] = i;
        }
    }

    // add connection p <=> q
    void Union(int p, int q) {
        int i = Find(p), j = Find(q);
        data[i] = j;
    }

    // find component id
    int Find(int p) {
        assert(p >= 0 && p < data.size());
        while(p != data[p]) {
            p = data[p];
        }
        return p;
    }

    // true if p and q are in the same  component
    bool Connected(int p, int q) {
        return p == q || Find(p) == Find(q);
    }

    // number of components
    int Count() {
        int n = 0;
        for(int i = 0; i < data.size(); i++) {
            if(data[i] == i) {
                n++;
            }
        }
        return n;
    }

private:
    vector<int> data;
};


int main() {
    UFS u(10);
    u.Union(4, 3);
    u.Union(3, 8);
    u.Union(6, 5);
    u.Union(9, 4);
    u.Union(2, 1);
    u.Union(8, 9);
    u.Union(5, 0);
    u.Union(7, 2);
    u.Union(6, 1);
    u.Union(1, 0);
    u.Union(6, 7);

    cout << boolalpha;
    
    cout << "Have " << u.Count() << " components" << endl;

    cout << "Node => Component" << endl;
    for(int i = 0; i < 10; i++) {
        cout << i << " => " << u.Find(i) << endl;
    }

    cout << "Node, Node => Connected " << endl;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            cout << i << ", " << j << " => " << u.Connected(i, j) << endl;
        }
    }
    return 0;
}
