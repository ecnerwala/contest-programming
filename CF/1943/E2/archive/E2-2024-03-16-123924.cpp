#include <bits/stdc++.h>

int bob_wins_slow(int K, const std::vector<int>& buf) {
	int L = int(buf.size());
	for (int i = 0; i < L; i++) {
		std::vector<int> tmp(buf.begin(), buf.begin() + i + 1);
		for (int t = 0; t < i; t++) {
			// Alice takes her move
			tmp[t] = 0;
			{
				// Now bob takes his
				int cur_K = K;
				// the suffix of length c must all be equal
				for (int c = 1; tmp[i] > 0 && c <= i; c++) {
					if (cur_K / c >= tmp[i] - tmp[i-c]) {
						cur_K -= c * (tmp[i] - tmp[i-c]);
						for (int a = 0; a < c; a++) {
							tmp[i-a] = tmp[i-c];
						}
					} else {
						for (int a = 0; a < c; a++) {
							tmp[i-(c-1)+a] -= cur_K / c + (a < (cur_K % c));
						}
						cur_K = 0;
					}
				}
			}
		}
		if (tmp[i] == 0) return i;
	}
	return -1;
}

int bob_wins_fast(int K, const std::vector<int>& buf) {
	int L = int(buf.size());
	if (buf[0] == 0) return 0;

	std::vector<int64_t> tot_fill(L+1);
	std::vector<int64_t> height(L+1);
	tot_fill[0] = 0;
	height[0] = 0;
	for (int c = 1; c <= L; c++) {
		tot_fill[c] = tot_fill[c-1] + height[c-1] + K;
		height[c] = tot_fill[c] / c;
	}
	std::vector<int64_t> pref_height(L+2);
	for (int i = 0; i <= L; i++) {
		pref_height[i+1] = pref_height[i] + height[i];
	}

	// alice moves first
	std::vector<int64_t> pref_buf(L+1);
	for (int i = 0; i < L; i++) pref_buf[i+1] = pref_buf[i] + buf[i];

	int day = 0;
	for (int i = 1; i < L; i++) {
		// While we can fill the day, keep going
		while (i > day && buf[i - day] >= height[day]) {
			day++;
		}
		assert(buf[i + 1 - day] >= height[day - 1]);
		// Now, we have to take buf[i + 1 - day] .. buf[i] using height[0] .. height[day - 1] + (K * (i+1))
		int64_t need = pref_buf[i+1] - pref_buf[i+1-day];
		int64_t have = int64_t(K) * i + pref_height[day];
		if (have >= need) {
			return i;
		}
	}
	return -1;
}

void test() {
	using std::cerr;
	std::mt19937 mt(48);
	for (int z = 0; z <= 100000; z++) {
		std::cerr << "Doing test " << z << '\n';
		int K = std::uniform_int_distribution<int>(1, 10)(mt);
		int L = std::uniform_int_distribution<int>(1, 10)(mt);
		std::vector<int> buf(L);
		for (int i = 0; i < L; i++) {
			buf[i] = std::uniform_int_distribution<int>(1, 5)(mt);
		}
		std::sort(buf.begin(), buf.end());
		assert(bob_wins_fast(K, buf) == bob_wins_slow(K, buf));
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

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
			return bob_wins_fast(K, buf) != -1;
			//return bob_wins_slow(K, buf) != -1;
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
