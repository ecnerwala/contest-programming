#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, R; cin >> N >> R;
		int num_pairs = 0;
		int num_singles = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			num_pairs += a / 2;
			num_singles += a % 2;
		}
		R -= num_pairs;
		assert(2 * R >= num_singles);
		cout << num_pairs * 2 + num_singles - std::max((num_singles - R) * 2, 0) << '\n';
	}

	return 0;
}
