#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 400;
const int MAXK = 1000;
const int MAXM = 200;

int N, K, M[MAXN];
ll P;

ll dp[MAXN][MAXK];
ll dp2[MAXN][MAXK];

int lastT = 0;

void insert(int m) {
    memset(dp2, 0, sizeof(dp2));
    int dt = m - lastT; lastT = m;
    for(int h = 0; h <= N / 2; h++) {
        for(int k = 0; k <= K; k++) {
            if(!dp[h][k]) continue;
            int k2 = k + h * dt;
            if(k2 > K) continue;
            dp2[h + 1][k2] += dp[h][k];
            dp2[h + 1][k2] %= P;
            if(h >= 1)  {
                dp2[h - 1][k2] += dp[h][k] * h;
                dp2[h - 1][k2] %= P;
            }
            dp2[h][k2] += dp[h][k] * (h + 1);
            dp2[h][k2] %= P;
        }
    }
    memcpy(dp, dp2, sizeof(dp));
}

int main() {
    scanf("%d%d%lld", &N, &K, &P);
    for(int i = 0; i < N; i++) {
        scanf("%d", &M[i]);
    }

    sort(M, M + N);

    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;
    
    for(int i = 0; i < N; i++) insert(M[i]);

    ll out = 0;
    for(int i = 0; i <= K; i++) {
        out += dp[0][i];
        out %= P;
    }
    printf("%lld\n", out);
    return 0;
}
