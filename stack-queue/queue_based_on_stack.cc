#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

// implement queue based on 2 stacks
class Queue {
public:
    stack<int> _push, _pop;
    
    bool empty() {
        return _push.empty() && _pop.empty();
    }

    void push(int x) {
        _push.push(x);
    }

    int front() {
        if(! _pop.empty()) {
            return _pop.top();
        } else {
            while(!_push.empty()) {
                _pop.push(_push.top());
                _push.pop();
            }
            assert(! _pop.empty());
            return _pop.top();
        }
    }

    void pop() {
        front();
        _pop.pop();
    }
};


int main() {
    Queue q;
    for(int i = 0; i < 10; i++) {
        cout << i << " ";
        q.push(i);
    }
    cout << endl;

    while(! q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}
