#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	
	while (T--) {
		int w1, w2, x1, x2, M; cin >> w1 >> w2 >> x1 >> x2 >> M;

		cout << (w1 + x1 * M <= w2 && w2 <= w1 + x2 * M ? 1 : 0) << '\n';
	}

	return 0;
}
