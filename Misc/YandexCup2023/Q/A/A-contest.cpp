#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> last_idx(N+1, N);
	for (int i = 0; i < N; i++) {
		if (A[i] <= N) {
			last_idx[A[i]] = i;
		}
	}
	int ans = 0;
	int cur_idx = 0;
	while (ans+1 <= N && last_idx[ans+1] != N && last_idx[ans+1] >= cur_idx) {
		cur_idx = last_idx[ans+1] + 1;
		ans++;
	}
	cout << ans << '\n';

	return 0;
}
