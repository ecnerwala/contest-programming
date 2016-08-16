#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 9;

inline ll inv(ll a, ll m = MOD) { return (a == 1) ? 1 : m - inv(m % a, a) * m / a; }

ll choose(ll a, ll b) {
	ll res = 1;
	for(int i = 0; i < b; i++) {
		res *= a - i;
		res %= MOD;
	}
	for(int i = 1; i <= b; i++) {
		res *= inv(i);
		res %= MOD;
	}
	return res;
}

ll f[200000];
ll slow(ll n) {
	if(f[n]) return f[n];
	f[n] = 0;
	for(int i = 0; i < n; i++) {
		for(int j = i + 1; j < n; j++) {
			f[n] += slow(j - i) * slow(n - (j - i)) % MOD * choose(n - 2, j - i - 1);
			f[n] %= MOD;
		}
	}
	return f[n];
}

ll exp(ll a, ll e) {
	ll r = 1;
	for(; e; a *= a, a %= MOD, e /= 2) {
		if(e & 1) r *= a, r %= MOD;
	}
	return r;
}

ll fast(ll n) {
	return exp(n, n-2);
}

void test() {
	f[1] = 1;
	for(int i = 1; i < 20; i++) {
		cerr << i << ' ' << fast(i) << ' ' << slow(i) << '\n';
		assert(fast(i) == slow(i));
	}
}

const int MAXN = 2e5;

int N;
int P[MAXN];
bool vis[MAXN];

int main() {
	//test();
	int T; cin >> T;
	while(T--) {
		cin >> N;
		for(int i = 0; i < N; i++) {
			cin >> P[i]; P[i] --;
			vis[i] = false;
		}
		ll tot = 0;
		ll res = 1;
		for(int i = 0; i < N; i++) {
			if(vis[i]) continue;
			int cnt = 1;
			vis[i] = true;
			for(int j = P[i]; j != i; j = P[j]) {
				cnt++;
				vis[j] = true;
			}
			tot += cnt - 1;
			res *= choose(tot, cnt - 1);
			res %= MOD;
			res *= fast(cnt);
			res %= MOD;
		}
		cout << res << '\n';
	}
	return 0;
}
