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

			{
				int tot_big = 0;
				int tot_small = 0;
				for (int f : freq) {
					// more bigs is better, cause it's just a smaller r
					int num_small = (big_sz - f % big_sz) % big_sz;
					int num_big = (f + num_small) / big_sz - num_small;
					if (num_big < 0) return std::nullopt;

					tot_big += num_big;
					tot_small += num_small;
				}
				if (tot_big < need_big) return std::nullopt;
				assert(tot_big == need_big && tot_small == need_small);
			}

			std::array<std::vector<int>, 52> inds;
			for (int v = 0; v < 52; v++) inds[v].reserve(freq[v]);
			for (int i = 0; i < N; i++) {
				inds[S[i]].push_back(i);
			}

			std::vector<std::vector<int64_t>> big_weights(need_big, std::vector<int64_t>(need_big));
			std::vector<std::vector<int64_t>> small_weights(need_small, std::vector<int64_t>(need_small));
			int big_idx = 0;
			int small_idx = 0;
			for (int v = 0; v < 52; v++) {
				int f = freq[v];

				// more bigs is better, cause it's just a smaller r
				int num_small = (big_sz - f % big_sz) % big_sz;
				int num_big = (f + num_small) / big_sz - num_small;
				if (num_big < 0) return std::nullopt;

				for (int i = 0; i < num_big; i++) {
					for (int m = 0, z = i; m < big_sz; m++, z += num_big + num_small) {
						int loc = inds[v][z];
						for (int a = 0; a < need_big; a++) {
							big_weights[big_idx][a] += std::abs(m * r + a - loc);
						}
					}
					big_idx++;
				}

				for (int i = 0; i < num_small; i++) {
					for (int m = 0, z = num_big + i; m < small_sz; m++, z += num_big + num_small) {
						int loc = inds[v][z];
						for (int a = 0; a < need_small; a++) {
							small_weights[small_idx][a] += std::abs(m * r + a - loc);
						}
					}
					small_idx++;
				}
			}
			assert(big_idx == need_big);
			assert(small_idx == need_small);

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
