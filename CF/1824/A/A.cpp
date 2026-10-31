#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		int cnt_left = 0;
		int cnt_right = 0;
		std::vector<int> seats; seats.reserve(N);
		for (int i = 0; i < N; i++) {
			int x; cin >> x;
			if (x == -1) cnt_left++;
			else if (x == -2) cnt_right++;
			else if (1 <= x && x <= M) seats.push_back(x-1);
			else assert(false);
		}
		std::sort(seats.begin(), seats.end());
		seats.resize(std::unique(seats.begin(), seats.end()) - seats.begin());

		int ans = 0;
		ans = std::max(ans, std::min(M, cnt_left + int(seats.size())));
		ans = std::max(ans, std::min(M, cnt_right + int(seats.size())));
		for (int i = 0; i < int(seats.size()); i++) {
			ans = std::max(ans, std::min(seats[i], i + cnt_left) + 1 + std::min(M - 1 - seats[i], int(seats.size()) - 1 - i + cnt_right));
		}
		cout << ans << '\n';
	}

	return 0;
}
