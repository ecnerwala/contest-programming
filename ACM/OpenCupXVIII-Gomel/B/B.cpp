#include<bits/stdc++.h>
using namespace std;

using ll = long long;
ll tri(ll a) {
	if (a % 2 == 0) return a / 2 * (a+1);
	else return (a+1) / 2 * a;
}

ll minN(ll C) {
	if (!C) return 0;
	return tri(C) + 2 * minN(C / 2);
}

ll go(ll N) {
	assert(N);
	ll mi = 0;
	ll ma = 2e9; // this is just big enough, doesn't overflow tho
	assert(minN(ma) > N);
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (minN(md) <= N) {
			mi = md;
		} else {
			ma = md;
		}
	}
	ll n_mi = minN(mi);
	ll n_ma = minN(ma);
	//cerr << mi << ' ' << ma << '\n';
	//cerr << n_mi << ' ' << N << ' ' << n_ma << '\n';
	assert(n_mi <= N && N < n_ma);
	ll res = 0;
	for (ll v = mi; v; v /= 2) {
		res += v;
	}
	while (n_mi + ma <= N) {
		n_mi += ma;
		res++;
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		ll N; cin >> N;
		cout << go(N) << '\n';
	}

	return 0;
}

// we're trying to pick a sequence of powers of 2
// you want as much assigned to value 1 as possible
// for each power of 2^p, having k copies costs 2^p k * (k+1)/2
// we just have marginal costs 1, 2, 3, 4, 5, 6,
// and costs 2, 4, 6, 8, 10
// and costs 4, 8, 16
//
// if max marginal cost is C, then it's triangle(C) + triangle(C // 2) + triangle(C//4) ...
