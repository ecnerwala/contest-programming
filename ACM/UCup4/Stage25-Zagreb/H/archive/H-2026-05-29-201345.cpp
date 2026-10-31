#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		std::string S; std::cin >> S;
		int N = int(S.size());
		std::array<std::vector<int>, 2> inds;
		inds[0].reserve(N);
		inds[1].reserve(N);
		bool state = 0;
		for (int i = N-1; i >= 0; i--) {
			inds[state].push_back(i);
			if (i > 0 && S[i] != S[i-1]) {
				state = S[i-1] > S[i];
			}
		}
		inds[0].insert(inds[0].end(), inds[1].rbegin(), inds[1].rend());
		S.erase(S.begin() + inds[0][N/2]);
		std::cout << S << '\n';
	}

	return 0;
}
