#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::string> G(N);
		for (auto& s : G) {
			s.reserve(M);
			cin >> s;
			assert(int(s.size()) == M);
		}
		std::array<int, 5> freq; freq.fill(0);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int v = (i*2+j)%5;
				if (G[i][j] == '#') {
					freq[v]++;
				} else if (G[i][j] == '.') {
					if (i+1 < N && G[i+1][j] == '#') freq[v]++;
					if (i-1 >=0 && G[i-1][j] == '#') freq[v]++;
					if (j+1 < M && G[i][j+1] == '#') freq[v]++;
					if (j-1 >=0 && G[i][j-1] == '#') freq[v]++;
				} else assert(false);
			}
		}
		int best_v = int(std::min_element(freq.begin(), freq.end()) - freq.begin());
		std::vector<std::string> R = G;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int v = (i*2+j)%5;
				if (v == best_v) {
					if (G[i][j] == '#') {
						R[i][j] = 'S';
					} else if (G[i][j] == '.') {
						if (i+1 < N && G[i+1][j] == '#') R[i+1][j] = 'S';
						if (i-1 >=0 && G[i-1][j] == '#') R[i-1][j] = 'S';
						if (j+1 < M && G[i][j+1] == '#') R[i][j+1] = 'S';
						if (j-1 >=0 && G[i][j-1] == '#') R[i][j-1] = 'S';
					} else assert(false);
				}
			}
		}
		for (auto& row : R) cout << row << '\n';
	}

	return 0;
}
