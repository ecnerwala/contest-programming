#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1500;
const int MAXM = 1500;
int N, M, K;
char G[MAXN][MAXM];

void input() {
    scanf("%d %d %d\n", &N, &M, &K);
    for (int i = 1; i <= N; i++) {
        scanf(" %s ", G[i]+1);
        G[i][0] = G[i][M+1] = '#';
    }
    for (int i = 0; i <= M+1; i++) {
        G[0][i] = G[N+1][i] = '#';
    }
}

void output() {
    for (int i = 1; i <= N; i++) {
        char c = G[i][M+1];
        G[i][M+1] = '\0';
        cout << (G[i]+1) << '\n';
        G[i][M+1] = c;
    }
}

// score * 100
int get_score() {
    int L = 0;
    return min(1000, 1000 * L / K);
}

int main() {
    input();

    for (int i = 1; i <= N; i++) {
    }

    output();
}
