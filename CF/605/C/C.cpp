#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
int N;
pii P;

pii V[MAXN];

bool good(ld c) {
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> P.A >> P.B;
	for(int i = 0; i < N; i++) {
		cin >> V[i].A >> V[i].B;
	}
	ld mi = 0;
	ld ma = 1e9;
	for(int i = 0; i < 200; i++) {
		ld md = (mi + ma) / 2;
		if(good(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	for(int i = 0; i < N; i++) {
		ma = min(ma, max(ld(P.A) / V[i].A, ld(P.B) / V[i].B));
	}
	cout << fixed << setprecision(20) << ma << '\n';
	return 0;
}

// if you have A1 x + A2 y = P, B1 x + B2 y = Q
// then x + y = (P cross V1 + V2 cross P) / (V2 cross V1) = c
// abs(P) / c = (- V1 cos t1 * V2 sin t2 + V2 cos t2 * V1 sin t1) / (V1 sin t1 - V2 sin t2)
// = (V1 cos t1 / V1 sin t1 - V2 cos t2 / V2 sin t2) / (1 / V1 sin t1 - 1 / V2 sin t2)
// = (tan t1 - tan t2) / (1 / (V1 sin t1) - 1 / (V2 sin t2)) <= abs(PQ) / c
// iff (abs(PQ) / (V1 sin t1) / c - tan t1) - (abs(PQ) / (V2 sin t2) / c - tan t2) >= 0
// V1 cross V
// cross over dot
