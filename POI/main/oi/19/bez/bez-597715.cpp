#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
#define A first
#define B second

struct trans {
	ll A, B;

	trans (ll a, ll b) {
		A = a, B = b;
	}

	trans (ll b) {
		A = -1;
		B = b;
	}

	trans () {
		A = 1;
		B = 0;
	}

	ll operator() (ll v) {
		return A * v + B;
	}
};

trans operator + (trans a, trans b) {
	return trans(a.A + b.A, a.B + b.B);
}

// compose
trans operator * (trans a, trans b) {
	return trans(a.A * b.A, a.A * b.B + a.B);
}

// inverse
trans operator ~ (trans a) {
	assert(a.A * a.A == 1);
	// ax + b = y
	// x = 1/a y - 1/a b
	return trans(a.A, -a.B * a.A);
}

const trans I;

const int MAXN = 1e6;
const int MAXM = 6e6;

int N, M;
int P[MAXN];
int L[MAXM], R[MAXM];
int V[MAXM];

vector<pii> adj[MAXN];

pair<int, trans> par[MAXN];

int mi[MAXN], ma[MAXN];
trans sum[MAXN];
pair<int, trans> getpar(int a) {
	if(par[a].A == a) {
		return par[a];
	}
	pair<int, trans> p = getpar(par[a].A);
	p.B = par[a].B * p.B;
	return par[a] = p;
}

bool merge(int l, int r, int v) {
	pair<int, trans> pl = getpar(l), pr = getpar(r);
	int a = pl.A, b = pr.A;

	//t(val(a)) == val(b);
	trans t = (~pr.B) * (trans(v) * pl.B);
	if(a == b) {
		if(t.A == 1) return t.B == 0;
		else if(t.A == -1) {
			if(t.B % 2 != 0) return false;
			int k = t.B / 2;
			if(mi[a] <= k && k <= ma[a]) {
				mi[a] = ma[a] = k;
				return true;
			} else {
				return false;
			}
		} else assert(false);
	} else {
		par[b] = make_pair(a, t);
		sum[b] = sum[b] * t;
		sum[a] = sum[a] + sum[b];
		int minv = (~t)(mi[b]);
		int maxv = (~t)(ma[b]);
		if(minv > maxv) swap(minv, maxv);
		mi[a] = max(minv, mi[a]);
		ma[a] = min(maxv, ma[a]);
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> P[i];
	}

	for(int i = 0; i < M; i++) {
		cin >> L[i] >> R[i] >> V[i];
		adj[L[i]].push_back(pii(R[i], V[i]));
		adj[R[i]].push_back(pii(L[i], V[i]));
	}

	for(int i = 0; i < N; i++) {
		par[i] = make_pair(i, I);
		mi[i] = 0;
		ma[i] = P[i];
		sum[i] = trans(P[i]);
	}

	for(int i = 0; i < M; i++) {
		if(!merge(L[i], R[i], V[i])) {
			cout << "NIE\n";
			return 0;
		}
	}

	int r1 = 0, r2 = 0;
	for(int i = 0; i < N; i++) {
		if(par[i].A == i) {
			int minv = sum[i](mi[i]);
			int maxv = sum[i](ma[i]);
			if(maxv < minv) swap(minv, maxv);
			r1 += minv, r2 += maxv;
		}
	}
	cout <<r1 << ' ' << r2 << '\n';
	return 0;
}
