#include <bits/stdc++.h>

std::vector<int> solve(int N, int K) {
	if (N-K > K) {
		auto res = solve(N, N-K);
		for (auto& v : res) v = -v;
		return res;
	}
	int num_neg = N-K;
	assert(num_neg <= K);
	std::vector<int> res(N); iota(res.begin(), res.end(), 1);
	for (int i = 0; i < num_neg; i++) res[2*i] *= -1;
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		auto res = solve(N, K);
		for (int i = 0; i < N; i++) cout << res[i] << " \n"[i+1==N];
	}

	return 0;
}
