#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N0, N1; std::cin >> N0 >> N1;
	std::vector<int64_t> p0(2*N0);
	for (auto& l : p0) std::cin >> l;
	std::vector<int64_t> p1(2*N1);
	for (auto& l : p1) std::cin >> l;

	int Q; std::cin >> Q;
	while (Q--) {
		int64_t _T; std::cin >> _T;
		__int128_t T = _T;
		auto check = [&](__int128_t k, int v, const std::vector<int64_t>& p) -> bool {
			assert(1 <= v && v <= 6);
			// We want to check that T * (a + v/6) / k is good for all a
			// T * 6 * a + T * v must be in 6 * k * p for a in [0, k)
			auto it = p.begin();
			for (__int128_t i = 0; i < k; ) {
				if (it == p.end()) return false;
				__int128_t lo = 6 * k * (*it), hi = 6 * k * (*std::next(it));
				lo -= T * v;
				hi -= T * v;
				if (lo > T * 6 * i) return false;
				if (hi >= T * 6 * i) {
					i = hi / (T * 6);
					i++;
				}
				it += 2;
			}
			return true;
		};
		std::cout << [&]() -> int64_t {
			__int128_t K = 1;
			while (true) {
				if (6 * K > T) return -1;
				if (!check(K, 6, p0)) return -1;
				if (!check(K, 1, p1)) { K *= 6; continue; }
				if (!check(K, 5, p1)) { K *= 6; continue; }
				if (!check(K, 2, p1)) { K *= 3; continue; }
				if (!check(K, 4, p1)) { K *= 3; continue; }
				if (!check(K, 3, p1)) { K *= 2; continue; }
				return int64_t(K * 6);
			}
		}() << '\n';
	}

	return 0;
}
