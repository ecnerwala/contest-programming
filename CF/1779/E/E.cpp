#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> deg(N);
	for (int i = 0; i < N; i++) {
		cout << '?' << ' ' << i+1 << ' ';
		for (int j = 0; j < N; j++) {
			cout << (j != i);
		}
		cout << '\n' << flush;
		cin >> deg[i].first;
		deg[i].second = i;
	}
	std::sort(deg.begin(), deg.end());
	std::vector<int> c(N);
	int tgt_wins = 0;
	int actual_wins = 0;
	for (int z = N-1; z >= 0; z--) {
		tgt_wins += z;
		actual_wins += deg[z].first;
		c[deg[z].second] = true;
		if (tgt_wins == actual_wins) {
			break;
		}
	}

	cout << '!' << ' ';
	for (int i = 0; i < N; i++) cout << c[i];
	cout << '\n' << flush;

	return 0;
}
