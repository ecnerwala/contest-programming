#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<int, 64> cnt{};
		for (int i = 0; i < N; i++) {
			int64_t a; cin >> a;
			for (int b = 0; b < 64; b++) {
				cnt[b] += (a >> b) & 1;
			}
		}
		for (int i = 0; true; i++) {
			if (cnt[i] != 0 && cnt[i] != N) {
				cout << (int64_t(1) << (i+1)) << '\n';
				break;
			}
		}
	}

	return 0;
}
