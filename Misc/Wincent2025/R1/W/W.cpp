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
			int tot_either = 0;
			for (int f : freq) {
				int num_big = f % small_sz;
				int num_small = (f / small_sz) - num_big;
				if (num_small < 0) return std::nullopt;
				tot_big += num_big;
				tot_either += num_small / big_sz;
				tot_small += num_small % big_sz;
			}
			if (tot_big > need_big || tot_small > need_small) {
				return std::nullopt;
			}
			assert((need_big - tot_big) % small_sz == 0);
			assert((need_small - tot_small) % big_sz == 0);
			assert(tot_either == (need_big - tot_big) / small_sz + (need_small - tot_small) / big_sz);
			assert(need_small == tot_small);
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
