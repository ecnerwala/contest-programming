#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
const int MAXN = int(1e5) + 200;
#define X first
#define Y second

ll cross(pll a, pll b) {
	return a.X * b.Y - a.Y * b.X;
}

int sign(ll a) { return (a > 0) - (a < 0); }

int N;
int CX, CY;

bool go() {
	scanf("%d %d %d", &N, &CX, &CY);
	pll first;
	pll prv;
	int sgn = 0;
	bool is_good = true;
	for (int i = 0; i <= N; i++) {
		pll cur;
		if (i < N) {
			int x, y;
			scanf("%d %d", &x, &y);
			cur = pll(x - CX, y - CY);
		} else {
			cur = first;
		}
		if (i == 0) {
			first = cur;
		} else {
			int s = sign(cross(cur, prv));
			if (s == 0) is_good = false;
			if (sgn * s == -1) is_good = false;
			sgn = s;
		}
		prv = cur;
	}
	return is_good;
}

int main() {
	int M; scanf("%d", &M);
	for (int i = 0; i < M; i++) {
		bool v = go();
		if(v) printf("TAK\n");
		else printf("NIE\n");
	}

	return 0;
}
