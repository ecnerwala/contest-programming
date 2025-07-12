#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> S(N);
		{
			std::string S_; S_.reserve(N); cin >> S_;
			assert(int(S_.size()) == N);
			for (int i = 0; i < N; i++) {
				char c = S_[i];
				if ('a' <= c && c <= 'z') S[i] = c - 'a';
				else if ('A' <= c && c <= 'Z') S[i] = 26 + (c - 'A');
				else assert(false);
			}
		}
		std::array<int, 52> freq; freq.fill(0);
		for (int c : S) freq[c]++;

		auto solve_r = [&](int r) -> std::optional<int> {
			// we need to bucket things into groups of N / r
			int small_sz = N / r;
			int big_sz = small_sz + 1;

			int need_big = N % r;
			int need_small = r - need_big;

			int tot_big = 0;
			int tot_small = 0;
			for (int f : freq) {
				// more bigs is better
				int num_small = (big_sz - f % big_sz) % big_sz;
				int num_big = (f + num_small) / big_sz - num_small;
				if (num_big < 0) return std::nullopt;

				tot_big += num_big;
				tot_small += num_small;
			}
			assert(tot_big == need_big && tot_small == need_small);

			return 0;
		};

		for (int r = 1; r <= N; r++) {
			auto a_opt = solve_r(r);
			if (a_opt) {
				cout<< r << ' ' << *a_opt << '\n';
				break;
			}
		}
	}

	return 0;
}
