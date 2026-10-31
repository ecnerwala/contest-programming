#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	int64_t M = 1;

	std::vector<std::pair<int64_t, int64_t>> pairs(N);
	for (int i = 0; i < N; i++) {
		M = std::max(M, int64_t(A[i]) + 1);
		M = std::max(M, int64_t(B[i]) + 1);
		pairs[i] = {A[i], B[i]};
	}
	std::sort(pairs.begin(), pairs.end(), std::greater<>());
	pairs.resize(std::unique(pairs.begin(), pairs.end()) - pairs.begin());
	for (int i = 0; i+1 < int(pairs.size()); i++) {
		if (pairs[i].first == pairs[i+1].first) {
			cout << "No" << '\n';
			exit(0);
		}
	}

	N = int(pairs.size());
	std::vector<int64_t> ops;
	ops.reserve(2*N);

	std::rotate(pairs.begin(), pairs.end() - 1, pairs.end());
	for (int i = 0; i < N; i++) {
		pairs[i].second += M * int64_t(N-i-1);
	}
	for (int i = 0; i+1 < N; i++) {
		pairs[i].second -= pairs[i+1].second;
	}
	int64_t cur_plus = M + pairs[0].second - pairs[0].first;
	ops.push_back(cur_plus);
	for (int i = 1; i < N; i++) {
		ops.push_back(pairs[i].first + cur_plus);
		ops.push_back(pairs[i].second);
		cur_plus += pairs[i].second;
	}

	ops.push_back(M);

	assert(int(ops.size()) == 2 * N);
	cout << "Yes" << '\n';
	cout << ops.size() / 2 << '\n';
	for (int i = 0; i < int(ops.size()); i += 2) {
		cout << ops[i] % ops[i+1] << ' ' << ops[i+1] << '\n';
	}

	return 0;
}
