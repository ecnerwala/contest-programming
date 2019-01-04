#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int gcd(int a, int b) { return a ? gcd(b % a, a) : b; }

struct frac {
	int a, b;

	frac(int x, int y) {
		int g = gcd(x, y);
		a = x / g, b = y / g;
	}
	frac() {
		a = b = 0;
	}
};

inline bool operator < (frac a, frac b) {
	return ll(a.a) * b.b < ll(b.a) * a.b;
}
inline bool operator ==(frac a, frac b) {
	return a.a == b.a && a.b == b.b;
}

const int MAXK = 150;
const int MAXN = 6e5;
const int MAXV = MAXN * 2;

int K;

int N;
int A[MAXN], B[MAXN];
frac L[MAXN], R[MAXN];

int V;
frac vals[MAXV];

const int D = 21;
const int S = 1 << D;
const int MAXS = S * 2;

int le[MAXS], ri[MAXS], sz[MAXS];

int dp[MAXV][2];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> K >> N;
	V = 0;
	for(int i = 0; i < N; i++) {
		int a, b, c, d; cin >> a >> b >> c >> d;
		L[i] = frac(a, b);
		R[i] = frac(c, d);
		vals[V++] = L[i];
		vals[V++] = R[i];
	}
	sort(vals, vals + V);
	V = unique(vals, vals + V) - vals;
	//cerr << V << '\n';
	for(int i = 0; i < S; i++) {
		le[S + i] = i;
		ri[S + i] = i;
		sz[S + i] = 0;
	}
	for(int i = S - 1; i > 0; i--) {
		le[i] = max(le[2 * i], le[2 * i + 1]);
		ri[i] = min(ri[2 * i], ri[2 * i + 1]);
		sz[i] = 0;
	}
	for(int i = 0; i < N; i++) {
		assert(binary_search(vals, vals + V, L[i]));
		assert(binary_search(vals, vals + V, R[i]));
		A[i] = lower_bound(vals, vals + V, L[i]) - vals; A[i] *= 2;
		B[i] = lower_bound(vals, vals + V, R[i]) - vals; B[i] *= 2;
		if(A[i] > B[i]) swap(A[i], B[i]);
		//cerr << A[i] << ' ' << B[i] << '\n';
		for(int a = S + A[i], b = S + B[i]; a <= b; a /= 2, b /= 2) {
			if(a % 2 == 1) {
				le[a] = min(le[a], A[i]);
				ri[a] = max(ri[a], B[i] + 1);
				sz[a] ++;
				a++;
			}
			if(b % 2 == 0) {
				le[b] = min(le[b], A[i]);
				ri[b] = max(ri[b], B[i] + 1);
				sz[b] ++;
				b--;
			}
		}
	}
	V *= 2;
	int res = 0;
	for(int k = 0; k < K; k++) {
		int t = k & 1;
		for(int i = 0; i < V; i++) {
			if(i) dp[i][t] = max(dp[i][t], dp[i - 1][t]);
			dp[i][!t] = 0;
		}
		for(int i = 0; i < V; i++) {
			int l = i, r = i, s = 0;
			for(int v = i + S; v > 0; v /= 2) {
				l = min(l, le[v]);
				r = max(r, ri[v]);
				s += sz[v];
			}
			//assert(s);
			assert(l <= i && i <= r);
			//cerr << i << ' ' << l << ' ' << r << ' ' << s << '\n';

			dp[r][!t] = max(dp[r][!t], dp[l][t] + s);
			if(dp[r][!t] > res) res = dp[r][!t];
		}
	}
	cout << res << '\n';
	return 0;
}
