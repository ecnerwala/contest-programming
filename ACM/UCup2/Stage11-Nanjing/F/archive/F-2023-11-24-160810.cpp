#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int M; cin >> M;
		std::vector<std::vector<int>> ops(N);
		for (int i = 0; i < N; i++) {
			int l; cin >> l;
			ops[i].resize(l);
			for (auto& v : ops[i]) { cin >> v; v--; }
		}
		std::vector<int> last_val(M, -1);
		for (int i = 0; i < N; i++) {
			for (auto v : ops[i]) last_val[v] = i;
		}
		int swap_idx = -1;
		for (int i = 0; i+1 < N; i++) {
			bool bad = false;
			for (int v : ops[i]) {
				if (last_val[v] == i+1) {
					bad = true;
					break;
				}
			}
			if (bad) continue;
			swap_idx = i;
			break;
		}
		if (swap_idx == -1) {
			cout << "No" << '\n';
		} else {
			cout << "Yes" << '\n';
			for (int i = 0; i < N; i++) {
				cout << (i == swap_idx ? i + 1 : i == swap_idx + 1 ? i - 1 : i) + 1 << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
