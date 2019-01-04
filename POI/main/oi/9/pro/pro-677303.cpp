#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXK = 20;
const int MAXN = 2e3;
const int MAXM = 20;
const int MAXL = MAXM;

ll K, N, M, L;

struct num {
	ld val; ll base;
	num(ld a = 0, ll b = 0) : val(a), base(b) {
		while(val >= 2) val /= 2, base ++;
	}
};

ld log(num a) {
	return log(a.val) / log(ld(2)) + a.base;
}

num operator + (num a, num b) {
	if(a.base > b.base) swap(a, b);
	// a.base <= b.base
	while(a.base < b.base) {
		a.val /= 2;
		a.base++;
	}
	assert(a.base == b.base);
	return num(a.val + b.val, b.base);
}

num operator * (num a, ld v) {
	return num(a.val * v, a.base);
}

num dp[MAXM][MAXL];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> K >> N >> M >> L;
	assert(N % M == 0);

	dp[1][1] = K;

	for(int i = 1; i < M; i++) {
		for(int l = 1; l < L; l++) {
			dp[i + 1][1] = dp[i + 1][1] + dp[i][l] * (K - 1);
			dp[i + 1][l + 1] = dp[i][l];
		}
	}

	num cnt = 0;
	for(int l = 1; l < L; l++) {
		cnt = cnt + dp[M][l];
	}

	ld res = log(cnt);

	res *= N / M;

	cout << ll(res + 1e-9) << '\n';
}
