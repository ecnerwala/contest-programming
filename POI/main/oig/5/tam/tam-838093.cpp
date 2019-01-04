#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e9;

ll ceil(ll a, ll b) {
	return (a + b - 1) / b;
}

const int MAXN = int(5e5) + 200;
int N;

int H[MAXN];
int R[MAXN];

ll solve() {
	ll res = INF;
	{ // Generate bins
		ll totv = 0;
		ll totr = 0;
		int l = 0;
		for (int r = 1; r <= N; r++) {
			assert(r > l);
			if (H[r] >= H[l]) {
				totv += (r - l) * H[l];
				for (int i = l; i < r; i++) {
					totr += R[i];
				}
				if (H[r] != H[l]) {
					totv += (r - l) * H[l];
					for (int i = l; i < r; i++) {
						totr += R[i];
					}
				}
				res = min(res, (totv / totr) + 1);
				if (H[r] == H[l]) {
					totv += (r - l) * H[l];
					for (int i = l; i < r; i++) {
						totr += R[i];
					}
				}
				l = r;
			}
		}
	}
	return res;
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i <= N; i++) scanf("%d", &H[i]);
	for (int i = 0; i < N; i++) scanf("%d", &R[i]);
	ll res = solve();
	reverse(H, H + N + 1);
	reverse(R, R + N);
	res = min(res, solve());
	printf("%lld\n", res);
	return 0;
}
