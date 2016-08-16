#include<bits/stdc++.h>
using namespace std;

const int MAXN = 15;
string strings[MAXN][3];
int cnts[MAXN][3][3];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	strings[0][0] = "R";
	cnts[0][0][0] ++;
	strings[0][1] = "P";
	cnts[0][1][1] ++;
	strings[0][2] = "S";
	cnts[0][2][2] ++;
	for(int i = 1; i < MAXN; i++) {
		for(int j = 0; j < 3; j++) {
			string w = strings[i - 1][j];
			string l = strings[i - 1][(j + 2) % 3];
			strings[i][j] = min(w, l) + max(w, l);
			for(int k = 0; k < 3; k++) {
				cnts[i][j][k] = cnts[i - 1][j][k] + cnts[i - 1][(j + 2) % 3][k];
			}
		}
	}

	for(int case_num = 1; case_num <= T; case_num ++) {
		int N, R, P, S;
		cin >> N >> R >> P >> S;
		assert(N < MAXN);
		string res = "IMPOSSIBLE";
		for(int z = 0; z < 3; z++) {
			if(R == cnts[N][z][0] && P == cnts[N][z][1] && S == cnts[N][z][2]) {
				res = strings[N][z];
			}
		}
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
