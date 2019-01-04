#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1010, MAXM = 1010;
int N, M;
char G[MAXN][MAXM+1]; // +1 for null terminator

const int MAXA = 1010, MAXB = 1010;
int A, B;
char S[MAXA][MAXB+1]; // +1 for null terminator

bool go() {
	vector<pair<int, int>> pts;
	for (int i = 0; i < A; i++) {
		for (int j = 0; j < B; j++) {
			if (S[i][j] == 'x') {
				pts.emplace_back(i, j);
			} else if (S[i][j] == '.') {
				// pass
			} else assert(false);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '.') continue;
			assert(G[i][j] == 'x');
			if (pts.empty()) return false;
			for (auto pt : pts) {
				int x = i + pt.first - pts[0].first;
				int y = j + pt.second - pts[0].second;
				if (0 <= x && x < N && 0 <= y && y < M && G[x][y] == 'x') {
					G[x][y] = '.';
				} else {
					return false;
				}
			}
			assert(G[i][j] == '.');
		}
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	int Q; cin >> Q;
	for (int i = 0; i < Q; i++) {
		cin >> N >> M >> A >> B;
		for (int i = 0; i < N; i++) cin >> G[i];
		for (int i = 0; i < A; i++) cin >> S[i];

		bool res = go();
		cout << (res ? "TAK" : "NIE") << '\n';
	}
	return 0;
}
