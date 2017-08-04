#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000;

int num_lines(int l) { return l * (l-1) / 2 + (l+1); }

typedef pair<int, int> pii;
typedef pii line;
#define M first
#define B second

bool intersect(line a, line b, line c) {
    if (a == b || b == c || c == a) return true;
    if (a.M == b.M || b.M == c.M || c.M == a.M) return false;
    return (a.B - b.B) * (a.M - c.M) == (a.B - c.B) * (a.M - b.M);
}

void go(int N) {
    int L = 0;
    while (num_lines(L) < N) {
        L++;
    }
    int D = num_lines(L) - N;
    vector<int> cnts;
    int tot = 0;
    if (L % 3 == 0 && D == L - 1) {
        if (L == 3) {
            assert(D == 2);
            assert(N == 5);
            cout << 4 << '\n';
            cout << 0 << ' ' << 1 << '\n';
            cout << 0 << ' ' << 2 << '\n';
            cout << 0 << ' ' << 3 << '\n';
            cout << 0 << ' ' << 4 << '\n';
            return;
        } else if (L == 6) {
            assert(D == 5);
            assert(N == 17);
            cout << 6 << '\n';
            cout << 0 << ' ' << 0 << '\n';
            cout << 0 << ' ' << 1 << '\n';
            cout << 0 << ' ' << 2 << '\n';
            cout << 2 << ' ' << 0 << '\n';
            cout << 2 << ' ' << 1 << '\n';
            cout << -1 << ' ' << 1 << '\n';
            return;
        } else if (L >= 9) {
            assert(D >= 8);
            D -= 6;
            cnts.push_back(4);
            tot += 4;
        }
    }
    while (D >= 3) {
        D -= 3;
        cnts.push_back(3);
        tot += 3;
    }
    while (D >= 1) {
        D -= 1;
        cnts.push_back(2);
        tot += 2;
    }
    assert(D == 0);
    assert(tot <= L);
    while (tot < L) {
        cnts.push_back(1);
        tot += 1;
    }
    assert(tot == L);
    vector<pii> res;
    for (int m = 0; m < int(cnts.size()); m++) {
        int cnt = cnts[m];
        pii cur(m, -10000);
        for (int i = 0; i < cnt; i++) {
            while (true) {
                bool good = true;
                for (pii a : res) {
                    for (pii b : res) {
                        if (a == b) continue;
                        if (intersect(a,b,cur)) {
                            good = false;
                            break;
                        }
                    }
                    if(!good) break;
                }
                if(good) break;
                cur.B ++;
            }
            res.push_back(cur);
            cur.B ++;
        }
    }
    assert(int(res.size()) == L);
    cout << L << '\n';
    for (int i = 0; i < L; i++) {
        cout << res[i].M << ' ' << res[i].B << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int N; cin >> N;
        go(N);
    }
}
