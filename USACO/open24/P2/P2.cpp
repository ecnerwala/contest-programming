#include <bits/stdc++.h>

int signum(int a) { return (a > 0) - (a < 0); }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto pneg1 = [](int a) -> int { return (a & 1) ? -1 : 1; };

	std::mt19937 mt(48);
	constexpr bool TESTING = true;

	for (int z = 0; z < 100; z++) {
		cerr << "case " << z << '\n';

	int N;
	if (TESTING) {
		N = std::uniform_int_distribution(1, 10)(mt);
		cerr << N << '\n';
	} else {
		cin >> N;
	}
	std::vector<int> A(N);
	if (TESTING) {
		for (auto& a : A) {
			a = std::uniform_int_distribution(0, 10)(mt); a *= 2;
		}
		std::sort(A.begin(), A.end(), std::greater<>());
		for (auto a : A) { cerr << (a/2) << ' '; } cerr << '\n';
	} else {
		for (auto& a : A) { cin >> a; a *= 2; }
	}

	std::vector<int64_t> pref_sum(N+1);
	for (int i = 0; i < N; i++) {
		pref_sum[i+1] = pref_sum[i] + A[i];
	}

	std::vector<int> suff_effect(N+1);
	for (int i = N-1; i >= 0; i--) {
		suff_effect[i] = A[i] - suff_effect[i+1];
	}
	auto apply_alternating = [&](int l, int r, int x) -> int {
		return x - signum(x) * (suff_effect[l] - pneg1(l^r) * suff_effect[r]);
	};

	auto find_bad = [&](int l, int r, int x) -> std::pair<int, int> {
		{
			int nl = int(std::upper_bound(pref_sum.begin() + l + 1, pref_sum.begin() + r, pref_sum[l] + int64_t(std::abs(x))) - pref_sum.begin());
			assert(l < nl);
			assert(nl <= r);
			x -= signum(x) * int(pref_sum[nl] - pref_sum[l]);
			l = nl;
		}
		{
			int mi = l - 1, ma = r;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				int64_t pre = apply_alternating(l, md, x);
				if (std::abs(pre) > A[md]) {
					ma = md;
				} else {
					mi = md;
				}
			}
			return {ma, apply_alternating(l, ma, x)};
		}
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
	for (int i = N-1; i > 0; i--) {
		for (int x = A[i] + 1; x < A[i-1]; x += 2) {
			st_pt[x / 2] = i;
			auto [j, nx] = find_bad(i, N, x);
			//cerr << "jump " << x << '@' << i << ' ' << nx << '@' << j << '\n';
			if (j == N) {
				// self-loop
				jump_t small = {x/2, false};
				big_jumps[x / 2] = {small, 0, small};
			} else {
				jump_t small{std::abs(nx) / 2, x < 0};
				assert(st_pt[small.dest] == j);
				big_jumps[x / 2].small = small;
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

	auto solve = [&](int l, int r, int x) -> int {
		//cerr << "query " << l << ' ' << r << ' ' << x << '\n';
		assert(l < r);
		{
			auto [nl, nx] = find_bad(l, r, x);
			l = nl;
			x = nx;
		}
		//cerr << "initial " << l << ' ' << r << ' ' << x << '\n';
		if (l == r) return x;
		// Now we do the jump stuff
		while (true) {
			//cerr << "at " << l << ' ' << r << ' ' << x << '\n';
			assert(l < r);
			assert(l > 0 && std::abs(x) < A[l-1]);
			assert(std::abs(x) > A[l]);
			assert(st_pt[std::abs(x) / 2] == l);
			auto [small, big_len, big] = big_jumps[std::abs(x) / 2];
			if (big_len == 0 || r >= st_pt[small.dest]) {
				auto [nl, nx] = find_bad(l, r, x);
				assert(nl == r);
				return nx;
			} else if (st_pt[big.dest] < r) {
				x = signum(x) * pneg1(big.flip) * (big.dest * 2 + 1);
				l = st_pt[big.dest];
			} else {
				assert(st_pt[small.dest] < r);
				x = signum(x) * pneg1(small.flip) * (small.dest * 2 + 1);
				l = st_pt[small.dest];
			}
		}
	};

	if (TESTING) {
		for (int q = 0; q < 100; q++) {
			int l = std::uniform_int_distribution<int>(0, N-1)(mt);
			int r = std::uniform_int_distribution<int>(0, N-1)(mt);
			if (l > r) std::swap(l, r);
			r++;
			int x = std::uniform_int_distribution<int>(0, 30)(mt);
			x *= 2; x--;
			//cerr << l << ' ' << r << ' ' << x << '\n';
			int res = solve(l, r, x);
			//cerr << res << '\n';
		}
	} else {
		int Q; cin >> Q;
		while (Q--) {
			int l, r; int x; cin >> l >> r >> x; l--;
			x *= 2;
			x--;
			int res = solve(l, r, x);
			//cerr << "result " << res << '\n';
			cout << (res + 1) / 2 << '\n';
		}
	}
	}

	return 0;
}
