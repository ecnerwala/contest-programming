#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	std::mt19937_64 mt(48);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<uint64_t> diff(N+1);
		for (int i = 0; i < M; i++) {
			int l, r; std::cin >> l >> r; l--;
			uint64_t v = mt();
			diff[l] ^= v;
			diff[r] ^= v;
		}
		std::vector<std::pair<uint64_t, int>> vals(N);
		{
			uint64_t cur = 0;
			for (int i = 0; i < N; i++) {
				cur ^= diff[i];
				vals[i] = {cur, i};
			}
			cur ^= diff[N];
			assert(cur == 0);
		}
		std::sort(vals.begin(), vals.end());
		std::vector<int> evts(N, N);
		std::pair<int, uint64_t> first_bad{N, 0};
		std::pair<int, uint64_t> last_bad{-1, 0};
		for (int i = 0; i+1 < int(vals.size()); i++) {
			if (vals[i].first == vals[i+1].first) {
				int a = vals[i].second, b = vals[i+1].second;
				assert(a < b);
				evts[a] = std::min(evts[a], b);
				first_bad = std::min(first_bad, {a, vals[i].first});
				last_bad = std::max(last_bad, {b, vals[i+1].first});
			}
		}
		std::vector<int> cuts; cuts.reserve(N);
		for (int i = 0; i+1 < N; i++) {
			if (evts[i] == i+1) {
				// must cut here
				cuts.push_back(i+1);
			} else {
				evts[i+1] = std::min(evts[i+1], evts[i]);
			}
		}
		if (first_bad.first == N) {
			assert(last_bad.first == -1);
			assert(cuts.empty());
		} else if (first_bad.second == last_bad.second) {
			cuts.push_back(N);
		}
		if (cuts.size() & 1) {
			cuts.insert(cuts.begin(), 0);
		}
		std::cout << cuts.size() / 2 << '\n';
		for (int i = 0; i < int(cuts.size()) / 2; i++) {
			int l = cuts[i], r = cuts[i + int(cuts.size()) / 2];
			std::cout << l+1 << ' ' << r << '\n';
		}
	}

	return 0;
}
