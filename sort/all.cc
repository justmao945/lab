#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

void prt(vector<int>& v) {
    for(auto& x: v)
        cout << x  << " ";
    cout << endl;
}

void selection_sort(vector<int>& v) {
    // select the min and put to current position
    for(int i = 0; i < v.size(); i++) {
        int min = i;
        for(int j = i+1; j < v.size(); j++) {
            if(v[j] < v[min]) {
                min = j;
            }
        }
        swap(v[min], v[i]);
    }
}

void insertion_sort(vector<int>& v) {
    // compare to sorted, swap when less than the sorted one
    for(int i = 1; i < v.size(); i++) {
        for(int j = i; j > 0; j--) {
            if(v[j] < v[j-1]) {
                swap(v[j], v[j-1]);
            }
        }
    }
}

int quick_sort_partition(vector<int>& v, int l, int h) {
    // choose l as pivot
    int i = l, j = h, p = v[l];
    while(i < j) {
        // find Vj < p
        while(i < j && v[j] >= p) j--;
        // assign to Vi to put to left, now Vj is empty
        v[i] = v[j];
        // find Vi > p
        while(i < j && v[i] <= p) i++;
        // assign  to Vj to put to right, now Vi is empty
        v[j] = v[i];
    }
    // now i == j, everything is done
    // fix the last empty Vi with p
    v[i] = p;
    return i;
}

void quick_sort_dac(vector<int>& v, int l, int h) {
    if(l >= h) {
        return;
    }
    int m = quick_sort_partition(v, l, h);
    prt(v);
    quick_sort_dac(v, l, m-1);
    quick_sort_dac(v, m+1, h);
}

void quick_sort(vector<int>& v) {
    if(v.empty()) {
        return;
    }
    quick_sort_dac(v, 0, v.size()-1);
}

// when k is 0, means the first
// v is unsorted
// the time complexity is O(N)! better than O(NlogN) and O(NLogK) <- heap
int select_kth_smallest(vector<int>& v, int k) {
    assert(! v.empty());
    int l = 0, h = v.size() - 1;
    while(l <= h) {
        int j = quick_sort_partition(v, l, h);
        if(j == k) { // found!
            return v[k];
        } else if (k < j) {
            h = j-1;
        } else { // k > j
            l = j+1;
        }
    }
    return v[k];
}
 

int main() {
    vector<int> a = {1,2,4,5,7,2,54,2,712,2,4,6,1};
    
    vector<int> b = a, c = a, d = a, e = a;

    selection_sort(b);
    prt(b);

    insertion_sort(c);
    prt(c);

    prt(d);
    quick_sort(d);
    prt(d);

    cout << "5th: " << select_kth_smallest(e, 5) << endl;
    prt(e);

    return 0;
}
