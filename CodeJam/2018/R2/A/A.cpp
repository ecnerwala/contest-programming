#include<bits/stdc++.h>
using namespace std;

const int MAXC = 200;
int C;
int B[MAXC];
int S[MAXC];
int T[MAXC];

const string BAD = "IMPOSSIBLE";

void go() {
	if (B[0] == 0 || B[C-1] == 0) {
		cout << BAD << '\n';
		return;
	}

	for (int i = 0, c = 0; i < C; i++) {
		S[i] = i;
		for (int z = 0; z < B[i]; z++) {
			T[c++] = i;
		}
	}
	int R = 0;
	string res;
	while (true) {
		string row = "";
		for (int i = 0; i < C; i++) {
			row.push_back('.');
		}
		bool changed = false;
		for (int i = 0; i < C; i++) {
			if (T[i] == S[i]) {
				// pass
			} else if (T[i] > S[i]) {
				changed = true;
				row[S[i]] = '\\';
				S[i] ++;
			} else if (T[i] < S[i]) {
				changed = true;
				row[S[i]] = '/';
				S[i] --;
			} else assert(false);
		}
		res += row;
		res.push_back('\n');
		R += 1;
		if (!changed) break;
	}
	cout << R << '\n';
	cout << res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int TC; cin >> TC;

	for (int case_num = 1; case_num <= TC; case_num ++) {
		cin >> C;
		for (int i = 0; i < C; i++) cin >> B[i];

		cout << "Case #" << case_num << ": ";

		go();
	}

	return 0;
}
