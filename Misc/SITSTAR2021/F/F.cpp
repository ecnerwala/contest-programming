#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int64_t ans = 0;
	for (int i = M-1; i <= N-1; i += M-1) {
		ans += N - i;
	}
	cout << ans << '\n';

	return 0;
}
