#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll& setmin(ll &a, const ll &b) { return a -= (b < a) * (a - b); }

const ll INF = 1e18;

const int MAXN = 1e5;

int N;
int V[MAXN];
ll H[MAXN]; // coordinate compression
ll C[MAXN];

ll res[MAXN];

pair<ll, int> ord[MAXN];

struct seg_tree {
	static const int D = 16;
	static const int S = 1 << D;

	ll s[2 * S];
	void insert(int a, ll v) {
		s[a+S] = v;
		for(int i = (a + S)/2; i; i /= 2) {
			s[i] = min(s[2*i], s[2*i+1]);
		}
	}

	ll query(int a, int b) {
		ll r = INF;
		for(int i = a+S, j = b+S; i <= j; i /= 2, j /= 2) {
			if(i & 1) {
				setmin(r, s[i]);
				i++;
			}
			if(!(j & 1)) {
				setmin(r, s[j]);
				j--;
			}
		}
		return r;
	}

	seg_tree() {
		for(int i = 1; i < 2 * S; i++) s[i] = INF;
	}
} segs[3][3];

int main() {
	scanf("%d", &N);
	for(int i = 1; i <= N; i++) { int v; scanf("%d", &v); H[i] = v; }

	if(N == 2) {
		printf("%lld\n", abs(H[2] - H[1]));
		printf("%lld\n", abs(H[2] - H[1]));
		return 0;
	}

	{ // special cases
		{ // 1 and 2
			ll val = abs(H[3] - H[1]) - abs(H[3] - H[2]);
			setmin(res[1], val);
			setmin(res[2], val);
		}
		{ // N-1 and N
			ll val = abs(H[N-2] - H[N]) - abs(H[N-2] - H[N-1]);
			setmin(res[N-1], val);
			setmin(res[N], val);
		}
		{ // 1 and N
			ll val = abs(H[N-1] - H[1]) + abs(H[2] - H[N]) - abs(H[N-1] - H[N]) - abs(H[2] - H[1]);
			setmin(res[1], val);
			setmin(res[N], val);
		}
		{ // 1 and 3..N-1
			for(int i = 3; i <= N-1; i++) {
				ll val = abs(H[i-1] - H[1]) + abs(H[i+1] - H[1]) + abs(H[2] - H[i])
					- (abs(H[i-1] - H[i]) + abs(H[i+1] - H[i]) + abs(H[2] - H[1]));
				setmin(res[1], val);
				setmin(res[i], val);
			}
		}
		{ // N and 2..N-2
			for(int i = 2; i <= N-2; i++) {
				ll val = abs(H[i-1] - H[N]) + abs(H[i+1] - H[N]) + abs(H[N-1] - H[i])
					- (abs(H[i-1] - H[i]) + abs(H[i+1] - H[i]) + abs(H[N-1] - H[N]));
				setmin(res[N], val);
				setmin(res[i], val);
			}
		}
		{ // (2,3) .. (N-2, N-1)
			for(int i = 2; i <= N-2; i++) {
				ll val = abs(H[i-1] - H[i+1]) + abs(H[i+2] - H[i]) - abs(H[i-1] - H[i]) - abs(H[i+2] - H[i+1]);
				setmin(res[i], val);
				setmin(res[i+1], val);
			}
		}
	}

	for(int i = 1; i <= N; i++) { ord[i].first = H[i], ord[i].second = i; }
	sort(ord+1, ord+N+1);
	for(int i = 1; i <= N; i++) { V[ord[i].second] = i; }

	for(int i = 2; i < N; i++) {
		C[i] = abs(H[i-1] - H[i]) + abs(H[i+1] - H[i]) - abs(H[i-1] - H[i+1]);
	}

	// insert things
	for(int i = 2; i < N; i++) {
		for(int v = 0; v <= 2; v++) {
			ll val = (v-1) * 2 * H[i] + 2 * min(H[i-1], H[i+1]) - C[i];
			segs[2][v].insert(V[i], val);
		}
	}

	for(int z = 1; z <= N; z++) {
		int cur = ord[z].second;
		// remove
		if(cur - 2 >= 1) {
			assert(V[cur] != V[cur-2]);
			for(int v = 0; v <= 2; v++) {
				segs[(V[cur] < V[cur-2]) ? 2 : 1][v].insert(V[cur-1], INF);
			}
		}
		if(cur + 2 <= N) {
			assert(V[cur] != V[cur+2]);
			for(int v = 0; v <= 2; v++) {
				segs[(V[cur] < V[cur+2]) ? 2 : 1][v].insert(V[cur+1], INF);
			}
		}
		// query
		if(1 < cur && cur < N) {
			int l = V[cur-1], r = V[cur+1];
			assert(l != r);
			if(l > r) swap(l, r);
			for(int v = 0; v <= 2; v++) {
				ll cnst = 2 * (v-1) * H[cur] + C[cur];
				setmin(res[cur], segs[v][0].query(1, l) + 2 * min(H[cur-1], H[cur+1]) - cnst);
				setmin(res[cur], segs[v][1].query(l, r) - cnst);
				setmin(res[cur], segs[v][2].query(r, N) - 2 * max(H[cur-1], H[cur+1]) - cnst);
			}
		}
		// insert
		if(cur - 2 >= 1) {
			if(V[cur] < V[cur-2]) {
				for(int v = 0; v <= 2; v++) {
					segs[1][v].insert(V[cur-1], (v-1) * 2 * H[cur-1] - C[cur-1]);
				}
			} else {
				for(int v = 0; v <= 2; v++) {
					segs[0][v].insert(V[cur-1], (v-1) * 2 * H[cur-1] - 2 * max(H[cur], H[cur-2]) - C[cur-1]);
				}
			}
		}
		if(cur + 2 <= N) {
			if(V[cur] < V[cur+2]) {
				for(int v = 0; v <= 2; v++) {
					segs[1][v].insert(V[cur+1], (v-1) * 2 * H[cur+1] - C[cur+1]);
				}
			} else {
				for(int v = 0; v <= 2; v++) {
					segs[0][v].insert(V[cur+1], (v-1) * 2 * H[cur+1] - 2 * max(H[cur], H[cur+2]) - C[cur+1]);
				}
			}
		}
	}

	ll sum = 0;
	for(int i = 1; i < N; i++) {
		sum += abs(H[i+1] - H[i]);
	}
	for(int i = 1; i <= N; i++) {
		res[i] += sum;
		printf("%lld\n", res[i]);
	}
	return 0;
}
