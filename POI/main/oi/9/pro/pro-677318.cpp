#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXK = 20;
const int MAXN = 2e3;
const int MAXM = 20000;
const int MAXL = MAXM;

ll K, N, M, L;

struct num {
	ld val;
	num(ld a = 1) : val(log(a) / log(ld(2))) {}
};

ld log(num a) {
	return a.val;
}

num operator + (num a, num b) {
	if(a.val > b.val) swap(a, b);
	// a.val <= b.val
	num res(pow(2., a.val - b.val) + 1);
	res.val += b.val;
	assert(abs(-pow(2., res.val) + pow(2., a.val) + pow(2., b.val)) <= 1e-9);
	return res;
}

num operator * (num a, num v) {
	num res;
	res.val = a.val + v.val;
	return res;
}

num dp[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> K >> N >> M >> L;
	assert(N % M == 0);

	dp[0] = K;

	for(int i = 1; i < M; i++) {
		dp[i] = dp[i - 1] * (K - 1);
		for(int l = 2; l < L && i - l >= 0; l++) {
			dp[i] = dp[i] + dp[i - l] * (K - 1);
		}
	}

	num cnt = dp[M - 1];
	assert(1 < L);
	for(int l = 2; l < L && M - l >= 0; l++) {
		cnt = cnt + dp[M - l];
	}

	ld res = log(cnt);

	res *= N / M;

	cout << ll(res + 1e-9) << '\n';
}
