#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct frac {
	ll p = 0, q = 1;
	frac() {}
	explicit frac(ll a, ll b = 1) {
		assert(b > 0);
		ll g = __gcd(a,b);
		p = a/g, q = b/g;
	}
	friend bool operator < (const frac& a, const frac& b) {
		return a.p * b.q < b.p * a.q;
	}
};

const int MAXN = 1.1e5;
int N;
pair<ll, ll> pts[MAXN]; // first is max, second is difference
ll pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		ll a, b; cin >> a >> b;
		pts[i] = {max(a, b), b-a};
	}
	sort(pts, pts + N);

	ll sum_better = 0;
	pref[0] = 0;
	for (int i = 0; i < N; i++) {
		sum_better += max(0ll, pts[i].second);
		pref[i+1] = pref[i] + pts[i].first;
	}

	pair<int, frac> ans;
	for (int i = 0; i < N; i++) {
		auto M = pts[i];
		ll cutoff = (sum_better - max(0ll, M.second) + M.second);
		int mi = -1;
		int ma = N+1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			ll lhs = pref[md] - (i < md ? M.first : 0);
			if (lhs <= cutoff) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (mi == -1) continue;
		ll lhs = pref[mi] - (i < mi ? M.first : 0);
		assert(cutoff >= lhs);
		ll B = M.second >= 0 ? M.first : M.first + M.second;
		ans = max(ans, pair<int, frac>(mi - (i < mi), frac{cutoff - lhs, B}));
	}
	frac res{ans.first * ans.second.q + ans.second.p, ans.second.q * N};
	cout << res.p << ' ' << res.q << '\n';

	return 0;
}

// say that you can beat ringo from fraction {t} into of beam M
// M.A + sum_pref A_i <= (1-{t}) * M.B + sum_non_pref (B_i - A_i)
// M.A + sum_pref_worse A_i + sum_pref_better A_i <= (1-{t}) * M.B + sum_suff_better (B_i - A_i)
// M.A + sum_pref_worse A_i + sum_pref_better B_i <= (1-{t}) * M.B + sum_better(B_i - A_i)
// we want to cram as many as possible into pref, so we should take as many until we hit M.B - M.A + sum_better
