#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, C; cin >> N >> C;

	std::vector<int> L(N); for (auto& l : L) cin >> l;

	auto find_cnt = [&](int min_dist) -> int {
		if (2 * min_dist > C) return 0;
		int gap_len = C - 2 * min_dist;
		std::vector<std::pair<int, bool>> parens(2*N);
		{
			std::vector<std::pair<int, bool>> opens; opens.reserve(N);
			std::vector<std::pair<int, bool>> closes; closes.reserve(N);
			for (int i = 0; i < N; i++) {
				opens.push_back({L[i] + min_dist, false});
				closes.push_back({L[i], true});
			}
			{
				auto it = opens.end();
				while (it != opens.begin() && std::prev(it)->first >= C) {
					--it;
					it->first -= C;
				}
				std::rotate(opens.begin(), it, opens.end());
			}
			auto res = std::merge(opens.begin(), opens.end(), closes.begin(), closes.end(), parens.begin());
			assert(res == parens.end());
		}

		{
			int min_bal_loc = 0;
			int bal = 0;
			for (int i = 0; i < 2*N; i++) {
				if (bal < 0) {
					min_bal_loc = i;
					bal = 0;
				}
				// close paren
				if (parens[i].second) bal--;
				else bal++;
			}
			assert(bal >= 0);
			for (int i = 0; i < min_bal_loc; i++) {
				parens[i].first += C;
			}
			std::rotate(parens.begin(), parens.begin() + min_bal_loc, parens.end());
		}

		{
			int bal = 0;
			for (int i = 0; i < 2*N; i++) {
				if (parens[i].second) bal--;
				else bal++;
				assert(bal >= 0);
			}
		}

		// we store the index and the max offset allowed
		std::vector<int> stk; stk.reserve(N);
		int good_open = 0;
		std::vector<int> open_loc; open_loc.reserve(N);
		int ans = 0;
		for (int i = 0; i < 2*N; i++) {
			if (parens[i].second) {
				// we're a close paren
				while (good_open < int(open_loc.size()) && open_loc[good_open] < parens[i].first - gap_len) good_open++;
				int max_offset = int(open_loc.size()) - good_open;
				assert(!stk.empty());
				max_offset = std::min(max_offset, stk.back());
				stk.pop_back();
				if (max_offset > 0) {
					ans++;
				}
				if (!stk.empty()) stk.back() = std::min(stk.back(), max_offset - 1);
			} else {
				// we're an open paren
				open_loc.push_back(parens[i].first);
				stk.push_back(2*N);
			}
		}
		return ans;
	};

	// now, just binary search for the first guy bigger than this
	for (int k = 1; k <= N/2; k++) {
		int mi = 0, ma = C+1;
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (find_cnt(md) >= 2*k) mi = md;
			else ma = md;
		}
		cout << mi << " \n"[k==N/2];
	}

	return 0;
}
