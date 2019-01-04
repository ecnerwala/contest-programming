#include<iostream>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;

const ll BASE = 13; // i like 13, and it's a primitive root
const ll MOD1 = 1e9 + 7;
const ll MOD2 = 1e9 + 9;
const ll MOD[] = {MOD1, MOD2};

const int MAXN = 6e5;
int N;
char S[MAXN];

ll pref[MAXN][2];
ll suff[MAXN][2];

ll bases[MAXN][2];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	cin >> S;

	for(int t = 0; t < 2; t++) {
		bases[0][t] = 1;
		for(int i = 1; i <= N; i++) {
			bases[i][t] = (bases[i - 1][t] * BASE) % MOD[t];
		}
		//cerr << "base done\n";

		pref[0][t] = suff[0][t] = 0;
		for(int i = 0; i < N; i++) {
			ll v = S[i] - '0';
			pref[i + 1][t] = (pref[i][t] * BASE + v) % MOD[t];
		}
		//cerr << "pref done\n";
		for(int i = N - 1; i >= 0; i--) {
			ll v = S[i] - '0';
			v = !v;
			suff[i][t] = (suff[i + 1][t] * BASE + v) % MOD[t];
		}
		//cerr << "suff done\n";
	}

	//cerr << "precomp done\n";

	//cerr << "p\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << pref[i][0] << ' ' << pref[i][1] << '\n';
	//cerr << "s\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << suff[i][0] << ' ' << suff[i][1] << '\n';

	ll res = 0;
	for(int i = 1; i < N; i++) {
		int mi = 0;
		int ma = min(i, N - i) + 1;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(
					((pref[i][0] - pref[i - md][0] * bases[md][0]) - 
					(suff[i][0] - suff[i + md][0] * bases[md][0])) % MOD[0] == 0 && 
					((pref[i][1] - pref[i - md][1] * bases[md][1]) - 
					(suff[i][1] - suff[i + md][1] * bases[md][1])) % MOD[1] == 0 
			  ) {
				mi = md;
			} else {
				ma = md;
			}
		}
		res += mi;
	}
	cout << res << '\n';
	return 0;
}

