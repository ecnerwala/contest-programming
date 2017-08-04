#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second

const ll INF = 4e18;

const ll MOD = ll(1e9) + 7;

const int MAXN = 300;
ll R, C;
int N;
pair<pll, ll> S[MAXN];
ll D;

ll dist(pll a, pll b) {
	return abs(a.first - b.first) + abs(a.second - b.second);
}

bool is_possible() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (dist(S[i].A, S[j].A) * D < abs(S[i].B - S[j].B)) return false;
		}
	}
	return true;
}

void setmin(ll &a, ll b) {
	if (b < a) a = b;
}

ll go() {
	assert(R < MAXN);
	assert(C < MAXN);
	ll val[MAXN][MAXN];
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			val[i][j] = INF;
		}
	}
	for (int i = 0; i < N; i++) {
		val[S[i].A.A][S[i].A.B] = S[i].B;
	}
	for (int i = 0; i < R; i++) {
		for (int j = 1; j < C; j++) {
			setmin(val[i][j], val[i][j-1] + D);
		}
	}
	for (int i = 0; i < R; i++) {
		for (int j = C-1; j > 0; j--) {
			setmin(val[i][j-1], val[i][j] + D);
		}
	}
	for (int i = 1; i < R; i++) {
		for (int j = 0; j < C; j++) {
			setmin(val[i][j], val[i-1][j] + D);
		}
	}
	for (int i = R-1; i > 0; i--) {
		for (int j = 0; j < C; j++) {
			setmin(val[i-1][j], val[i][j] + D);
		}
	}
	ll res = 0;
	for (ll i = 0; i < R; i++) {
		for (ll j = 0; j < C; j++) {
			res += val[i][j];
			res %= MOD;
		}
	}
	res %= MOD;
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> R >> C >> N >> D;
		for (int i = 0; i < N; i++) {
			cin >> S[i].A.A >> S[i].A.B >> S[i].B;
			S[i].A.A --;
			S[i].A.B --;
		}
		bool poss = is_possible();
		ll res;
		if (poss) {
			res = go();
		}
		cout << "Case #" << case_num << ": ";
		if (poss) {
			cout << res << '\n';
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
