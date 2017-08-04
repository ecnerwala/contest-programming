#include<bits/stdc++.h>
using namespace std;

const long long MOD = (long long)(1e9 + 7);

struct num {
	long long v;
	num(long long _v) : v(_v) {}
	operator long long () {
		return (v + MOD) % MOD;
	}
	num operator - (const num &o) const {
		return num{(v - o.v) % MOD};
	}
	num operator + (const num &o) const {
		return num{(v + o.v) % MOD};
	}
	num operator * (const num &o) const {
		return num{(v * o.v) % MOD};
	}
	num operator % (const num &o) const {
		return num{v};
	}
	num& operator += (const num &o) {
		v += o.v;
		v %= MOD;
		return *this;
	}
	num& operator -= (const num &o) {
		v -= o.v;
		v %= MOD;
		return *this;
	}
	num& operator *= (const num &o) {
		v *= o.v;
		v %= MOD;
		return *this;
	}
	num& operator %= (const num &o) {
		return *this;
	}
};

typedef num ll;

typedef pair<ll, ll> pii;

const int MAXM = 4e5;
const int MAXK = 200;
ll N, M, K;
pii H[MAXM];
pii prv[MAXM], nxt[MAXM];

ll black, white, grey, blue;

ll ways;

ll pref[MAXM], prefi[MAXM];

ll c2(ll a) {
	a %= MOD;
	const int INV = (MOD + 1) / 2;
	return a * (a - 1) % MOD * INV % MOD;
}

ll c3(ll a) {
	a %= MOD;
	const int INV = (MOD + 1) / 6;
	return a * (a - 1) % MOD * (a - 2) % MOD * INV % MOD;
}

void go() {
	black = 0, white = 0, grey = 0, blue = 0;
	ways = 0;
	H[0].first = H[M + 1].first = N;
	for (ll i = 0; i <= M + 1; i++) {
		H[i].second = i;
	}
	for (ll i = 1; i <= M; i++) {
		assert(1 <= H[i].first && H[i].first < N);
	}

	{ // compute prv
		stack<pii> q;
		q.push(H[0]);
		for (ll i = 1; i <= M; i++) {
			assert(!q.empty());
			while (q.top() < H[i]) q.pop();
			prv[i] = q.top();
			assert(prv[i] > H[i]);
			q.push(H[i]);
		}
	}
	{ // compute nxt
		stack<pii> q;
		q.push(H[M+1]);
		for (ll i = M; i >= 1; i--) {
			assert(!q.empty());
			while (q.top() < H[i]) q.pop();
			nxt[i] = q.top();
			assert(nxt[i] > H[i]);
			q.push(H[i]);
		}
	}

	{ // compute pref and prefi
		pref[0] = 0, prefi[0] = 0;
		for (ll i = 1; i <= M; i++) {
			pref[i] = (pref[i-1] + H[i].first) % MOD;
			prefi[i] = (prefi[i-1] + H[i].first * i) % MOD;
		}
		pref[M+1] = pref[M];
		prefi[M+1] = prefi[M];
	}

	for (ll m = 1; m <= M; m++) {
		ll l = prv[m].second, r = nxt[m].second;
		assert(l < m && m < r);
		ll h = H[m].first;
		ll w = c2(N - h + 1);
		w *= (m - l) * (r - m) % MOD;
		w %= MOD;
		ways += w;
		ways %= MOD;

		ll cwhite = c3(N - h + 2);
		cwhite *= ((c2(m - l) * (r - m) % MOD) + (c2(r - m) * (m - l) % MOD) + ((m-l) * (r-m) % MOD)) % MOD;
		cwhite %= MOD;
		white += cwhite;
		white %= MOD;

		ll cblue = c3(N - h + 1) + c2(N - h + 1) * h;
		cblue %= MOD;
		cblue *= ((c2(m - l) * (r - m) % MOD) + (c2(r - m) * (m - l) % MOD) + ((m-l) * (r-m) % MOD)) % MOD;
		cblue %= MOD;

		// subtract out height of mid
		cblue -= w * h % MOD;
		cblue %= MOD;

		// subtract height of left sides
		cblue -= ((prefi[m-1] - prefi[l]) - (pref[m-1] - pref[l]) * l % MOD) % MOD * (r-m) % MOD * c2(N-h+1) % MOD;
		cblue %= MOD;
		// subtract height of right sides
		cblue -= ((pref[r-1] - pref[m]) * r % MOD - (prefi[r-1] - prefi[m])) % MOD * (m-l) % MOD * c2(N-h+1) % MOD;

		if (cblue < 0) cblue += MOD;

		blue += cblue;
		blue %= MOD;
	}

	ways %= MOD;
	grey = (ways * pref[M]) % MOD;
	blue %= MOD;
	white %= MOD;

	black = (N * M) % MOD * ways % MOD - grey - blue - white;
	black %= MOD;
	if (black < 0) black += MOD;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N >> M >> K;
		assert(M < MAXM);
		assert(K < MAXK);
		int i = 1;
		for (int k = 0; k < K; k++) {
			ll L, S, A, B;
			cin >> L >> S >> A >> B;
			for (int l = 0; l < L; l++) {
				H[i++].first = S;
				S = ((A * S + B) % (N-1)) + 1;
			}
		}
		assert(i == M + 1);
		go();

		black = (MOD - black) % MOD;
		white = (MOD - white) % MOD;
		grey = (MOD - grey) % MOD;
		blue = (MOD - blue) % MOD;
		cout << "Case #" << T << ": " << black << ' ' << white << ' ' << grey << ' ' << blue << '\n';
	}

	return 0;
}
