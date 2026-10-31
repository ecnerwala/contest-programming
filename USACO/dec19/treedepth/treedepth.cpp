#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 310;

int N, K, MOD;
int L;
int poly[MAXN * MAXN];

void mul(int i) {
	// multiply by 1-x^i
	for (int j = L+1; j <= L+i; j++) {
		poly[j] = 0;
	}
	for (int j = L; j >= 0; j--) {
		poly[j+i] -= poly[j];
		if (poly[j+i] < 0) poly[j+i] += MOD;
	}
	L += i;
}

void div(int i) {
	// divide by 1-x^i
	for (int j = 0; j+i <= L; j++) {
		poly[j+i] += poly[j];
		if (poly[j+i] >= MOD) poly[j+i] -= MOD;
	}
	for (int j = L-i+1; j <= L; j++) {
		assert(poly[j] == 0);
	}
	L -= i;
}

int ans[MAXN];
int main() {
	if(fopen("treedepth.in", "r")) {
		freopen("treedepth.in", "r", stdin);
		freopen("treedepth.out", "w", stdout);
	}
	cin >> N >> K >> MOD;

	L = 0;
	poly[0] = 1;
	for (int i = 0; i < N; i++) {
		// multiply by (1 - x^{i+1}) / (1 - x)
		mul(i+1);
		div(1);
	}

	for (int i = 0; i < N; i++) {
		mul(1); div(i+1);

		if (0 <= K && K <= L) {
			// precededs by i
			for (int j = i; j < N; j++) {
				ans[j] += poly[K];
				if (ans[j] >= MOD) ans[j] -= MOD;
			}
		}
		if (i && 0 <= K-i && K-i <= L) {
			// succeeds by i
			for (int j = 0; j+i < N; j++) {
				ans[j] += poly[K-i];
				if (ans[j] >= MOD) ans[j] -= MOD;
			}
		}

		mul(i+1); div(1);
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
