#include<bits/stdc++.h>
using namespace std;

const int MAXR = 5000;
const int MAXC = 8000;

int R, C;
int horiz[MAXR][MAXC];
int path[MAXR][MAXC][2][2]; // 1 is down, 1 is right

int bit[MAXC];
// gets from a onwards
int get(int a) {
	assert(0 <= a && a < MAXC);
	int res = 0;
	a++;
	for (; a < MAXC; a += (a & -a)) {
		res += bit[a];
	}
	return res;
}

void add(int a, int v) {
	assert(0 <= a && a < MAXC);
	a++;
	for (; a; a -= (a & -a)) {
		bit[a] += v;
	}
}

void reset() {
	memset(bit, 0, sizeof(bit));
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> R >> C;
	{ string S; getline(cin, S); }
	for (int i = 0; i < R; i++) {
		string S;
		if (i) {
			getline(cin, S);
			for (int j = 1; j < int(S.size()); j+=2) {
				if (S[j] == ' ') {
					continue;
				} else if (S[j] == '/') {
					assert((i-1) % 2 == (j+1)/2 % 2);
					assert(i % 2 == (j-1)/2 % 2);
					path[i-1][(j+1)/2][1][0] = true;
					path[i][(j-1)/2][0][1] = true;
				} else if (S[j] == '\\') {
					assert((i-1) % 2 == (j-1)/2 % 2);
					assert(i % 2 == (j+1)/2 % 2);
					path[i-1][(j-1)/2][1][1] = true;
					path[i][(j+1)/2][0][0] = true;
				} else assert(false);
			}
		}
		getline(cin, S);
		for (int j = i%2; j < C; j += 2) {
			assert(S[2 * j] == 'x');
			if (j + 2 < C && S[2 * j + 2] == '-') {
				horiz[i][j+2] = true;
			}
		}
	}

	for (int i = 0; i < R; i++) {
		for (int j = i % 2; j < C; j += 2) {
			assert(path[i][j][0][0] <= 1);
			if (path[i][j][0][0]) {
				path[i][j][0][0] += path[i-1][j-1][0][0];
			}
			assert(path[i][j][0][1] <= 1);
			if (path[i][j][0][1]) {
				path[i][j][0][1] += path[i-1][j+1][0][1];
			}
		}
	}

	for (int i = R-1; i >= 0; i--) {
		for (int j = i % 2; j < C; j += 2) {
			assert(path[i][j][1][0] <= 1);
			if (path[i][j][1][0]) {
				path[i][j][1][0] += path[i+1][j-1][1][0];
			}
			assert(path[i][j][1][1] <= 1);
			if (path[i][j][1][1]) {
				path[i][j][1][1] += path[i+1][j+1][1][1];
			}
		}
	}

	for (int i = 0; i < R; i++) {
		for (int j = i % 2; j < C; j += 2) {
			assert(horiz[i][j] <= 1);
			if (horiz[i][j]) {
				horiz[i][j] += horiz[i][j-2];
			}
			path[i][j][0][0] = min(path[i][j][0][0], horiz[i][j]);
			path[i][j][1][0] = min(path[i][j][1][0], horiz[i][j]);
		}
	}

	long long res = 0;
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < R; i++) {
			priority_queue<pair<int, int>> pq;
			reset();
			for (int j = i % 2; j < C; j += 2) {
				res += get(j - 2 * path[i][j][z][0]);

				add(j, 1);
				pq.emplace(-(j + 2 * path[i][j][z][1]), j);
				while (!pq.empty() && -pq.top().first <= j) {
					add(pq.top().second, -1);
					pq.pop();
				}
			}
		}
	}
	printf("%lld\n", res);
	return 0;
}
