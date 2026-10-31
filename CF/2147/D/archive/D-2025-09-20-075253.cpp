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
		int64_t v_base = 0;
		std::vector<int> extras; extras.reserve(N);
		for (int i = 0, j = i; i < N; i = j) {
			while (j < N && A[j] == A[i]) j++;
			int cnt = j - i;
			v_base += int64_t(A[i] / 2) * int64_t(cnt);
			if (A[i] & 1) extras.push_back(cnt);
		}
		std::sort(extras.begin(), extras.end(), std::greater<>());
		std::array<int64_t, 2> ans{v_base, v_base};
		for (int z = 0; z < int(extras.size()); z++) {
			ans[z & 1] += extras[z];
		}
		cout << ans[0] << ' ' << ans[1] << '\n';
	}

	return 0;
}
