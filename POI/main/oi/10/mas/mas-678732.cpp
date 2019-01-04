#include<bits/stdc++.h>
using namespace std;

const int MAXC = 20;
int C;
int A[MAXC];
int B[MAXC];
int V[MAXC][MAXC];
bool has[MAXC][MAXC];

bool used[MAXC];

bool dfs(int pos) {
	if(pos == 0) {
		for(int i = 0; i < C; i++) {
			if(A[i] || B[i]) return false;
		}
		return true;
	}
	for(int i = 0; i < C; i++) {
		if(A[i] < 0 || B[i] < 0) return false;
	}
	pos--;
	for(int d = 1; d <= 9; d++) {
		if(used[d]) continue;
		used[d] = true;
		for(int i = 0; i < C; i++) {
			if(V[i][pos] == d) {
				A[i] -= d;
			} else if(has[i][d]) {
				B[i] -= d;
			}
		}
		if(dfs(pos)) {
			if(pos) cout << ' ';
			cout << d;
			return true;
		}
		for(int i = 0; i < C; i++) {
			if(V[i][pos] == d) {
				A[i] += d;
			} else if(has[i][d]) {
				B[i] += d;
			}
		}
		used[d] = false;
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> C;
	for(int i = 0; i < C; i++) {
		cin >> A[i] >> B[i];
		for(int j = 0; j < C; j++) {
			cin >> V[i][j];
			has[i][V[i][j]] = true;
		}
	}
	assert(dfs(C));
	cout << '\n';
	return 0;
}

