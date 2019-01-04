#include<bits/stdc++.h>
using namespace std;

const int MAXN = 30;
const int MAXM = 30;
int N, M;
int X[MAXN][MAXM];

int cost;
vector<vector<int>> answer;
void doCycle(vector<pair<int, int>> vec) {
	vector<int> vals;
	for (auto it : vec) {
		assert(0 <= it.first && it.first < N);
		assert(0 <= it.second && it.second < M);
		vals.push_back(X[it.first][it.second]);
	}
	answer.push_back(vals);
	cost += int(vals.size());
	for (int i = 0; i < int(vec.size()); i++) {
		auto it = vec[(i+1) % int(vec.size())];
		X[it.first][it.second] = vals[i];
	}

	/*
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cerr << X[i][j]+1 << ' ';
		}
		cerr << '\n';
	}
	cerr << '\n';
	*/
}

void cycleRight(int i, int j) {
	assert(0 <= i);
	assert(0 <= j);
	assert(i+1 < N);
	assert(j+1 < M);
	doCycle(vector<pair<int, int>>{{i, j}, {i, j+1}, {i+1, j+1}, {i+1, j}});
}
void cycleDown(int i, int j) {
	assert(0 <= i);
	assert(0 <= j);
	assert(i+1 < N);
	assert(j+1 < M);
	doCycle(vector<pair<int, int>>{{i, j}, {i+1, j}, {i+1, j+1}, {i, j+1}});
}

// swap the bottom 2
void swapBot() {
	doCycle(vector<pair<int, int>>{{N-2, M-3}, {N-2, M-2}, {N-2, M-1}, {N-1, M-1}, {N-1, M-2}, {N-1, M-3}});
	cycleDown(N-2, M-3);
	cycleDown(N-2, M-2);
}
// swap the right 2
void swapRight() {
	cycleRight(N-2, M-2);
	swapBot();
	cycleDown(N-2, M-2);
}
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> X[i][j];
			X[i][j] --;
		}
	}

	for (int di = 0; di+2 < N; di++) {
		for (int dj = 0; dj < M; dj++) {
			int v = di * M + dj;
			int si, sj;
			for (si = 0; si < N; si++) {
				for (sj = 0; sj < M; sj++) {
					if (X[si][sj] == v) {
						goto foundSiSj;
					}
				}
			}
			assert(false);
foundSiSj:
			assert(X[si][sj] == v);
			assert(si >= di);
			if (si == di && sj == dj) continue;
			// move it to the correct col
			while (sj < dj) {
				// move it right
				if (si == N-1) {
					cycleDown(si-1, sj);
				} else {
					cycleRight(si, sj);
				}
				sj++;
				assert(X[si][sj] == v);
			}
			while (sj > dj) {
				// move it left
				if (si == N-1) {
					cycleRight(si-1, sj-1);
				} else {
					cycleDown(si, sj-1);
				}
				sj--;
				assert(X[si][sj] == v);
			}
			assert(X[si][sj] == v);
			assert(sj == dj);

			if (dj == M-1) {
				assert(si != di);
				assert(si > di);
				while (si > di+2) {
					cycleDown(si-1, sj-1);
					si--;
					assert(X[si][sj] == v);
				}
				if (si == di+1) {
					cycleRight(si, sj-1);
					si++;
					assert(X[si][sj] == v);
				}
				assert(si == di+2);
				assert(sj == dj);
				assert(X[di][dj-1] == v-1);

				cycleRight(di, dj-1);
				cycleRight(di, dj-1);
				doCycle(vector<pair<int, int>>{{di, dj-1}, {di+1, dj-1}, {di+2, dj-1}, {di+2, dj}, {di+1, dj}, {di, dj}});
				doCycle(vector<pair<int, int>>{{di, dj-1}, {di+1, dj-1}, {di+2, dj-1}, {di+2, dj}, {di+1, dj}, {di, dj}});
				assert(X[di][dj-1] == v-1);
				assert(X[di][dj] == v);
			} else {
				while (si > di) {
					cycleRight(si-1, sj);
					si--;
					assert(X[si][sj] == v);
				}
				assert(si == di && sj == dj);
			}
		}
	}

	// all but the last two rows are done
	for (int dj = 0; dj+2 < M; dj++) {
		for (int di = N-2; di < N; di++) {
			int v = di * M + dj;
			int si, sj;
			for (si = 0; si < N; si++) {
				for (sj = 0; sj < M; sj++) {
					if (X[si][sj] == v) {
						goto foundSiSj2;
					}
				}
			}
			assert(false);
foundSiSj2:
			assert(X[si][sj] == v);
			if (si == di && sj == dj) continue;
			if (di == N-2) {
				if (si == N-1) {
					if (sj == M-1) {
						cycleDown(si-1, sj-1);
						si--;
						assert(X[si][sj] == v);
					} else {
						cycleRight(si-1, sj);
						si--;
						assert(X[si][sj] == v);
					}
				}
				assert(si == N-2);
				assert(X[si][sj] == v);
				assert(si == di);
				assert(sj >= dj);
				while (sj > dj) {
					cycleDown(si, sj-1);
					sj--;
					assert(X[si][sj] == v);
				}
				assert(si == di && sj == dj);
			} else {
				assert(di == N-1);
				if (sj == dj) continue;
				assert(sj > dj);
				if (si == N-2) {
					if (sj == M-1) {
						cycleRight(si, sj-1);
						si++;
						assert(X[si][sj] == v);
					} else {
						cycleDown(si, sj);
						si++;
						assert(X[si][sj] == v);
					}
				}
				assert(si == N-1);
				assert(X[si][sj] == v);
				if (sj == dj + 1) {
					cycleDown(si-1, sj);
					sj++;
					assert(X[si][sj] == v);
				}
				while (sj > dj + 2) {
					cycleRight(si-1, sj-1);
					sj--;
					assert(X[si][sj] == v);
				}
				assert(X[si][sj] == v);
				assert(sj == dj + 2);
				cycleRight(di-1, dj);
				cycleRight(di-1, dj);
				doCycle(vector<pair<int, int>>{{di, dj}, {di-1, dj}, {di-1, dj+1}, {di-1, dj+2}, {di, dj+2}, {di, dj+1}});
				doCycle(vector<pair<int, int>>{{di, dj}, {di-1, dj}, {di-1, dj+1}, {di-1, dj+2}, {di, dj+2}, {di, dj+1}});
			}
		}
	}

	// now we just have the bottom right
	int va = (N-2) * M + (M-2);
	int vb = (N-2) * M + (M-1);
	int vc = (N-1) * M + (M-2);
	int vd = (N-1) * M + (M-1);
	while (X[N-2][M-2] != va) {
		cycleRight(N-2, M-2);
	}
	assert(X[N-2][M-2] == va);
	if (X[N-1][M-2] == vd) swapBot();
	if (X[N-2][M-1] == vd) swapRight();
	assert(X[N-2][M-2] == va);
	assert(X[N-1][M-1] == vd);
	if (X[N-2][M-1] != vb) {
		swapBot();
		swapRight();
		swapBot();
	}
	assert(X[N-2][M-1] == vb);
	assert(X[N-1][M-2] == vc);

	cerr << cost << ' ' << "moves" << '\n';
	cout << answer.size() << '\n';
	for (const auto& vec : answer) {
		cout << vec.size();
		for (int v : vec) {
			cout << ' ' << v+1;
		}
		cout << '\n';
	}
	return 0;
}
