#include<bits/stdc++.h>

std::pair<int, std::vector<std::string>> solve(int N, int K) {
	int area = N * (N+1) / 2;
	int cnt = (area + K-1) / K;
	std::vector<std::string> grid(N);
	for (int i = 0; i < N; i++) grid[i] = std::string(i+1, '?');

	return {cnt, grid};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> K;
		cerr << "Case #" << case_num << ": " << N << ' ' << K << '\n';
		auto [cnt, res] = solve(N, K);

		cout << "Case #" << case_num << ": " << cnt << '\n';
		for (std::string s : res) cout << s << '\n';
	}

	return 0;
}
