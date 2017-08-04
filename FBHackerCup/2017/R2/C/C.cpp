#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

struct data{
	ll val[2][2];
};

inline data merge(const data &a, const data &b) {
	data res;
	for (int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			res.val[i][j] = ((a.val[i][0] * b.val[0][j]) + (a.val[i][1] * b.val[1][j])) % MOD;
		}
	}
	return res;
}

void reset (data &a) {
	a.val[0][0] = 1;
	a.val[0][1] = a.val[1][0] = a.val[1][1] = 0;
}

const int L = 21;
const int S = 1 << L;
data dat[2 * S];

inline void update(ll a) {
	dat[a] = merge(dat[2 * a], dat[2 * a + 1]);
}

inline void update_all(ll a) {
	for (a /= 2; a; a /= 2) {
		update(a);
	}
}

const int MAXN = 2e6;
static_assert(S > MAXN, "uh oh!");
ll N;

const int MAXM = 2e6;
ll M;

ll W[MAXM];
ll AW, BW;
ll D[MAXM];
ll AD, BD;
ll Z[MAXM];
ll V[MAXM];
ll AV, BV;

int main() {

	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N; assert(N < MAXN);
		cin >> M; assert(M < MAXM);
		for (int i = 0; i < 2 * S; i++) {
			reset(dat[i]);
		}
		cin >> W[1] >> AW >> BW;
		for (int i = 2; i <= M; i++) W[i] = ((AW * W[i-1] + BW) % N) + 1;
		cin >> D[1] >> AD >> BD;
		for (int i = 2; i <= M; i++) D[i] = ((AD * D[i-1] + BD) % 3);
		for (int i = 1; i <= M; i++) Z[i] = max(ll(1), min(N, W[i] + D[i] - 1));
		cin >> V[1] >> AV >> BV;
		for (int i = 2; i <= M; i++) V[i] = ((AV * V[i-1] + BV) % ll(1e9)) + 1;

		ll res = 0;
		for (int i = 1; i <= M; i++) {
			if (Z[i] == W[i]) {
				dat[S + W[i]].val[0][0] += V[i];
				dat[S + W[i]].val[0][0] %= MOD;
			} else if (Z[i] == W[i] + 1) {
				dat[S + W[i]].val[0][1] += V[i];
				dat[S + W[i]].val[0][1] %= MOD;
			} else if (Z[i] == W[i] - 1) {
				dat[S + W[i]].val[1][0] += V[i];
				dat[S + W[i]].val[1][0] %= MOD;
			} else {
				assert(false);
			}
			update_all(S + W[i]);
			res += dat[1].val[0][0];
			res %= MOD;
		}
		cout << "Case #" << T << ": " << res << '\n';
	}
	return 0;
}
