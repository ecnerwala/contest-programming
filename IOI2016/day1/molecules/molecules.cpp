#include "molecules.h"
#include<algorithm>
#include<vector>
#include<cassert>
using namespace std;

typedef long long ll;

const vector<int> BAD;

vector<int> find_subset(int L, int U, vector<int> W) {
    int N = int(W.size());
    vector<pair<ll, int>> M(N);
    for(int i = 0; i < N; i++) {
        M[i] = pair<ll, int>(W[i], i);
    }
    sort(M.begin(), M.end());
    vector<ll> P(N+1);
    vector<ll> S(N+1);
    P[0] = 0;
    S[0] = 0;
    for(int i = 0; i < N; i++) {
        P[i + 1] = P[i] + M[i].first;
        S[i + 1] = S[i] + M[N-1 - i].first;
    }
    for(int i = 1; i <= N; i++) {
        if(P[i] <= U && S[i] >= L) {
            for(int j = 0; j <= i; j++) {
                ll v = P[j] + S[i - j];
                if(L <= v && v <= U) {
                    vector<int> res;
                    for(int k = 0; k < j; k++) {
                        res.push_back(M[k].second);
                    }
                    for(int k = 0; k < i-j; k++) {
                        res.push_back(M[N-1-k].second);
                    }
                    return res;
                }
            }
            assert(false);
        }
    }
    return BAD;
}