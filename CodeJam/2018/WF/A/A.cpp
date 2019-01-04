#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using pll = pair<ll, ll>;

pll R;
const int MAXS = 16;
int S;

pair<pll, ll> stations[MAXS];

ll cnt[1<<MAXS];

ll go() {
	for (int m = 1; m < (1 << S); m++) {
		ll xmi = 1;
		ll xma = R.first;
		ll ymi = 1;
		ll yma = R.second;
		for (int i = 0; i < S; i++) {
			if (m & (1 << i)) {
				xmi = max(xmi, (stations[i].first.first - stations[i].second));
				xma = min(xma, (stations[i].first.first + stations[i].second));
				ymi = max(ymi, (stations[i].first.second - stations[i].second));
				yma = min(yma, (stations[i].first.second + stations[i].second));
			}
		}
		if (xmi > xma || ymi > yma) {
			cnt[m] = 0;
			continue;
		}
		cnt[m] = (xma - xmi + 1) * (yma - ymi + 1);
		for (int i = 0; i < S; i++) {
			if (
					xmi <= stations[i].first.first && stations[i].first.first <= xma
					&& ymi <= stations[i].first.second && stations[i].first.second <= yma) {
				cnt[m] --;
			}
		}
		assert(cnt[m] >= 0);
	}
	for (int i = 0; i < S; i++) {
		for (int m = 0; m < (1 << S); m++) {
			if (!(m & (1 << i))) {
				cnt[m] -= cnt[m | (1 << i)];
			}
		}
	}
	cnt[0] = 0;
	for (int m = 0; m < (1 << S); m++) {
		cerr << m << ' ' << cnt[m] << '\n';
	}
	for (int i = 0; i < S; i++) {
		for (int m = 0; m < (1 << S); m++) {
			if (!(m & (1 << i))) {
				cnt[m | (1 << i)] += cnt[m];
			}
		}
	}
	ll tot = cnt[(1<<S)-1];
	ll L = tot;
	ll U = 0;
	for (int m = 1; m < (1 << S); m++) {
		int ppc = __builtin_popcount(unsigned(m));
		// lower bound is maximum L such that L*popcount(m) <= sum(at least one of m)
		// lower bound is maximum L such that L*popcount(m) <= tot - sum(<= ~m)
		// upper bound is minimum U such that U*popcount(m) >= sum(<= m)
		L = min(L, (tot - cnt[((1<<S)-1)^m]) / ppc);
		U = max(U, (cnt[m] + ppc - 1) / ppc);
	}
	cerr << L << ' ' << U << '\n';
	cerr << '\n';
	return U - L;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> R.first >> R.second >> S;
		for (int i = 0; i < S; i++) {
			cin >> stations[i].first.first >> stations[i].first.second >> stations[i].second;
		}

		ll res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
