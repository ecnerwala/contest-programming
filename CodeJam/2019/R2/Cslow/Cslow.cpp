#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct frac {
	ll n, d;
	frac(ll n_, ll d_) : n(n_), d(d_) {
		//assert(n >= 0 && d >= 0);
	}

	friend bool operator < (const frac& a, const frac& b) {
		return a.n * b.d < b.n * a.d;
	}
	friend bool operator > (const frac& a, const frac& b) {
		return a.n * b.d > b.n * a.d;
	}
	friend bool operator >= (const frac& a, const frac& b) {
		return a.n * b.d >= b.n * a.d;
	}
	friend bool operator <= (const frac& a, const frac& b) {
		return a.n * b.d <= b.n * a.d;
	}
};

const int MAXN = 1.1e5;
int N;
ll C[MAXN];
ll J[MAXN];
frac go() {
	cin >> N;
	for (int i = 0; i < N; i++) { cin >> C[i] >> J[i]; }

	frac mi = frac(0, 1);
	frac ma = frac(1, 0);

	for (int i = 0; i+1 < N; i++) {
		ll dc = C[i+1] - C[i];
		ll dj = J[i+1] - J[i];
		ll g = __gcd(abs(dj), abs(dc));
		if (g) {
			dc /= g;
			dj /= g;
		}

		if (dc <= 0 && dj <= 0) return frac(0, 0);
		else if (dc >= 0 && dj >= 0) continue;
		else if (dc > 0 && dj < 0) {
			mi = max(mi, frac(-dj, dc));
		} else if (dc < 0 && dj > 0) {
			ma = min(ma, frac(dj, -dc));
		} else assert(false);
	}
	if (!(mi < ma)) return frac(0, 0);
	//cerr << mi.n << '/' << mi.d << ' ' << ma.n << '/' << ma.d << '\n';
	frac lo(0, 1);
	frac hi(1, 0);
	while (true) {
		frac md(lo.n + hi.n, lo.d + hi.d);
		if (md <= mi) {
			lo = md;
		} else if (md >= ma) {
			hi = md;
		} else {
			return md;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		frac res = go();

		cout << "Case #" << case_num << ": ";
		if (res.n == 0 && res.d == 0) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			cout << res.n << ' ' << res.d << '\n';
		}
	}

	return 0;
}
