#include <bits/stdc++.h>

int main() {
	int N, M, K; std::cin >> N >> M >> K;
	std::vector<int> A(N);
	for (auto& a : A) { std::cin >> a; }
	std::sort(A.begin(), A.end(), std::greater());
	assert(N >= 2);
	int reps = M / (2*K);
	int64_t ans = int64_t(A[0] + A[1]) * K * reps;
	M %= (2*K);
	std::cout << ans << '\n';
}
