#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int M; int K; cin >> M >> K;
		std::vector<std::pair<int, int>> freq(M+1);
		for (int i = 0; i <= M; i++) {
			cin >> freq[i].first;
			freq[i].second = i;
		}
		std::sort(freq.begin(), freq.end());

		std::vector<int> buf; buf.reserve(M+1);
		auto check = [&](int L) -> bool {
			buf.clear();
			for (auto [f, i] : freq) {
				if (i < L) buf.push_back(f);
			}

			if (buf[0] == 0) return true;

			int day = 0;
			int64_t tot_fill = 0;
			int64_t height = 0;
			int64_t surplus = 0;
			for (int i = 1; i < L; i++) {
				surplus += buf[i];
				surplus -= buf[i - day];
				// While we can fill the day, keep going
				if (buf[i - day] >= height) {
					surplus += buf[i - day] - height;
					day++;
					tot_fill += height + K;
					height = tot_fill / day;
				}
				surplus -= K;
				if (surplus <= 0) {
					return true;
				}
			}
			return false;
		};

		int mi = 0;
		int ma = M+2;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (check(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		cout << mi << '\n';
	}

	return 0;
}
