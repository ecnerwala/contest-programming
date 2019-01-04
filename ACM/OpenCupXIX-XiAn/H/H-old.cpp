#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4000;
const int MAXM = 30;
int Norig, M;

const int MAXH = MAXN * MAXM;
char stacks[MAXM][MAXH];
int H[MAXM];

int origSize[MAXM];

vector<pair<pair<char, int>, int>> answer;

const int THRESH[] = {5, 2, 5, 2, 5, 2}; // "ABCDEF";

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

int pickedCnt[MAXM];

char activated(char a) {
	if ('A' <= a && a <= 'F') {
		return a ^ 32;
	}
	return a;
}
char deactivated(char a) {
	if ('a' <= a && a <= 'f') {
		return a ^ 32;
	}
	return a;
}

void printGrid() {
	for (int m = 1; m <= M; m++) {
		for (int i = 0; i < H[m]; i++) {
			cerr << stacks[m][i];
		}
		cerr << '\n';
	}
}

int oldH[MAXM];
int saveBot[MAXM];
char oldStacks[MAXM][MAXH];

void saveTo(int m, int v) {
	assert(1 <= m && m <= M);
	assert(v >= 0);
	while (v < saveBot[m]) {
		saveBot[m] --;
		oldStacks[m][saveBot[m]] = stacks[m][saveBot[m]];
	}
}

vector<pair<int, int>> flooded;
int flood(int x, int y, char oldVal, char newVal) {
	if (!(1 <= x && x <= M && 0 <= y && y < H[x])) {
		return 0;
	}
	if (deactivated(stacks[x][y]) != oldVal) {
		return 0;
	}
	int res = 1;
	saveTo(x, y);
	stacks[x][y] = newVal;
	flooded.emplace_back(x,y);
	for (int d = 0; d < 4; d++) {
		res += flood(x + mx[d], y + my[d], oldVal, newVal);
	}
	return res;
}

int minRemoved[MAXM];

