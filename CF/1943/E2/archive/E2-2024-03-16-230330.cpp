#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int M; int K; cin >> M >> K;
		std::vector<int> freq(M+1);
		for (auto& f : freq) cin >> f;
		int mi = 0;
		int ma = M+2;

		std::vector<int> buf; buf.reserve(M+1);
		auto check = [&](int L) -> bool {
			buf.assign(freq.begin(), freq.begin() + L);
			std::sort(buf.begin(), buf.end());

			if (buf[0] == 0) return true;

			int day = 0;
			int64_t tot_fill = 0;
			int64_t height = 0;
			int64_t surplus = 0;
			for (int i = 1; i < L; i++) {
				surplus += buf[i];
				surplus -= buf[i - day];
				// While we can fill the day, keep going
				if (i > day && buf[i - day] >= height) {
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

// Alice always just takes the lowest remaining frequency
