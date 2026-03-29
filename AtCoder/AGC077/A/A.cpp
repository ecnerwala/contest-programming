#include <bits/stdc++.h>

struct interval_t {
	int l, r, d;
	void incr_head() {
		l += d;
	}
	void decr_tail() {
		r -= d;
	}
	int len() const { return (r - l) * d + 1; }
	void flip() {
		std::swap(l, r);
		d = -d;
	}
};

int signum(int a) {
	return (a > 0) - (a < 0);
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto solve = [&](std::string X, std::string Y) -> std::optional<std::vector<std::array<int, 2>>> {
		int N = int(X.size());
		assert(N == int(Y.size()));
		int v_X = 0, v_Y = 0;
		for (int i = 0; i < N; i++) {
			v_X += (X[i] == 'B');
			v_Y += (Y[i] == 'B');
		}
		// equal count
		if (v_X != v_Y) {
			return std::nullopt;
		}
		// lex less from the start, lex greater from the end
		if (X == Y) {
			return std::vector<std::array<int, 2>>();
		}
		int L = 0, R = N-1;
		while (X[L] == Y[L]) L++;
		while (X[R] == Y[R]) R--;
		if (X[L] != 'A' || X[R] != 'B') return std::nullopt;

		std::vector<int> pref_X(N+1);
		for (int i = 0; i < N; i++) pref_X[i+1] = pref_X[i] + (X[i] == 'B');
		std::vector<int> pref_Y(N+1);
		for (int i = 0; i < N; i++) pref_Y[i+1] = pref_Y[i] + (Y[i] == 'B');

		auto get_pref = [&](const std::vector<int>& pref, interval_t inter) {
			if (inter.d == +1) {
				return pref[inter.r+1] - pref[inter.l];
			} else if (inter.d == -1) {
				return pref[inter.l+1] - pref[inter.r];
			} else assert(false);
		};
		auto with_len = [&](interval_t inter, int l) -> interval_t {
			return {inter.l, inter.l + inter.d * (l-1), inter.d};
		};
		auto without_len = [&](interval_t inter, int l) -> interval_t {
			return {inter.l + inter.d * l, inter.r, inter.d};
		};

		std::vector<std::array<int, 2>> pre_ops; pre_ops.reserve(N);
		std::vector<std::array<int, 2>> post_ops; post_ops.reserve(N);
		[&](this auto&& self, interval_t x, interval_t pre, interval_t post, interval_t y) {
			assert(get_pref(pref_X, x) == get_pref(pref_Y, y));
			while (x.len() > 0 && X[x.l] == Y[y.l]) {
				x.incr_head(), y.incr_head(), pre.incr_head(), post.incr_head();
			}
			assert(get_pref(pref_X, x) == get_pref(pref_Y, y));
			if (x.len() == 0) return;
			assert(X[x.l] != Y[y.l]);
			int dir = (X[x.l] == 'B') - (Y[y.l] == 'B');
			// now, we find a good prefix
			auto chk = [&](int md) -> bool {
				int vx = get_pref(pref_X, with_len(x, md));
				int vy = get_pref(pref_Y, with_len(y, md));
				return (vx - vy) * dir > 0;
			};
			int mi = 1, ma = x.len();
			assert(chk(mi));
			assert(!chk(ma));
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (chk(md)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			// solve the rest
			self(without_len(x, ma), without_len(pre, ma), without_len(post, ma), without_len(y, ma));
			x = with_len(x, ma);
			pre = with_len(pre, ma);
			post = with_len(post, ma);
			y = with_len(y, ma);

			assert(X[x.l] != Y[y.l]);
			assert(X[x.r] != Y[y.r]);
			assert(X[x.l] != X[x.r]);

			if (X[x.l] == 'B') {
				x.flip();
				pre.flip();
				post.flip();
				y.flip();
			}
			assert(X[x.l] == 'A');
			assert(pre.d != post.d);
			if (pre.d == 1) {
				// we flip before
				pre_ops.push_back({pre.l, pre.r});
				x.flip();
			} else {
				post_ops.push_back({post.l, post.r});
				y.flip();
			}
			self(x, pre, post, y);
		}({L+1, R-1, +1}, {L+1, R-1, +1}, {R-1, L+1, -1}, {R-1, L+1, -1});

		std::vector<std::array<int, 2>> ops; ops.reserve(N);
		ops.insert(ops.end(), pre_ops.begin(), pre_ops.end());
		ops.push_back({L, R});
		ops.insert(ops.end(), post_ops.rbegin(), post_ops.rend());
		return ops;
	};
	if (false) {
		std::mt19937 mt(48);
		for (int z = 0; z < 100000; z++) {
			int N = std::uniform_int_distribution<int>(1, 20)(mt);
			int cA = std::uniform_int_distribution<int>(0, N)(mt);
			std::string X = std::string(cA, 'A') + std::string(N - cA, 'B');
			std::string Y = X;
			std::shuffle(X.begin(), X.end(), mt);
			std::shuffle(Y.begin(), Y.end(), mt);
			std::cerr << X << '\n';
			std::cerr << Y << '\n';
			solve(X, Y);
		}
		std::cerr << "done testing" << '\n';
	}
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string X, Y; std::cin >> X >> Y;
		assert(int(X.size()) == N);
		assert(int(Y.size()) == N);

		auto ans = solve(X, Y);
		if (ans) {
			std::cout << "Yes" << '\n';
			std::cout << ans->size() << '\n';
			for (auto [l, r] : *ans) {
				std::cout << l+1 << ' ' << r+1 << '\n';
			}
		} else {
			std::cout << "No" << '\n';
		}
	}

	return 0;
}
