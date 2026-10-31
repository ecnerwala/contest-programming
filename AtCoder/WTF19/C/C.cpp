#include<bits/stdc++.h>
using namespace std;

#define SUBTASK 2

using ll = long long;

const ll XMAX = 1.1e17;

const int MAXN = 1.1e5;
int N;
pair<ll, ll> P[MAXN];

bool checkPt(ll Y) {
	bool res = 0;
	for (int i = 0; i < N; i++) {
		res ^= (((Y - P[i].second) | P[i].first) == P[i].first);
	}
	return res;
}

pair<ll, ll> ptToBounds(ll Y) {
	assert(checkPt(Y));
	ll lo = Y, hi = Y;
	for (int l = 59; l >= 0; l--) {
		if (checkPt(hi + (1ll << l))) {
			hi += (1ll << l);
		}
		if (checkPt(lo - (1ll << l))) {
			lo -= (1ll << l);
		}
	}
	return {hi - lo, lo};
}

pair<ll, ll> goSub1() {
	return ptToBounds(XMAX);
}

bool checkRng(ll Ymin, ll v3) {
	bool res = 0;
	for (int i = 0; i < N; i++) {
		ll curMin = max(0ll, Ymin - P[i].second);

		// must be strictly greater than curMin
		bool dp[3] = {0,0,0};

		for (int l = 59; l >= 0; l--) {
			if (!(P[i].first & (1ll << l))) continue; // must be 0

			{
				bool ndp[3] = {0,0,0};
				for (int z = 0; z < 3; z++) {
					ndp[z] = dp[z];
				}
				for (int z = 0; z < 3; z++) {
					ndp[(z+(1ll << l)) % 3] ^= dp[z];
				}
				for (int z = 0; z < 3; z++) {
					dp[z] = ndp[z];
				}
			}

			if (!(curMin & (1ll << l))) {
				// we can increment and be bigger
				ll val = ((curMin >> l) | 1) << l;
				if ((val | P[i].first) == P[i].first) {
					dp[(val + P[i].second) % 3] ^= 1;
				}
			}
		}

		if ((curMin | P[i].first) == P[i].first) {
			dp[(curMin + P[i].second) % 3] ^= 1;
		}

		res ^= dp[v3];
	}
	return res;
}

pair<ll, ll> go() {
	ll v3 = 0;
	while (!checkRng(0, v3)) {
		v3++;
		assert(v3 < 3);
	}

	ll mi = 0;
	ll ma = ll(1e18);
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (checkRng(md, v3)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	return ptToBounds(mi);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> P[i].first >> P[i].second;
		P[i].first += XMAX;
		P[i].second += XMAX;
		assert(P[i].first >= 0);
		assert(P[i].second >= 0);
	}

#if SUBTASK == 1
	auto ans = goSub1();
#else
	auto ans = go();
#endif
	ans.first -= XMAX;
	ans.second -= XMAX;

#if SUBTASK == 1
	cout << ans.first << '\n';
#else
	cout << ans.first << ' ' << ans.second << '\n';
#endif

	return 0;
}
