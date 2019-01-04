#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e3;
int N;
int S[MAXN][MAXN];

vector<int> adj[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int tot = 0;
		while (tot < N-1) {
			if (tot) {
				char c; cin >> c; // kill the dash
			}
			int s; cin >> s;
			tot += s;
			for (int k = 0; k < s; k++) {
				char c; cin >> c;
				assert(c == (k ? ',' : ':'));
				int j; cin >> j;
				assert(i != j);
				S[i][j] = s;
			}
		}
	}
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (S[i][j] + S[j][i] == N) {
				adj[i].push_back(j);
				adj[j].push_back(i);
				cnt ++;
			}
		}
	}
	if (cnt != N-1) {
		cout << -1 << '\n';
		return 0;
	}

	return 0;
}
