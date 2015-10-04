#include<bits/stdc++.h>
using namespace std;

const int INF = 2e9 + 500;

typedef long long ll;

const int MAXN = 2e5;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXV = MAXN * 2;

int V;
int vals[MAXV];

int N;
int A, B;
pair<pii, int> L[MAXN];

const int S = 1 << 19;
int seg[2 * S];
// point query
int query(int a) {
	int res = INF;
	for(int i = a + S; i; i /= 2) {
		res = min(res, seg[i]);
	}
	return res;
}

// [a,b]
void update(int a, int b, int v) {
	for(a += S, b += S; a <= b; a /= 2, b /= 2) {
		if(a % 2 == 1) {
			seg[a] = min(seg[a], v);
			a++;
		}
		if(b % 2 == 0) {
			seg[b] = min(seg[b], v);
			b--;
		}
	}
}

void reset() {
	V = 0;
	for(int i = 0; i < 2 * S; i++) {
		seg[i] = INF;
	}
}

int go() {
	reset();
	cin >> N >> A >> B;
	vals[V++] = A;
	vals[V++] = B;
	for(int i = 0; i < N; i++) {
		int x, d;
		cin >> x >> d >> L[i].B;
		pii &r = L[i].A;
		r = pii(x - d, x + d);
		if(r.A < A) r.A = A;
		if(r.B > B) r.B = B;
		if(r.A >= r.B) {
			i--, N--;
			continue;
		}
		assert(r.A < r.B);
		assert(A <= r.A && r.A < r.B && r.B <= B);
		vals[V++] = r.A;
		vals[V++] = r.B;
	}
	sort(vals, vals + V);
	V = unique(vals, vals + V) - vals;

	assert(vals[0] == A);
	assert(vals[V - 1] == B);

	for(int i = 0; i < N; i++) {
		L[i].A.A = lower_bound(vals, vals + V, L[i].A.A) - vals;
		L[i].A.B = lower_bound(vals, vals + V, L[i].A.B) - vals;
	}
	sort(L, L + N);

	update(0, 0, 0);

	for(int i = 0; i < N; i++) {
		int cost = query(L[i].A.A);
		if(cost == INF) continue;
		cost += L[i].B;
		update(0, L[i].A.B, cost);
	}

	int res = query(V - 1);
	if(res == INF) return -1;
	return res;
}

int main() {
  ios_base::sync_with_stdio(0);
  int K;
  cin >> K;
  while(K--) {
	  cout << go() << '\n';
  }
  return 0;
}
