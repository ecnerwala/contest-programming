#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> ops; ops.reserve(40);
		int v = N;
		while (true) {
			if (v == 1) break;
			if (v % 2 == 0) {
				break;
			}
			if (ops.size() >= 40) break;
			if (v % 4 == 3) {
				ops.push_back(2);
				v = (v - 1) / 2;
			} else {
				ops.push_back(1);
				v = (v + 1) / 2;
			}
		}

		if (v == 1 && ops.size() <= 40) {
			std::reverse(ops.begin(), ops.end());
			cout << ops.size() << '\n';
			for (int i = 0; i < int(ops.size()); i++) {
				cout << ops[i] << " \n"[i+1==int(ops.size())];
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
