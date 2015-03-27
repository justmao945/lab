#include <iostream>
#include <vector>

using namespace std;

//  min queue, index 1 is the top
//  0 1 2 3 4 5 6
//           
// a min queue
class PQ {
public:
    PQ() {
        // 0 is not used
        v.resize(1);
    }

    PQ(const int* A, int n) {
        v.resize(n+1);
    }

    int top() {
        return v[1];
    }

    void push(int x) {
        // bottom-up, swim up v.back
        v.push_back(x);
        int i = v.size() -1;
        int j;
        while(i > 1) {
            j = i/2;
            // current v[i] less then parent v[j], swap
            if(v[i] < v[j]) {
                swap(v[i], v[j]);
            } else {
                break;
            }
            i = j;
        }
    }

    void pop() {
        // top down, swap root and v.back, then sink root
        swap(v[1], v.back());
        v.pop_back();
        int i = 1;
        int j;
        // if current v[i] has no left child, then nonthing to do
        while(i*2 < v.size()) {
            // left child
            j = i * 2;
            // try right child if right child is smaller
            if(j + 1 < v.size() && v[j+1] < v[j]) j++;
            // swap if root v[i] is bigger than the smaller child v[j], else done
            if(v[i] > v[j]) {
                swap(v[i], v[j]);
            } else {
                break;
            }
            i = j;
        }
    }

    bool empty() {
        return size() == 0;
    }

    int size() {
        return v.size() - 1;
    }
private:
    // the last is top
    vector<int> v;
};

int main() {

    vector<int> v = {10, 2, 3, 5, 8, 2, 2, -1, 100, 89};

    PQ q;
    for(auto x: v) {
        q.push(x);
    }
    while(! q.empty()) {
        cout << q.top() << endl;
        q.pop();
    }
    return 0;
}
