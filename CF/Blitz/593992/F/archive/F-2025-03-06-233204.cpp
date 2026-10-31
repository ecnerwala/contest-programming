#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S, T; cin >> S >> T;
	int N = int(S.size());
	int M = int(T.size());
	std::vector<std::array<int, 2>> res;
	int cur = 0;
	while (cur < int(T.size())) {
		std::pair<int, std::array<int, 2>> max_match{0, {-1, -1}};
		for (int i = 0; i < N; i++) {
			int j = i;
			while (j<N && j-i < M-cur && S[j] == T[cur+(j-i)]) j++;
			max_match = std::max(max_match, std::pair<int, std::array<int, 2>>(j-i, {i, j-1}));
		}
		for (int i = 0; i < N; i++) {
			int j = i;
			while (j >= 0 && i-j < M-cur && S[j] == T[cur+(i-j)]) j--;
			max_match = std::max(max_match, std::pair<int, std::array<int, 2>>(i-j, {i, j+1}));
		}
		if (max_match.first == 0) {
			cout << -1 << '\n';
			exit(0);
		}
		res.push_back(max_match.second);
		cur += max_match.first;
	}
	cout << res.size() << '\n';
	for (auto [x, y] : res) cout << x+1 << ' ' << y+1 << '\n';

	return 0;
}
