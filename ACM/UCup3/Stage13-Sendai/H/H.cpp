#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<std::pair<int, int>> best(N);
	for (int i = N-1; i >= 0; i--) {
		best[i] = {A[i], -i};
		if (i+K < N) best[i] = std::max(best[i], best[i+K]);
	}
	std::vector<int> res;
	for (int i = 0; i < N; i++) {
		i = -best[i].second;
		res.push_back(A[i]);
	}
	for (int i = 0; i < int(res.size()); i++) {
		cout << res[i] << " \n"[i+1==int(res.size())];
	}

	return 0;
}
