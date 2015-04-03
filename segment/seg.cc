#include <iostream>
#include <string>
#include <vector>

using namespace std;

// have an array A[N]
// 1. sum(l, r)  0<=l<=r<N
// 2. A[i] = x, 0<=i<N

// seg: both op can be in O(logN)
// representation:
//  1. leaf nodes are elements of the array A
//  2. each node represents some merging of the leaf nodes,
//  for this problem, the value is the sum
// 
// for each node at index i:


struct Node {
    Node(): left(0),right(0),sum(0) {}
    int left, right;
    int sum;
};

void seg_make(Node tree[], int A[], int root, int left, int right) {
    Node& root_node = tree[root];
    root_node.left = left;
    root_node.right = right;

    if(left == right) {
        root_node.sum = A[left];
        return;
    }

    const int mid = left + (right - left) / 2, left_child = root * 2, right_child = left_child + 1;
    // left child, [left,mid]
    seg_make(tree, A, left_child, left, mid);
    // right child, (mid, right]
    seg_make(tree, A, right_child, mid+1, right);

    root_node.sum = tree[left_child].sum + tree[right_child].sum;
}

void seg_update(Node tree[], int A[], int root, int index, int value) {
    Node& root_node = tree[root];
    if(index  < root_node.left || index > root_node.right) 
        return;

    // restore the sum
    root_node.sum -= A[index];
    // add the sum
    root_node.sum += value;
    // set value in A
    A[index] = value;
    if(root_node.left == root_node.right) {
        return;
    }

    const int mid = root_node.left + (root_node.right - root_node.left) / 2;
    const int left_child = root * 2, right_child = left_child + 1;
    if(index <= mid) {
        seg_update(tree, A, left_child, index, value);
    } else {
        seg_update(tree, A, right_child, index, value);
    }
}

int seg_query(Node tree[], int root, int left, int right) {
    const Node& root_node = tree[root];
    if(root_node.left == left && root_node.right == right) {
        return root_node.sum;
    }
    const int mid = root_node.left + (root_node.right - root_node.left) / 2;
    const int left_child = root * 2, right_child = left_child + 1;
    if(left > mid) {
        // right child
        return seg_query(tree, right_child, left, right);
    } else if(right <= mid) {
        // left child
        return seg_query(tree, left_child, left, right);
    } else {
        // left ~ mid, and mid+1 ~ right
        return seg_query(tree, left_child, left, mid) + seg_query(tree, right_child, mid+1, right);
    }
}

int main() {
    // index 0 is invalid
    int A[] = {0, 1,2,3,4,5,6,7,8,9,10,11,12,13};
    // about 4*N
    const int N = 4 * sizeof(A)/sizeof(int);
    Node tree[N];

    seg_make(tree, A, 1, 1, 9);
    cout << "1~9: " << seg_query(tree, 1, 1, 9) << endl;
    cout << "1~1: " << seg_query(tree, 1, 1, 1) << endl;
    cout << "2~3: " << seg_query(tree, 1, 2, 3) << endl;
    cout << "2~8: " << seg_query(tree, 1, 2, 8) << endl;
    cout << "4~7: " << seg_query(tree, 1, 4, 7) << endl;

    seg_update(tree, A, 1, 5, 50);

    cout << "1~9: " << seg_query(tree, 1, 1, 9) << endl;
    cout << "1~1: " << seg_query(tree, 1, 1, 1) << endl;
    cout << "2~3: " << seg_query(tree, 1, 2, 3) << endl;
    cout << "2~8: " << seg_query(tree, 1, 2, 8) << endl;
    cout << "4~7: " << seg_query(tree, 1, 4, 7) << endl;

    for(int i = 0; i < N; i++) {
        Node& n = tree[i];
        cout << n.left << "," << n.right << ":" << n.sum << " ";
    }
    cout <<  endl;

    return 0;
}
