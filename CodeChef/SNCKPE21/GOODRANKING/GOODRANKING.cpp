#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> G(N);
		std::vector<bool> type(N);
		for (int i = 0; i < N; i++) {
			int num_loss = 0;
			cin >> G[i];
			for (char c : G[i]) num_loss += (c == '0');
			num_loss -= (G[i][i] == '0');
			type[i] = (num_loss >= (N+1)/2);
		}
		std::vector<std::tuple<bool, int, int>> to_sort(N);
		for (int i = 0; i < N; i++) {
			int num_loss = 0;
			for (int j = 0; j < N; j++) {
				if (j == i) continue;
				if (type[j] == type[i]) num_loss += (G[i][j] == '0');
			}
			to_sort[i] = {type[i], num_loss, i};
		}
		std::sort(to_sort.begin(), to_sort.end());
		std::vector<int> P(N);
		for (int i = 0; i < N; i++) {
			P[i] = std::get<2>(to_sort[i]);
		}
		bool is_good = true;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N && j <= i + (N+1)/2; j++) {
				if (G[P[i]][P[j]] != '1') is_good = false;
			}
		}
		if (is_good) {
			for (int i = 0; i < N; i++) {
				cout << P[i]+1 << " \n"[i+1==N];
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
