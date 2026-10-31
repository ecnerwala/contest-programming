#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<int, 3> cnt{0,0,0};
		for (int i = 0; i < N; i++) {
			int a; cin >> a; cnt[a%3] ++;
		}
		if ((cnt[1] - cnt[2]) % 3) {
			cout << -1 << '\n';
		} else {
			cout << (max(cnt[1], cnt[2]) + cnt[1] + cnt[2]) / 3 << '\n';
		}
	}

	return 0;
}
