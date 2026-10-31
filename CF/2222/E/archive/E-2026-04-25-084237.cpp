#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	constexpr bool TESTING = true;
	std::mt19937 mt(48);

	int T;
	if constexpr (TESTING) {
		T = 100000;
	} else {
		std::cin >> T;
	}
	while (T--) {
		int N;
		int K_ans; int64_t C_ans;
		if constexpr (TESTING) {
			N = std::uniform_int_distribution<int>(2, 5)(mt);
			K_ans = std::uniform_int_distribution<int>(1, 3)(mt);
			C_ans = std::uniform_int_distribution<int64_t>(1, (int64_t(1) << N) - 1)(mt);
		} else {
			std::cin >> N;
		}
		std::set<int64_t> st;
		int num_ops = 0;
		auto do_query = [&](char type, int64_t v) -> int {
			num_ops++;
			if constexpr (TESTING) {
				if (type == 'I') {
					if (K_ans == 1) st.insert(v & C_ans);
					else if (K_ans == 2) st.insert(v | C_ans);
					else if (K_ans == 3) st.insert(v ^ C_ans);
					else assert(false);
					return int(st.size());
				} else if (type == 'Q') {
					int ans = 0;
					for (auto z : st) ans += (z >= v);
					return ans;
				}
			}
			std::cout << type << ' ' << v << '\n' << std::flush;
			int res; std::cin >> res;
			return res;
		};
		auto cnt_ge = [&](int64_t v) -> int { return do_query('Q', v); };
		auto ins = [&](int64_t v) -> int { return do_query('I', v); };
		std::cout << 0 << '\n' << std::flush;
		st.insert(0);
		ins(0);
		if (cnt_ge(1) == 0) {
			// k == and
			ins((int64_t(1) << N) - 1);
			int64_t v = 0;
			for (int b = N-1; b >= 0; b--) {
				int64_t cnd = v + (int64_t(1) << b);
				if (cnt_ge(cnd)) v = cnd;
			}
			assert(v > 0);
			std::cout << 'A' << ' ' << 1 << ' ' << v << '\n' << std::flush;
			if constexpr (TESTING) {
				assert(K_ans == 1);
				assert(C_ans == v);
			}
		} else {
			// k == or or k == xor
			int64_t v = 0;
			for (int b = N-1; b >= 0; b--) {
				int64_t cnd = v + (int64_t(1) << b);
				if (cnt_ge(cnd)) v = cnd;
			}
			assert(v != 0);
			int r = ins(v);
			if (r == 2) {
				std::cout << 'A' << ' ' << 2 << ' ' << v << '\n' << std::flush;
				if constexpr (TESTING) {
					assert(K_ans == 2);
					assert(C_ans == v);
				}
			} else if (r == 3) {
				std::cout << 'A' << ' ' << 3 << ' ' << v << '\n' << std::flush;
				if constexpr (TESTING) {
					assert(K_ans == 3);
					assert(C_ans == v);
				}
			} else assert(false);
		}
		assert(num_ops <= N+3);
	}

	return 0;
}
