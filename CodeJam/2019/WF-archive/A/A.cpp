#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int Nmax;
ll M;
int R;

void go() {
	// x is truex + truey, y is truex - truey
	// return the l1 distance, which is twice the l_infty distance
	auto queryDist = [](ll x, ll y) -> ll {
		assert((x & 1) == (y & 1));
		cout << (x + y) / 2 << ' ' << (x - y) / 2 << '\n' << flush;
		string s; cin >> s;
		if (s == "ERROR") {
			exit(0);
		}
		return stoll(s) * 2;
	};

	ll d0 = queryDist(2 * M, 2 * M);
	ll d1 = queryDist(2 * M + 1, 2 * M + 1);
	int N = int((d1 - d0) / 2); // true N

	// binary search along each axis
	vector<ll> xs[2];
	for (int z = 0; z < 2; z++) {
		auto queryX = [&](ll x) -> ll {
			ll y, c;
			if (x & 1) {
				y = 2 * M + 1, c = -N;
			} else {
				y = 2 * M, c = 0;
			}
			if (z) swap(x, y);
			return queryDist(x, y) + c;
		};

		for (int i = 0; i < N; i++) {
			int slope = N - 1 - i * 2;
			ll ma = 2 * M + 1;
			ll mi = -(2 * M + 1);
			while (ma - mi > 1) {
				ll md = mi + (ma - mi) / 2;
				if (queryX(md + 1) - queryX(md) >= slope) {
					ma = md;
				} else {
					mi = md;
				}
			}
			xs[z].push_back(ma);
		}
	}

	cout << "READY" << '\n' << flush;

	while (true) {
		string s; cin >> s;
		if (s == "DONE") break;
		if (s == "ERROR") exit(0);
		ll x = stoll(s);
		cin >> s;
		ll y = stoll(s);
		ll ans = 0;
		for (auto it : xs[0]) {
			ans += abs(x+y - it);
		}
		for (auto it : xs[1]) {
			ans += abs(x-y - it);
		}
		ans /= 2;
		cout << ans << '\n' << flush;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T >> Nmax >> M >> R;

	for (int case_num = 1; case_num <= T; case_num ++) {
		go();
	}

	return 0;
}
