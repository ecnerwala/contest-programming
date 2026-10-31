#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N-1);
	for (auto& a : A) cin >> a;

	int64_t a_lo = 1;
	int64_t a_hi = A[0] - 1;
	for (int i = 0; i < N-1; i++) {
		std::tie(a_lo, a_hi) = std::make_pair(A[i] - a_hi, A[i] - a_lo);
		a_lo = std::max<int64_t>(a_lo, 1);
	}
	cout << std::max<int64_t>(0, a_hi - a_lo + 1) << '\n';

	return 0;
}
