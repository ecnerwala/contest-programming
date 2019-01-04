#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int MAXN = int(1e6) + 200;
int N;
int C[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		int a; scanf("%d", &a);
		C[a] ++;
	}
	ll res = 1;
	ll c = 0;
	for (int i = N; i > 0; i--) {
		c += C[i];
		res *= c;
		res %= MOD;
		c--;
	}
	printf("%d\n", int(res % MOD));


	return 0;
}
