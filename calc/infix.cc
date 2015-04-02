#include <iostream>
#include <stack>
#include <vector>
#include <cassert>
#include <cstdlib>

using namespace std;

void calc(stack<char>& ops, stack<int>& vals) {
  assert(! ops.empty());
  char op = ops.top();
  ops.pop();

  assert(vals.size() >= 2);
  int b = vals.top();
  vals.pop();

  int a = vals.top();
  vals.pop();

  int res = 0;
  switch(op) {
    case '+':
      res = a + b;
      break;
    case '-':
      res = a - b;
      break;
    case '*':
      res = a * b;
      break;
    case '/':
      res = a / b;
      break;
    default:
      assert(0 && "bug");
      break;
  }
  vals.push(res);
}

int calc(vector<string>& expr) {
  stack<char> ops;
  stack<int> vals;

  for(auto& e: expr) {
    if(e == "+" || e == "-" || e == "*" || e == "/") {
      ops.push(e[0]);
    } else if(e == "(") {
      // ignore
    } else if(e == ")") {
      calc(ops, vals);
    } else {
      vals.push(atoi(e.c_str()));
    }
  }

  while(! ops.empty()) {
    calc(ops, vals);
  }
  assert(vals.size() == 1);

  return vals.top();
}


int main() {
  vector<string> in = {"3", "*", "(", "4", "+", "5", ")"};
  cout << calc(in) << endl;
  return 0;
}
