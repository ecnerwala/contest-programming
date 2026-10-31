#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	uint32_t S, T; cin >> S >> T;
	std::vector<std::pair<char, uint32_t>> A(N);
	for (auto& [op, a] : A) cin >> op >> a;

	cout << [&]() -> int {
		std::vector<uint32_t> orig_seq; orig_seq.reserve(N+1);
		{
			uint32_t cur = S;
			orig_seq.push_back(cur);
			for (auto [op, a] : A) {
				if (op == 'A') cur &= a;
				else if (op == 'O') cur |= a;
				else if (op == 'X') cur ^= a;
				else assert(false);
				orig_seq.push_back(cur);
			}
		}

		std::map<std::array<uint32_t, 2>, int> dp;
		int best_ans = -1;

		dp[{~T, T}] = 0;
		for (int i = N-1; i >= 0; i--) {
			auto [op, a] = A[i];
			std::map<std::array<uint32_t, 2>, int> ndp;
			auto check = [&](std::array<uint32_t, 2> k, int v) -> void {
				if (ndp.count(k)) ndp[k] = std::min(ndp[k], v);
				else ndp[k] = v;
			};

			if (op == 'X') {
				// Since this can make the prefix into anything, we should just check and then exit
				for (auto [st, cost] : dp) {
					uint32_t ov = orig_seq[i+1];
					if ((ov & st[0]) || ((~ov) & st[1])) cost++;
					if (best_ans == -1 || cost < best_ans) best_ans = cost;
				}
				return best_ans;
			} else if (op == 'A') {
				for (auto [st, cost] : dp) {
					if (!((~a) & st[1])) {
						// don't change
						check({st[0] & a, st[1]}, cost);
					}

					check({0, st[1]}, cost+1);
				}
			} else if (op == 'O') {
				for (auto [st, cost] : dp) {
					if (!(a & st[0])) {
						check({st[0], st[1] & ~a}, cost);
					}

					check({st[0], 0}, cost+1);
				}
			} else assert(false);
			dp = ndp;
		}

		for (auto [st, cost] : dp) {
			if ((S & st[0]) || ((~S) & st[1])) {
				continue;
			}
			if (best_ans == -1 || cost < best_ans) best_ans = cost;
		}
		return best_ans;
	}() << '\n';

	return 0;
}
