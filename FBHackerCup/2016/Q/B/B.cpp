#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e3;

int N;
char C[2][MAXN];

int go() {
	cin >> N;
	cin >> (C[0] + 1) >> (C[1] + 1);
	C[0][0] = C[1][0] = C[0][N + 1] = C[1][N + 1] = 'X';

	int res = 0;

	for(int t = 0; t <= 1; t++) {
		for(int i = 1; i <= N; i++) {
			if(C[t][i] == '.' && C[t][i - 1] == 'X' && C[t][i + 1] == 'X' && C[!t][i] == '.') {
				res++;
				for(int j = i - 1; C[!t][j] == '.'; j--) {
					C[!t][j] = 'X';
				}
				for(int j = i + 1; C[!t][j] == '.'; j++) {
					C[!t][j] = 'X';
				}
				C[t][i] = 'X';
				C[!t][i] = 'X';
			}
		}
	}

	// C[0][N + 2] = C[1][N + 2] = 0; cerr << C[0] << '\n' << C[1] << '\n';
	for(int t = 0; t <= 1; t++) {
		for(int i = 1; i <= N; i++) {
			if(C[t][i] == '.' && C[t][i + 1] == 'X') {
				res++;
			}
		}
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int __T = 1; __T <= T; __T ++) {
		cout << "Case #" << __T << ": " << go() << '\n';
	}

	return 0;
}
