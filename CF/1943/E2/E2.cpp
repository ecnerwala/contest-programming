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
		std::vector<int64_t> tot_fill(M+3);
		std::vector<int64_t> height(M+3);
		tot_fill[0] = 0;
		height[0] = 0;
		for (int c = 1; c <= M+2; c++) {
			tot_fill[c] = tot_fill[c-1] + height[c-1] + K;
			height[c] = tot_fill[c] / c;
		}
		std::vector<int64_t> pref_height(M+4);
		for (int i = 0; i <= M+2; i++) {
			pref_height[i+1] = pref_height[i] + height[i];
		}

		std::vector<int> buf; buf.reserve(M+1);
		std::vector<int64_t> pref_buf(M+2);
		auto bob_wins = [&](int L) -> bool {
			buf.assign(freq.begin(), freq.begin() + L);
			std::sort(buf.begin(), buf.end());
			if (buf[0] == 0) return true;
			// alice moves first
			buf[0] = 0;
			for (int i = 0; i < L; i++) pref_buf[i+1] = pref_buf[i] + buf[i];
			cerr << "solve " << L << '\n';
			for (auto& c : buf) { cerr << c << ' '; } cerr << '\n';
			int day = 0;
			for (int i = 1; i < L; i++) {
				// While we can fill the day, keep going
				while (buf[i - day] >= height[day]) {
					assert(day != i);
					day++;
				}
				cerr << "kill " << i << ' ' << "day " << day << '\n';
				assert(buf[i + 1 - day] >= height[day - 1]);
				// Now, we have to take buf[i + 1 - day] .. buf[i] using height[0] .. height[day - 1] + (K * (i+1))
				int64_t need = pref_buf[i+1] - pref_buf[i-day];
				int64_t have = int64_t(K) * (i+1) + pref_height[day];
				if (have >= need) {
					return true;
				}
			}
			return false;
		};
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (bob_wins(md)) {
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
