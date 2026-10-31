#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] ^ A[i];

		int L = 25;
		struct trie_t {
			int cnt;
			std::array<int, 2> c;
		};
		std::vector<trie_t> tries((L+1) * (N+1));
		auto merge_tries = [&](this auto&& self, int a, int b) -> int {
			if (a == -1) return b;
			if (b == -1) return a;
			tries[a].cnt += tries[b].cnt;
			tries[a].c[0] = self(tries[a].c[0], tries[b].c[0]);
			tries[a].c[1] = self(tries[a].c[1], tries[b].c[1]);
			return a;
		};
		std::vector<std::pair<int, int>> suff_ands; suff_ands.reserve(L+2);
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			{
				// add in pref
				tries[i * (L+1)] = {1, {-1, -1}};
				for (int z = 0; z < L; z++) {
					tries[i * (L+1) + z+1].cnt = 1;
					tries[i * (L+1) + z+1].c = {-1, -1};
					tries[i * (L+1) + z+1].c[(pref[i] >> z) & 1] = i * (L+1) + z;
				}
				suff_ands.push_back({(1 << L) - 1, i * (L+1) + L});
			}

			{
				int len = 0;
				for (int z = 0; z < int(suff_ands.size()); z++) {
					suff_ands[z].first &= A[i];
					if (len && suff_ands[len-1].first == suff_ands[z].first) {
						suff_ands[len-1].second = merge_tries(suff_ands[len-1].second, suff_ands[z].second);
					} else {
						suff_ands[len++] = suff_ands[z];
					}
				}
				suff_ands.resize(len);
			}

			for (auto [a, t] : suff_ands) {
				int tgt = pref[i+1] ^ a;
				for (int b = L-1; b >= 0; b--) {
					t = tries[t].c[(tgt >> b) & 1];
					if (t == -1) break;
					if ((a >> b) & 1) ans += int64_t(tries[t].cnt) << b;
				}
			}
		}
		std::cout << ans << '\n';
	}

	return 0;
}
