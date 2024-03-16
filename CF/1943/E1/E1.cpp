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
		auto bob_wins = [&](int L) -> bool {
			buf.assign(freq.begin(), freq.begin() + L);
			std::sort(buf.begin(), buf.end());
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
				if (tmp[i] == 0) return true;
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
