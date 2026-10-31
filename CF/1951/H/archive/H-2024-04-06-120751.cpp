#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		// We want to solve the 0/1 game
		// Pretty much each subgame has some surplus and some deficit
		// When you go up by 1 level, you merge children's counts, then cancel 1 surplus with 1 deficit
		// You win if there's 0 deficit at the top
		int K; cin >> K;
		int N = int(1 << K);
		std::vector<int> A(1<<K);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> ans(K+1);
		std::vector<int> buf(1<<K);
		// surplus, deficit
		std::vector<std::pair<int, int>> seg(1 << K);
		for (int d = K; d >= 1; d--) {
			// We can just mergesort the bottom, since the last turn immediately merges them anyways
			for (int i = 0; i < (1 << K); i += (2 << (K-d))) {
				// Now mergesort the things up
				std::merge(A.begin() + i, A.begin() + i + (1 << (K-d)), A.begin() + i + (1 << (K-d)), A.begin() + i + (2 << (K-d)), buf.begin());
				std::copy(buf.begin(), buf.begin() + (2 << (K-d)), A.begin() + i);
			}

			// Check if we can force everything to be >= v
			auto check = [&](int v) -> bool {
				for (int i = 0; i < (1 << K); i += (2 << (K-d))) {
					int cnt_small = int(std::lower_bound(A.begin() + i, A.begin() + i + (2 << (K-d)), v) - (A.begin() + i));
					int cnt_big = (2 << (K-d)) - cnt_small;
					int surplus = std::max(cnt_big - 2, 0);
					int deficit = std::max(2 - cnt_big, 0);
					seg[(1 << (d-1)) + i / (2 << (K-d))] = {surplus, deficit};
				}
				for (int i = (1 << (d-1)) - 1; i > 0; i--) {
					seg[i].first = seg[2 * i].first + seg[2 * i + 1].first;
					seg[i].second = seg[2 * i].second + seg[2 * i + 1].second;
					if (seg[i].first > 0 && seg[i].second > 0) {
						seg[i].first--;
						seg[i].second--;
					}
				}
				return seg[1].second == 0;
			};

			int ma = (1 << K);
			int mi = 0;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (check(md)) mi = md;
				else ma = md;
			}
			ans[d] = ma;
		}
	}

	return 0;
}
