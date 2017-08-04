#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second

typedef double ld;

const int MAXN = int(1e5) + 200;
int N, M;

pll P[MAXN];

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%lld %lld", &P[i].A, &P[i].B);
    }

    ld R = go(); 
    printf("%.7f", R);
    for (int i = 0; i < N; i++) {
    }

}
