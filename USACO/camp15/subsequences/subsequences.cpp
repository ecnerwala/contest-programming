#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

const int MAXL = 60;

ll N;
string S, T;
int L;

ll chars[10][MAXL][MAXL];
ll trs[MAXL][MAXL];

ll res[MAXL][MAXL];

ll tmp[MAXL][MAXL];

// a *= b;
void mult(ll a[MAXL][MAXL], ll b[MAXL][MAXL]) {
	for(int i = 0; i < L; i++) {
		for(int j = 0; j < L; j++) {
			tmp[i][j] = 0;
			for(int k = 0; k < L; k++) {
				tmp[i][j] += a[i][k] * b[k][j];
				tmp[i][j] %= MOD;
			}
		}
	}
	for(int i = 0; i < L; i++) {
		for(int j = 0; j < L; j++) {
			a[i][j] = tmp[i][j];
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> S >> T;
	L = S.size() + 1;

	for(int i = 0; i < L; i++) {
		trs[i][i] = 1;
		res[i][i] = 1;
		for(int c = 0; c < 5; c++) {
			chars[c][i][i] = 1;
		}
	}

	for(int i = 0; i < int(S.size()); i++) {
		chars[S[i] - 'a'][i][i + 1] ++;
	}

	for(char c : T) {
		mult(trs, chars[c - 'a']);
	}


	while(N) {
		if(N % 2) mult(res, trs);
		N /= 2;
		mult(trs, trs);
	}

	cout << res[0][L - 1] << '\n';

}
