#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4000;
int N;
char G[2][MAXN];

bool go() {
	cin >> G[0] >> G[1];
	N = int(strlen(G[0]));
	assert(G[0][0] == 'A');
	assert(G[0][N-1] == 'B');
	for (int i = 0; i < N; i++) {
		if (G[1][i] == '#') return false;
	}
	int numEmpty = 0;
	for (int i = 0; i < N; i++) {
		for (int r = 0; r < 2; r++) {
			if (G[r][i] == '.') numEmpty++;
		}
	}
	numEmpty++; // A itself gets to count
	int cur = 0;
	for (int i = 1; i < N; i++) {
		if (G[0][i] == '#') continue;
		if (numEmpty >= i - cur + 3) {
			// we're good
		} else {
			return false;
		}
		cur = i;
	}
	assert(cur == N-1);
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N_CASES; cin >> N_CASES;
	for (int case_num = 1; case_num <= N_CASES; case_num ++) {
		bool res = go();
		cout << "Case #" << case_num << ": " << (res ? "Possible" : "Impossible") << '\n';
	}

	return 0;
}
