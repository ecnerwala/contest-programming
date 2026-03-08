#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> X(N);
		auto query = [&]() -> int {
			std::cout << '?';
			for (int i = 0; i < N; i++) {
				std::cout << ' ' << X[i]+1;
			}
			std::cout << '\n' << std::flush;
			int resp; std::cin >> resp; resp--;
			return int(std::find(X.begin(), X.end(), resp) - X.begin());
		};

		struct op_t {
			int l, r; // half-open as usual
			bool is_max;
			std::array<int, 2> ch{-1, -1};
		};
		std::vector<op_t> ops; ops.reserve(2 * N - 1);
		ops.resize(N);

		auto merge = [&](int a, int b, bool is_max) -> int {
			int r = int(ops.size());
			ops.push_back({ops[a].l, ops[b].r, is_max, {a, b}});
			return r;
		};

		auto only = [&](std::vector<int> a) -> int {
			assert(int(a.size()) == 1);
			return a[0];
		};
		auto solve = [&](this auto&& self, int l, int r, bool is_max, std::optional<int> known_m = std::nullopt) -> std::vector<int> {
			std::cerr << "solve " << l << ' ' << r << ' ' << is_max << '\n';
			if (is_max) {
				assert(std::is_sorted(X.begin() + l, X.begin() + r));
			} else {
				assert(std::is_sorted(X.begin() + l, X.begin() + r, std::greater<>()));
			}
			if (r - l == 0) {
				return {};
			}
			if (r - l == 1) {
				auto& op = ops[l];
				op.l = l, op.r = r;
				op.is_max = false;
				return std::vector<int>({l});
			}
			if (r - l == 2) {
				// we could solve 2 as well, save a query
				auto a = only(self(l, l+1, is_max));
				auto b = only(self(r-1, r, is_max));
				return std::vector<int>({a, b});
			}
			assert(r - l >= 3);
			int m = known_m ? *known_m : query();
			assert(l <= m && m < r);
			// first, put ourselves at the bottom
			std::rotate(X.begin() + l, X.begin() + l+1, X.begin() + m+1);
			auto l_ch = self(l, m, is_max);
			std::rotate(X.begin() + l, X.begin() + m, X.begin() + m+1);

			auto mid_ch = only(self(m, m+1, is_max));

			std::reverse(X.begin() + m, X.begin() + r);
			auto r_ch = self(m+1, r, !is_max);
			std::reverse(X.begin() + m, X.begin() + r);

			// now, we gotta merge them
			std::reverse(r_ch.begin(), r_ch.end());
			while (!r_ch.empty()) {
				assert(!l_ch.empty());
				assert(ops[l_ch.back()].r == ops[mid_ch].l);
				assert(ops[r_ch.back()].l == ops[mid_ch].r);
				std::reverse(X.begin() + ops[l_ch.back()].l, X.begin() + ops[r_ch.back()].r);
				int loc = query();
				std::reverse(X.begin() + ops[l_ch.back()].l, X.begin() + ops[r_ch.back()].r);
				assert(ops[l_ch.back()].l <= loc && loc < ops[r_ch.back()].r); 
				assert(!(ops[mid_ch].l <= loc && loc < ops[mid_ch].r));
				if (loc < ops[mid_ch].l) {
					// merge right first
					mid_ch = merge(mid_ch, r_ch.back(), !is_max);
					r_ch.pop_back();
				} else if (ops[mid_ch].r <= loc) {
					// go left
					mid_ch = merge(l_ch.back(), mid_ch, is_max);
					l_ch.pop_back();
				} else assert(false);
			}
			assert(!l_ch.empty());
			l_ch.push_back(mid_ch);
			return l_ch;
		};

		std::iota(X.begin(), X.end(), 0);
		int loc_incr = query();

		std::reverse(X.begin(), X.end());
		int loc_decr = query();
		std::reverse(X.begin(), X.end());

		assert(N == 1 || (loc_incr != loc_decr));
		bool is_max = (loc_incr >= loc_decr);
		if (!is_max) std::reverse(X.begin(), X.end());
		auto top = solve(0, N, is_max, is_max ? loc_incr : loc_decr);
		while (int(top.size()) >= 2) {
			int b = top.back(); top.pop_back();
			int a = top.back(); top.pop_back();
			top.push_back(merge(a, b, is_max));
		}
		int root = only(top);
		std::cout << '!' << '\n' << std::flush;
		while (true) {
			std::vector<int> x_cur(N);
			std::cin >> x_cur[0];
			if (x_cur[0] == 0) break;
			for (int i = 1; i < N; i++) std::cin >> x_cur[i];
			std::cout << [&](this auto&& self, int cur) -> int {
				if (cur < N) return x_cur[cur];
				int a = self(ops[cur].ch[0]);
				int b = self(ops[cur].ch[1]);
				return ops[cur].is_max ? std::max(a, b) : std::min(a, b);
			}(root) << '\n' << std::flush;
		}
	}

	return 0;
}
