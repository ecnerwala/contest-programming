#include<bits/stdc++.h>
using namespace std;

template <typename T> int signum(T v) { return (v > 0) - (v < 0); }

using ld = long double;
using ll = long long;

const ll MAXROOT = 1.1e9;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	map<ll, int> cnts;
	for (int i = 0; i < N; i++) {
		ll A, B, C, D; cin >> A >> B >> C >> D;
		assert(A);
		if (A < 0) {
			A = -A, B = -B, C = -C, D = -D;
		}
		assert(A > 0);
		//cerr << "start " << A << ' ' << B << ' ' << C << ' ' << D << '\n';
		set<ll> roots;

		auto signEval = [&](ll v) {
			//cerr << v << '\n';
			if (v == 0) return signum(D);
			ll a = A, b = B, c = C, d = D;
			c += d / v;
			d %= v;
			b += c / v;
			c %= v;
			a += b / v;
			b %= v;

			if (a) return signum(v) * signum(a);
			else if (b) return signum(b);
			else if (c) return signum(v) * signum(c);
			else if (d) return signum(d);
			return 0;
		};

		// inclusive range to check
		auto bsearch = [&](ld lo, ld hi, bool incr) {
			//cerr << "bsearch " << ' ' << lo << ' ' << hi << ' ' << incr << '\n';
			lo = max(lo, ld(-MAXROOT));
			hi = min(hi, ld(MAXROOT));
			ll mi = ll(ceil(lo)), ma = ll(floor(hi));
			//cerr << mi << ' ' << ma << '\n';
			for (int z = -3; z <= 3; z++) {
				if (signEval(mi+z) == 0) roots.insert(mi+z);
				if (signEval(ma+z) == 0) roots.insert(ma+z);
			}
			mi += 3;
			ma -= 3;
			while (ma - mi > 1) {
				ll md = mi + (ma - mi) / 2;
				ll v = signEval(md);
				if (v == 0) {
					roots.insert(md);
					return;
				} else if ((v > 0) == (incr)) {
					ma = md;
				} else {
					mi = md;
				}
			}
		};

		ll da = 3 * A, db = 2 * B, dc = C;
		ld ddisc = db * db - ld(4) * da * dc;
		if (ddisc >= 0) {
			ld rt = sqrt(ld(ddisc));
			ld lo = (-db - rt) / 2 / da;
			ld hi = (-db + rt) / 2 / da;
			bsearch(-MAXROOT, lo, true);
			bsearch(lo, hi, false);
			bsearch(hi, MAXROOT, true);
		} else {
			// binary search over the whole thing
			bsearch(-MAXROOT, MAXROOT, true);
		}

		for (ll root : roots) {
			//cerr << A << ' ' << B << ' ' << C << ' ' << D << ' ' << root << '\n';
			cnts[root]++;
			auto isDouble = [&]() {
				if (root == 0) return C == 0;
				ll v = C;
				if (v % root) return false;
				v /= root;
				v += 2 * B;
				if (v % root) return false;
				v /= root;
				v += 3 * A;
				return v == 0;
			};
			if (isDouble()) {
				cnts[root]++;
				auto isTriple = [&]() {
					if (root == 0) return B == 0;
					ll v = 2 * B;
					if (v % root) return false;
					v /= root;
					v += 6 * A;
					return v == 0;
				};
				if (isTriple()) {
					cnts[root]++;
				}
			}
		}
	}

	if (cnts.empty()) {
		cout << "NO" << '\n';
	} else {
		cout << "YES" << '\n';
		pair<int, ll> best(0, 0);
		for (const auto& it : cnts) {
			best = max(best, {it.second, it.first});
		}
		cout << best.second << ' ' << best.first << '\n';
	}
	return 0;
}
