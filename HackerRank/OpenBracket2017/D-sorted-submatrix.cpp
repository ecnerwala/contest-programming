#include<bits/stdc++.h>
using namespace std;

int N, M;
vector<vector<int>> A;

void wssert(bool b) {
    if (!b) {
        cout << "WSSERT" << '\n';
        exit(0);
    }
}

struct hist {
    vector<pair<int, int>> vec;
    long long cnt;
    hist() {
        vec.clear();
        vec.emplace_back(-1, -1);
        cnt = 0;
    }
    void rem() {
        pair<int, int> last = vec.back();
        vec.pop_back();
        wssert(!vec.empty());
        wssert(last.second > vec.back().second);
        cnt -= last.first * (last.second - vec.back().second);
    }
    void add(pair<int, int> next) {
        wssert(next.second > vec.back().second);
        wssert(next.first >= 0);
        wssert(!vec.empty());
        while (next.first <= vec.back().first) {
            rem();
        }
        assert(next.first > vec.back().first);
        assert(next.second >= vec.back().second);
        cnt += next.first * (next.second - vec.back().second);
        vec.push_back(next);
    }
};

int main() {
    scanf("%d %d", &N, &M);
    A = vector<vector<int>>(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", &A[i][j]);
        }
    }
    vector<int> stp(N, 0);
    long long tot = 0;
    for (int col = 0; col < M; col++) {
        hist h;
        long long cur = 0;
        for (int i = 0; i < N; i++) {
            if (col && A[i][col] < A[i][col-1]) {
                stp[i] = col;
            }
            // first add the binary search constraint
            if (i) {
                auto beg = A[i].begin() + stp[i], end = A[i].begin() + col + 1;
                int ind = lower_bound(beg, end, A[i-1][col]) - A[i].begin();
                assert(ind <= col + 1);
                if (ind > stp[i-1]) {
                    assert(h.vec.back().second == i-1);
                    assert(h.vec.back().first == (col - stp[i-1] + 1));
                    h.rem();
                    h.add(pair<int, int>(col - ind + 1, i-1));
                }
            }
            h.add(pair<int, int>(col - stp[i] + 1, i));
            tot += h.cnt;
        }
    }
    cout << tot << '\n';
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    return 0;
}
