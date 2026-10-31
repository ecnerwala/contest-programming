#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int M; cin >> M;

	vector<int64_t> A(N); for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end(), std::greater<>());
	vector<pair<int64_t, int64_t>> B(M);
	for (auto& b : B) cin >> b.first;
	for (auto& b : B) cin >> b.second;
	std::sort(B.begin(), B.end(), [&](auto& a, auto& b) { return a.second / a.first < b.second / b.first; });

	int64_t ans = 0;

	int64_t cur_A = 0;
	int64_t cur_B = 0;
	int64_t cur_C = 0;
	for (auto b : B) cur_B += b.first;
	auto it = B.begin();

	for (int k = 1; k <= N; k++) {
		cur_A += A[k-1];
		for (; it != B.end() && it->second <= k * it->first; ++it) {
			cur_B -= it->first;
			cur_C += it->second;
		}
		ans = min(ans, cur_C + cur_B * k - cur_A);
	}

	for (auto a : A) ans += a;
	cout << ans << '\n';

	return 0;
}

// Let's consider a cut in the flow graph from candies to children,
//  candies A_1, ... A_k are to the left of the cut, A_{k+1} ... A_N are to the right
//  children B_1, ... B_l are to the left and B_{l+1} ... B_M are to the right
// The value of the cut is (A_{k+1} + ... + A_N) + (C_1 + C_2 + ... + C_l) + k * (B_{l+1} + ... + B_M)
// Let's just write this as sum A + (sum_{children} min(C_i, k * B_i) - sum_{A_1..A_k})
// We need sum_{children} min(C_i, k * B_i) >= sum of k biggest A_i for all k
