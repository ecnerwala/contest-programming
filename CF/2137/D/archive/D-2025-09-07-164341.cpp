#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> val(N+1);
		int nxt = 0;
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) {
			int b; cin >> b;
			if (val[b].first == 0) {
				val[b] = {++nxt, b};
			}
			A[i] = val[b].first;
			if (--val[b].second == 0) {
				val[b] = {0, 0};
			}
		}
		bool good = true;
		for (int i = 0; i <= N; i++) {
			if (val[i].first != 0) good = false;
		}
		if (good) {
			for (int i = 0; i < N; i++) cout << A[i] << " \n"[i+1==N];
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
