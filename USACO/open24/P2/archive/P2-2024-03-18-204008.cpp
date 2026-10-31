#include <bits/stdc++.h>

int signum(int a) { return (a > 0) - (a < 0); }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto pneg1 = [](int a) -> int { return (a & 1) ? -1 : 1; };

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a *= 2; }

	std::vector<int> suff_effect(N+2);
	for (int i = N-1; i >= 0; i--) {
		suff_effect[i] = A[i] - suff_effect[i+1];
	}
	auto apply_alternating = [&](int l, int r, int x) -> int {
		return x - signum(x) * (suff_effect[l] - pneg1(l^r) * suff_effect[r]);
	};

	auto find_path = [&](int l, int x) -> int {
		int mi = l - 1, ma = N;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			int64_t pre = apply_alternating(l, md, x);
			if (std::abs(pre) > A[md]) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return ma;
	};

	const int V = A[0] / 2;
	std::vector<int> st_pt(V);
	struct jump_t {
		int dest;
		bool flip;
	};
	struct big_jump_t {
		jump_t small;
		int big_len;
		jump_t big;
	};
	std::vector<big_jump_t> big_jumps(V);
	for (int i = N; i > 0; i--) {
		for (int x = (i == N ? 0 : A[i]) + 1; x < A[i-1]; x += 2) {
			st_pt[x / 2] = i;
			if (i == N) {
				// self-loop
				jump_t small = {x/2, false};
				big_jumps[x / 2] = {small, 0, small};
			} else {
				int j = find_path(i+1, x - A[i]);
				int nx = apply_alternating(i+1, j, x - A[i]);
				jump_t small{std::abs(nx) / 2, x < 0};
				big_jumps[x / 2].small = small;
				cerr << "jump " << x << '@' << i << ' ' << nx << '@' << j << '\n';
				if (big_jumps[small.dest].big_len == big_jumps[big_jumps[small.dest].big.dest].big_len) {
					jump_t big = big_jumps[big_jumps[small.dest].big.dest].big;
					big.flip ^= big_jumps[small.dest].big.flip;
					big.flip ^= small.flip;
					big_jumps[x / 2].big_len = 1 + 2 * big_jumps[small.dest].big_len;
					big_jumps[x / 2].big = big;
				} else {
					big_jumps[x / 2].big_len = 1;
					big_jumps[x / 2].big = small;
				}
			}
		}
	}

	std::vector<int64_t> pref_sum(N+1);
	for (int i = 0; i < N; i++) {
		pref_sum[i+1] = pref_sum[i] + A[i];
	}
	auto solve = [&](int l, int r, int x) -> int {
		cerr << "query " << l << ' ' << r << ' ' << x << '\n';
		assert(l < r);
		{
			int nl = int(std::upper_bound(pref_sum.begin() + l + 1, pref_sum.begin() + r, pref_sum[l] + int64_t(std::abs(x))) - pref_sum.begin());
			assert(l < nl);
			assert(nl <= r);
			x -= signum(x) * int(pref_sum[nl] - pref_sum[l]);
			l = nl;
		}
		{
			int first_bad = find_path(l, x);
			if (first_bad >= r) {
				return apply_alternating(l, r, x);
			}
			x = apply_alternating(l, first_bad, x);
			l = first_bad;
		}
		// Now we do the jump stuff
		while (true) {
			cerr << "at " << l << ' ' << r << ' ' << x << '\n';
			assert(l < r);
			assert(l > 0 && std::abs(x) < A[l-1]);
			assert(std::abs(x) > A[l]);
			assert(st_pt[std::abs(x) / 2] == l);
			auto [small, _, big] = big_jumps[std::abs(x) / 2];
			if (st_pt[big.dest] < r) {
				x = signum(x) * pneg1(big.flip) * (big.dest * 2 + 1);
				l = st_pt[big.dest];
			} else if (st_pt[small.dest] < r) {
				x = signum(x) * pneg1(small.flip) * (small.dest * 2 + 1);
				l = st_pt[small.dest];
			} else {
				return apply_alternating(l + 1, r, x - signum(x) * A[l]);
			}
		}
	};

	int Q; cin >> Q;
	while (Q--) {
		int l, r; int x; cin >> l >> r >> x; l--;
		x *= 2;
		x++;
		int res = solve(l, r, x);
		cout << (res - 1) / 2 << '\n';
	}

	return 0;
}
