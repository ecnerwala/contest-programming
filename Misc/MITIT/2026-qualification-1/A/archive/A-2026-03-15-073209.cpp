#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		assert(int(S.size()) == 2*N+1);
		auto it = std::find(S.begin(), S.end(), 'M');
		int cost = it != S.begin();
		std::rotate(S.begin(), std::find(S.begin(), S.end(), 'M'), S.end());
		std::string tgt; tgt.reserve(2*N+1);
		tgt.push_back('M');
		for (int i = 0; i < N; i++) {
			tgt.push_back('I');
			tgt.push_back('T');
		}
		if (S == tgt) {
			std::cout << cost << '\n';
		} else {
			std::cout << -1 << '\n';
		}
	}

	return 0;
}
