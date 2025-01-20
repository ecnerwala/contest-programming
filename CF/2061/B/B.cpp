#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		std::optional<std::array<int, 4>> ans;
		auto check = [&](int i, int j, int k, int l) -> void {
			if (ans) return;
			if ((A[i] == A[j] || A[j] == A[k] || A[k] == A[l]) && 2 * std::max({A[i], A[j], A[k], A[l]}) > (A[i] + A[j] + A[k] + A[l])) {
				ans = {A[i], A[j], A[k], A[l]};
			}
		};
		for (int i = 2; i+1 < N; i++) {
			check(0, 1, i, i+1);
		}
		for (int i = 3; i+1 < N; i++) {
			check(1, 2, i, i+1);
		}
		check(0, 1, 2, 3);
		if (ans) {
			cout << (*ans)[0] << ' ' << (*ans)[1] << ' ' << (*ans)[2] << ' ' << (*ans)[3] << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
