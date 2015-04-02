
#include <iostream>

using namespace std;

void swap(int& a, int& b) {
    if(&a == &b) return;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

void swap(int *a, int *b) {
    // when a == b, both *a and *b changed...
    if(a == b) return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}


int main() {
    int a = 1, b = 2, c = 3;
    
    cout << a  << " ";
    swap(&a, &a);
    cout << a << endl;
    
    cout << c  << " ";
    swap(c, c);
    cout << c << endl;
    
    int d = 2;
    cout << b << "," << d  << " ";
    swap(&b, &d);
    cout << b << "," << d  << endl;
    

    return 0;
}
