#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> B(N); for (auto& b : B) { cin >> b; b--; }

	vector<int> prv(N, -1);
	vector<int> prvprv(N, -1);

	struct seg_node {
		int lsum = 0;
		int mlazy = 0;
		int64_t val = 0;
	};
	vector<seg_node> seg(2*N);
	auto update_node = [&](int a) {
		assert(a < N);
		assert(!seg[a].mlazy);
		seg[a].lsum = seg[2*a].lsum + seg[2*a+1].lsum;
		seg[a].val = seg[2*a].val + seg[2*a+1].val;
	};
	auto apply_lazy = [&](int a, int v) {
		seg[a].val += int64_t(seg[a].lsum) * v;
		seg[a].mlazy += v;
	};
	auto downdate_node = [&](int a) {
		assert(a < N);
		if (seg[a].mlazy) {
			apply_lazy(2*a, seg[a].mlazy);
			apply_lazy(2*a+1, seg[a].mlazy);
			seg[a].mlazy = 0;
		}
	};
	auto update_pars = [&](int a) {
		assert(0 <= a && a <= N);
		a += N;
		a >>= __builtin_ctz(a);
		a >>= 1;
		for (; a; a >>= 1) {
			update_node(a);
		}
	};
	auto downdate_pars = [&](int a) {
		assert(0 <= a && a <= N);
		a += N;
		a >>= __builtin_ctz(a);
		a >>= 1;
		if (!a) return;
		for (int l = 31 - __builtin_clz(a); l >= 0; l--) {
			downdate_node(a >> l);
		}
	};

	int64_t ans = 0;
	for (int n = 0; n < N; n++) {
		int p = prv[B[n]];

		// First, answer the query from prv[n]+1..n
		downdate_pars(p+1);
		downdate_pars(n);
		for (int a = p+1+N, b = n+N; a < b; a >>= 1, b >>= 1) {
			if (a & 1) ans += seg[a++].val;
			if (b & 1) ans += seg[--b].val;
		}

		// Now, fix up the previous occurrence of this one
		if (p != -1) {
			// go delete it
			int pp = prvprv[B[n]];
			downdate_pars(pp+1);
			downdate_pars(p);
			downdate_pars(p+1);
			for (int a = pp+1+N, b = p+N; a < b; a >>= 1, b >>= 1) {
				if (a & 1) apply_lazy(a++, -1);
				if (b & 1) apply_lazy(--b, -1);
			}

			seg[p+N].lsum--;
			seg[p+N].val -= seg[p+N].mlazy;

			update_pars(pp+1);
			update_pars(p);
			update_pars(p+1);
		}

		prvprv[B[n]] = p;
		prv[B[n]] = n;
		downdate_pars(p+1);
		downdate_pars(n);
		downdate_pars(n+1);
		for (int a = p+1+N, b = n+N; a < b; a >>= 1, b >>= 1) {
			if (a & 1) apply_lazy(a++, +1);
			if (b & 1) apply_lazy(--b, +1);
		}

		seg[n+N].lsum++;
		seg[n+N].val += seg[n+N].mlazy;

		update_pars(p+1);
		update_pars(n);
		update_pars(n+1);
	}

	cout << ans << '\n';

	return 0;
}

// We fix r, then there are some colors from (prv(r), r)
// For each first-color-loc n, it's an l-candidate at n, and a m-candidate at (prv(n), n)
// We just want to count # l-cands * # m-cands from (prv(r), r)
