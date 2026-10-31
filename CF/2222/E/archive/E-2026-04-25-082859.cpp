#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		auto do_query = [&](char type, int64_t v) -> int {
			std::cout << type << ' ' << v << '\n' << std::flush;
			int res; std::cin >> res;
			return res;
		};
		auto cnt_ge = [&](int64_t v) -> int { return do_query('Q', v); };
		auto ins = [&](int64_t v) -> int { return do_query('I', v); };
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
		} else {
			// k == or or k == xor
			int64_t v = 0;
			for (int b = N-1; b >= 0; b--) {
				int64_t cnd = v + (int64_t(1) << b);
				if (cnt_ge(cnd)) v = cnd;
			}
			assert(v != 0);
			int r = ins(v);
			if (r == 1) {
				std::cout << 'A' << ' ' << 2 << ' ' << v << '\n' << std::flush;
			} else if (r == 2) {
				std::cout << 'A' << ' ' << 3 << ' ' << v << '\n' << std::flush;
			} else assert(false);
		}
	}

	return 0;
}
