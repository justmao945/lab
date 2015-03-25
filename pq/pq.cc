#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template<class T>
struct MyComapre {
  bool operator()(const T& a, const T& b) const {
    return a > b;
  }
};

int main() {
  vector<int> v ={1,2,3,5,7,8,100,24,5};
  // default is max heap
  priority_queue<int, vector<int>, MyComapre<int> > q;

  for(auto x: v) {
    q.push(x);
  }

  while(! q.empty()) {
    cout << q.top() << endl;
    q.pop();
  }
  return 0;
}
