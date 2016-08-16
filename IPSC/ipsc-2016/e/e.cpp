#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXR = 30;
const int MAXC = 30;

int R, C, M, AR, AC;

bool mine[MAXR][MAXC];

string board_str() {
	string res;
	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			res += (mine[i][j] ? '*' : '.');
		}
		res += '\n';
	}
	return res;
}

int main() {

	int T; cin >> T;
	while(T--) {
		cin >> R >> C >> M >> AR >> AC;
		vector<pii> locs;
		for(int i = 0; i < R; i++) {
			for(int j = 0; j < C; j++) {
				if(abs(i - AR) <= 2 && abs(j - AC) <= 2) continue;
				locs.emplace_back(i, j);
			}
		}

		cout << locs.size() + 2 << '\n';
		assert(M >= 8);

		memset(mine, 0, sizeof(mine));
		for(int i = 0; i < M - 8; i++) {
			mine[locs[i].A][locs[i].B] = true;
		}

		mine[AR - 2][AC - 2] = true;
		mine[AR - 2][AC + 2] = true;
		mine[AR + 2][AC - 2] = true;
		mine[AR + 2][AC + 2] = true;

		mine[AR - 2][AC - 1] = true;
		mine[AR + 2][AC + 1] = true;
		mine[AR - 1][AC + 2] = true;
		mine[AR + 1][AC - 2] = true;

		cout << board_str();

		memset(mine, 0, sizeof(mine));
		for(int i = 0; i < M - 8; i++) {
			mine[locs[i].A][locs[i].B] = true;
		}

		mine[AR - 2][AC - 2] = true;
		mine[AR - 2][AC + 2] = true;
		mine[AR + 2][AC - 2] = true;
		mine[AR + 2][AC + 2] = true;

		mine[AR - 2][AC + 1] = true;
		mine[AR + 2][AC - 1] = true;
		mine[AR - 1][AC - 2] = true;
		mine[AR + 1][AC + 2] = true;

		cout << board_str();

		for(int s = 0; s < int(locs.size()); s++) {
			memset(mine, 0, sizeof(mine));
			for(int i = 0, p = s; i < M - 4; i++, p++, p %= int(locs.size())) {
				mine[locs[p].A][locs[p].B] = true;
			}
			mine[AR - 2][AC] = true;
			mine[AR + 2][AC] = true;
			mine[AR][AC - 2] = true;
			mine[AR][AC + 2] = true;
			cout << board_str();
		}
	}

	return 0;
}
