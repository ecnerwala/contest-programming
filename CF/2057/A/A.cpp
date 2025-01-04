#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		if (std::min(N, M) == 1) {
			cout << 1 + std::max(N, M) << '\n';
		} else {
			cout << std::max(N, M) << '\n';
		}
	}

	return 0;
}
