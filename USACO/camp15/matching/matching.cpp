#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e5;
const int MAXL = 30;

string S;
ll N; // length of S
const ll L = 26;
ll trans[MAXN][MAXL];

string T;
ll M; // length of T

ll R; // number of repetitions

int main() {
	cin >> R >> S >> T;

	N = S.size();
	M = T.size();

	// failure and trans intialized to 0
	for(ll i = 0; i < N; i++) {
		ll c = S[i] - 'a';
		ll v = trans[i][c];
		trans[i][c] = i + 1;
		memcpy(trans[i + 1], trans[v], sizeof(trans[0]));
	}

	ll res = 0;
	for(ll l = 0, r = 0, len = 0; l < M && r <= R * M; len = trans[len][T[r % M] - 'a'], r++, l = r - len) {
		// string is [l, r), length len
		assert(r - l == len);

		if(len == N) {
			// M * k + r <= MR
			// k <= (MR - r) / M
			// k <= floor((MR - r) / M)
			// k is from [0.. floor((MR - r) / M)]
			
			res += (R * M - r) / M + 1;
		}
	}

	cout << res << '\n';
	return 0;
}
