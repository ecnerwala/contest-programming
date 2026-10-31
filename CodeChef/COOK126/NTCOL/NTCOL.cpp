#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		int R, G, B; cin >> R >> G >> B;
		array<pair<int, char>, 3> colors{pair<int, char>{R,'R'},{G,'G'},{B,'B'}};
		if (colors[0] > colors[1]) swap(colors[0], colors[1]);
		if (colors[2] > colors[1]) swap(colors[2], colors[1]);
		vector<string> X(N, string(M, '\0'));
		int c = 0;
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					if (((i+j)&1) != z) continue;
					while (colors[c].first == 0) c++;
					--colors[c].first;
					X[i][j] = colors[c].second;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			cout << X[i] << '\n';
		}
	}

	return 0;
}
