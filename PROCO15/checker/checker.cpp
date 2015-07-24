#include<bits/stdc++.h>
using namespace std;

const int INF = 1e8;

int N;

inline bool inrange(int a) { return 0 <= a && a < N; }
inline bool inrange(int a, int b) { return inrange(a) && inrange(b); }

int mx[4] = {-1, -1, 1, 1}, my[4] = {-1, 1, -1, 1};

int go(const vector<string> &B, const int T) {
	if(T == 0) {
		int res = 0;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				char c = B[i][j];
				if(c == 'x') res++;
				else if(c == 'o') res--;
			}
		}
		return res;
	}

	int res = -INF;

	vector<string> C = B;

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(C[i][j] == 'x') {
				C[i][j] = 'o';
			} else if(C[i][j] == 'o') {
				C[i][j] = 'x';
			}
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(B[i][j] == 'x') {
				for(int d = 0; d < 4; d++) {
					int x = i + mx[d], y = j + my[d];
					if(!inrange(x, y)) continue;
					if(B[x][y] == '*') {
						swap(C[x][y], C[i][j]);
						res = max(res, -go(C, T - 1));
						swap(C[x][y], C[i][j]);
					}
					int a = x + mx[d], b = y + my[d];
					if(!inrange(a, b)) continue;
					if(B[x][y] == 'o' && B[a][b] == '*') {
						C[i][j] = '*';
						C[x][y] = '*';
						C[a][b] = 'o';

						res = max(res, -go(C, T - 1));

						C[i][j] = 'o';
						C[x][y] = 'x';
						C[a][b] = '*';
					}
				}
			}
		}
	}
	if(res == -INF) {
		return go(B, 0);
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);

	int K; cin >> K;
	K *= 2;
	cin >> N;
	vector<string> v(N);
	for(int i = 0; i < N; i++) {
		cin >> v[i];
	}
	cout << go(v, K) << '\n';

	return 0;
}
