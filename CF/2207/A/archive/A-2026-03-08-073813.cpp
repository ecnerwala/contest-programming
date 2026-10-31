#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		for (int z = 0; z < 3; z++) {
			for (int i = 0; i+2 < N; i++) {
				if (S[i] == '1' && S[i+2] == '1') S[i+1] = '1';
			}
		}
		int max_cnt = 0;
		for (int i = 0; i < N; i++) max_cnt += S[i] == '1';
		for (int i = 0; i < N; i++) {
			if (S[i] == '1' && S[i+2] == '1') S[i+1] = '0';
		}
		int min_cnt = 0;
		for (int i = 0; i < N; i++) min_cnt += S[i] == '1';
		std::cout << min_cnt << ' ' << max_cnt << '\n';
	}

	return 0;
}
