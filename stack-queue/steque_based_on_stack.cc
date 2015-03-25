#include <cassert>
#include <stack>
#include <iostream>

using namespace std;

class Staque {
public:
    stack<int> p, q;

    bool empty() {
        return p.empty() && q.empty();
    }

    void push(int x) {
        p.push(x);
    }

    void pop() {
        top();
        p.pop();
    }

    int top() {
        if(! p.empty()) {
            return p.top();
        } else {
            assert(! q.empty());
            while(! q.empty()) {
                p.push(q.top());
                q.pop();
            }
            return p.top();
        }
    }

    void enque(int x) {
        q.push(x);
    }

};


int main() {

    Staque k;

    cout << "push ";
    for(int i = 0; i < 5; i++) {
        cout << i << " ";
        k.push(i);
    }
    cout << endl;

    cout << "enque ";
    for(int i = 6; i < 10; i++) {
        cout << i << " ";
        k.enque(i);
    }
    cout << endl;

    cout << "pop ";
    while(! k.empty()) {
        cout << k.top() << " " ;
        k.pop();
    }
    cout << endl;

    return 0;
}
