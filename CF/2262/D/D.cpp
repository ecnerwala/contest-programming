#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	//int T; std::cin >> T;
	int T = 10000;
	std::mt19937 mt(48);
	while (T--) {
		//int N; std::cin >> N;
		int N = std::uniform_int_distribution<int>(1, 5)(mt);
		std::vector<int> A(N);
		//for (auto& a : A) { std::cin >> a; a--; }
		std::vector<int> B(N);
		//for (auto& b : B) { std::cin >> b; b--; }
		for (int i = 0; i < N; i++) {
			B[i] = std::uniform_int_distribution<int>(i, N-1)(mt);
			A[i] = std::uniform_int_distribution<int>(0, B[i])(mt);
		}
		std::cerr << N << '\n';
		for (auto& a : A) { std::cerr << a << ' '; } std::cerr << '\n';
		for (auto& a : B) { std::cerr << a << ' '; } std::cerr << '\n';
		auto ans = [&]() -> std::optional<std::pair<int, std::vector<int>>> {
			for (int i = 0; i < N; i++) if (A[i] > B[i]) return std::nullopt;
			int first_bad = N-1;
			while (first_bad > 0 && A[first_bad] == B[first_bad]) first_bad--;
			if (first_bad < 0) return std::make_pair(0, std::vector<int>());
			int orig_bad = first_bad;

			std::vector<bool> is_okay(N, false);
			is_okay[first_bad] = true;
			std::vector<int> tgt(N, -1);
			int cur_min = first_bad;
			for (int i = first_bad - 1; i >= 0; i--) {
				is_okay[i] = B[i] > i && B[i] <= first_bad && is_okay[B[i]];
				if (A[i] != B[i] && !is_okay[i]) {
					return std::nullopt;
				}
				if (A[i] != B[i]) {
					if (i < A[cur_min]) {
						first_bad = i+1;
						cur_min = i;
						tgt[cur_min] = cur_min;
					} else if (A[i] < A[cur_min]) {
						assert(A[i] < i);
						tgt[cur_min] = i;
						cur_min = i;
					}
				}
			}
			tgt[cur_min] = cur_min;
			// we're only allowed to touch things that is_okay is true
			int st = cur_min;
			std::vector<int> ops; ops.reserve(2 * N * N);
			int cur = st;
			while (A[orig_bad] != B[orig_bad]) {
				int op = [&]() -> int {
					std::cerr << "at " << cur << ' ' << tgt[cur] << '\n';
					if (tgt[cur] != cur && tgt[cur] != -1) {
						if (tgt[tgt[cur]] == -1) {
							tgt[cur] = cur;
						} else {
							return A[cur] < tgt[cur] ? 1 : 2;
						}
					}
					if (tgt[cur] == cur) {
						if (A[cur] < cur) {
							return A[A[cur]] != B[A[cur]] ? 2 : 1;
						} else {
							tgt[cur] = -1;
						}
					}
					if (tgt[cur] == -1) {
						assert(A[cur] <= B[cur]);
						return A[cur] < B[cur] ? 1 : 2;
					}
					assert(false);
				}();
				ops.push_back(op);
				if (op == 1) {
					A[cur]++;
				} else if (op == 2) {
					cur = A[cur];
				} else assert(false);
			}
		for (auto& a : A) { std::cerr << a << ' '; } std::cerr << '\n';
			assert(A == B);
			return std::make_pair(st, std::move(ops));
		}();
		if (ans) {
			auto& v = ans->second;
			std::cout << v.size() << ' ' << ans->first + 1 << '\n';
			for (int i = 0; i < int(v.size()); i++) std::cout << v[i] << " \n"[i+1==int(v.size())];
			if (v.empty()) std::cout << '\n';
		} else {
			std::cout << -1 << '\n';
		}
	}

	return 0;
}
