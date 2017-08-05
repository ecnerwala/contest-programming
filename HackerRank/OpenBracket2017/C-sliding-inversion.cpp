#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int MAXN = 5e5;
struct seg{
    static const int L = 19;
    static const int S = 1 << L;
    int arr[2 * S];
    
    // inclusive
    int query(int a, int b) {
        int res = 0;
        for (int i =a+S, j = b+S; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) {
                res += arr[i];
                i++;
            }
            if (j % 2 == 0) {
                res += arr[j];
                j --;
            }
        }
        return res;
    }
    
    void update(int a, int v) {
        for (int i = a+S; i; i /= 2) {
            arr[i] += v;
        }
    }
    
};


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int N, M;
    vector<int> A;
    vector<int> vals;
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; i++) {
        int v; scanf("%d", &v);
        A.push_back(v);
        vals.push_back(v);
    }
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
    for (int i = 0; i < N; i++) {
        A[i] = lower_bound(vals.begin(), vals.end(), A[i]) - vals.begin();
    }
    seg s;
    long long res = 0;
    long long cnt = 0;
    for (int i = 0; i < N; i++) {
        if (i >= M) {
            // remove
            int j = i - M;
            cnt -= s.query(0,A[j]-1);
            s.update(A[j], -1);
        }
        cnt += s.query(A[i]+1,N+1);
        s.update(A[i], 1);
        if (i >= M-1) res += cnt;
    }
    cout << res << '\n';
    return 0;
}