// -1 for bad, >= 0 for score
int round(char T, int X) {
	for (int m = 1; m <= M; m++) {
		oldH[m] = H[m];
		saveBot[m] = oldH[m];
	}
	assert(1 <= X && X <= M);
	int cnt = 0;
	for (int m = 1; m <= M; m++) {
		pickedCnt[m] = 0;
		for (int i = H[m]-1; i >= 0 && stacks[m][i] == T; i--) {
			pickedCnt[m] ++;
			cnt++;
		}
	}
	assert(cnt >= 0);
	if (T == 'X' || T == 'Y') {
		if (cnt < 2) {
			return 0;
		}
		if (pickedCnt[X] == H[X]) {
			return 0;
		}
		char y = stacks[X][H[X] - 1 - pickedCnt[X]];
		if (y == 'X' || y == 'Y') {
			return 0;
		}
		assert('A' <= y && y <= 'F');
	} else {
		if (cnt < 1) {
			return 0;
		}
	}
	// we're always good, so let's just make the moves

	int res = 0;

	vector<pair<int, int>> removed;
	vector<pair<int, int>> wasActivated;

	// pick everything
	for (int m = 1; m <= M; m++) {
		for (int i = H[m] - pickedCnt[m]; i < H[m]; i++) {
			saveTo(m, i);
			stacks[m][i] = '0';
		}
		H[m] -= pickedCnt[m];
	}
	if (T == 'X' || T == 'Y') {
		res += cnt;
		char y = stacks[X][H[X]-1];
		assert('A' <= y && y <= 'F');
		char newVal = '0';
		if (T == 'X' && y < 'F') {
			newVal = char(y+1);
		}
		for (int m = 1; m <= M; m++) {
			for (int i = 0; i < H[m]; i++) {
				if (stacks[m][i] == y) {
					saveTo(m, i);
					stacks[m][i] = activated(newVal);
					if (newVal != '0') wasActivated.emplace_back(m, i);
					removed.emplace_back(m, i);
					res++;
				}
			}
		}
	} else {
		for (int i = 0; i < cnt; i++) {
			saveTo(X, H[X]);
			stacks[X][H[X]] = activated(T);
			wasActivated.emplace_back(X, H[X]);
			H[X]++;
		}
	}

	// now, we activate anything adjacent to an activated thing
	while (true) {
		for (int m = 1; m <= M; m++) {
			minRemoved[m] = H[m];
		}
		// activate everything adjacent to something removed
		for (auto pt : removed) {
			minRemoved[pt.first] = min(minRemoved[pt.first], pt.second);
			for (int d = 0; d < 4; d ++) {
				int nx = pt.first + mx[d];
				int ny = pt.second + my[d];
				if (1 <= nx && nx <= M && 0 <= ny && ny < H[nx]) {
					saveTo(nx, ny);
					char c = stacks[nx][ny];
					stacks[nx][ny] = activated(c);
					if (activated(c) != c) wasActivated.emplace_back(nx, ny);
				}
			}
		}
		removed.clear();
		// everything falls down
		for (int m = 1; m <= M; m++) {
			int j = minRemoved[m];
			for (int i = minRemoved[m]; i < H[m]; i++) {
				if (stacks[m][i] != '0') {
					if (i > j) {
						saveTo(m, j);
						stacks[m][j] = stacks[m][i];
						if ('a' <= stacks[m][j] && stacks[m][j] <= 'f') {
							wasActivated.emplace_back(m, j);
						}
					}
					j++;
				}
			}
			H[m] = j;
		}

		bool didRemove = false;
		vector<pair<pair<int, int>, char>> toAdd;
		for (auto act : wasActivated) {
			int m = act.first, i = act.second;
				char v = stacks[m][i];
				if ('a' <= v && v <= 'f') {
					flooded.clear();
					flood(m, i, deactivated(v), '1');
					if (int(flooded.size()) >= THRESH[v - 'a']) {
						res += int(flooded.size());
						flooded.clear();
						flood(m, i, '1', '0');
						removed.insert(removed.end(), flooded.begin(), flooded.end());
						if (v < 'f') {
							for (auto& it : flooded) {
								swap(it.first, it.second);
							}
							auto pos = *min_element(flooded.begin(), flooded.end());
							swap(pos.first, pos.second);
							for (auto& it : flooded) {
								swap(it.first, it.second);
							}
							toAdd.emplace_back(pos, char(v+1));
						}
						didRemove = true;
					} else {
						flooded.clear();
						flood(m, i, '1', deactivated(v));
					}
				}
		}

		wasActivated.clear();
		for (auto it : toAdd) {
			saveTo(it.first.first, it.first.second);
			stacks[it.first.first][it.first.second] = it.second;
			wasActivated.emplace_back(it.first.first, it.first.second);
		}

		if (!didRemove) break;
	}
	return res;
}

void restore() {
	for (int m = 1; m <= M; m++) {
		assert(saveBot[m] <= oldH[m]);
		memcpy(stacks[m] + saveBot[m], oldStacks[m] + saveBot[m], size_t(oldH[m] - saveBot[m]));
		H[m] = oldH[m];
	}
}

bool round() {
	char bestT = '0';
	int bestX = 0;
	int bestI = 0;
	for (char t = 'X'; t <= 'Y'; t++) {
		for (int m = 1; m <= M; m++) {
			int v = round(t, m);
			restore();
			if (v > bestI) {
				bestI = v;
				bestT = t;
				bestX = m;
			}
		}
	}
	if (bestI > 0) {
		round(bestT, bestX);
		answer.emplace_back(pair<char, int>(bestT, bestX), bestI);
		return true;
	}
	const string order = "ACEBDF";
	for (char t : order) {
		for (int m = 1; m <= M; m++) {
			int v = round(t, m);
			restore();
			if (v > bestI) {
				bestI = v;
				bestT = t;
				bestX = m;
			}
		}
	}
	if (bestI > 0) {
		round(bestT, bestX);
		answer.emplace_back(pair<char, int>(bestT, bestX), bestI);
		return true;
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> Norig >> M;
	for (int h = 1; h <= Norig; h++) {
		string s; cin >> s;
		assert(int(s.size()) == M);
		for (int m = 1; m <= M; m++) {
			stacks[m][H[m]++] = s[m-1];
		}
	}

	//printGrid();
	while (round()) {
		//printGrid();
		//cerr << int(answer.size()) << '\n';
		//auto it = answer.back(); cerr << it.first.first << ' ' << it.first.second << ' ' << it.second << '\n';
	}

	cout << int(answer.size()) << '\n';
	for (auto it : answer) {
		cout << it.first.first << ' ' << it.first.second << ' ' << it.second << '\n';
	}

	return 0;
}
