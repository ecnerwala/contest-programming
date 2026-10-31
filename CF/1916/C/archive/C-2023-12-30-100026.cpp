#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		int64_t cur_sum = 0;
		int cur_even = 0;
		int cur_odd = 0;
		for (int i = 1; i <= N; i++) {
			cur_sum += A[i-1];
			cur_odd += (A[i-1] & 1);
			cur_even += !(A[i-1] & 1);

			int64_t ans = cur_sum - (cur_odd / 3) - (cur_odd % 3 == 1);
			cout << ans << " \n"[i==N];
		}
	}

	return 0;
}
