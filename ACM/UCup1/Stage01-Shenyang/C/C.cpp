#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, D; cin >> N >> D;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	int64_t best = 0;
	auto test = [&](int64_t l) -> void {
		int64_t tot = 0;
		for (int i = 0; i+1 < N; i++) {
			tot += std::abs(
				std::clamp<int64_t>(A[i], l, l+D)
				- std::clamp<int64_t>(A[i+1], l, l+D)
			);
		}
		if (tot > best) best = tot;
	};
	for (int a : A) {
		test(a-D);
		test(a);
	}
	cout << best << '\n';

	return 0;
}
