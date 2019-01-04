#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e5 + 200;
int N[2];
char S[2][MAXN];
ll NC[2][2];

const int MAXL = 50;
const int L = 42;
ll NX[MAXL][2][MAXN][2];
ll T[MAXL][2][2];

int query_round(ll a, int z, int rounds) {
	assert(0 <= a);
	assert(a < T[rounds][z][0] + T[rounds][z][1]);
	if (rounds == 0) return z;
	assert(rounds >= 1);
	int mi = 0;
	int ma = N[z];
	assert(NX[rounds][z][mi][0] + NX[rounds][z][mi][1] <= a);
	assert(NX[rounds][z][ma][0] + NX[rounds][z][ma][1] > a);
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (NX[rounds][z][md][0] + NX[rounds][z][md][1] <= a) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return query_round(a - NX[rounds][z][mi][0] - NX[rounds][z][mi][1], S[z][mi], rounds-1);
}

int query(ll a) {
	assert(a >= 1);
	a --;
	int rounds = 0;
	while (a >= T[rounds][0][0] + T[rounds][0][1]) {
		a -= T[rounds][0][0] + T[rounds][0][1];
		rounds++;
	}
	return query_round(a, 0, rounds);
}

int main() {
	for (int z = 0; z < 2; z++) {
		scanf(" %s", S[z]);
		N[z] = int(strlen(S[z]));
		NC[z][0] = NC[z][1] = 0;
		for (int i = 0; i < N[z]; i++) {
			assert('0' <= S[z][i] && S[z][i] <= '1');
			S[z][i] -= '0';
			NC[z][S[z][i]] ++;
		}
	}
	T[0][0][0] = 1;
	T[0][1][1] = 1;

	T[0][0][1] = 0;
	T[0][1][0] = 0;

	for (int l = 0; l < L; l++) {
		for (int z = 0; z < 2; z++) {
			NX[l+1][z][0][0] = NX[l+1][z][0][1] = 0;
			for (int i = 0; i < N[z]; i++) {
				NX[l+1][z][i+1][0] = NX[l+1][z][i][0] + T[l][S[z][i]][0];
				NX[l+1][z][i+1][1] = NX[l+1][z][i][1] + T[l][S[z][i]][1];
			}
			T[l+1][z][0] = NX[l+1][z][N[z]][0];
			T[l+1][z][1] = NX[l+1][z][N[z]][1];
		}
	}

	int Q; scanf("%d", &Q);
	for (int i = 0; i < Q; i++) {
		ll a; scanf("%lld", &a);
		printf("%d\n", query(a));
	}

	return 0;
}
