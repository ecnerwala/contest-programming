#include<bits/stdc++.h>
using namespace std;

typedef double ld;
typedef complex<ld> pt;
#define X real()
#define Y imag()

const int MAXN = 4e3;

int N;
ld D;
pt P[MAXN];
int dp[MAXN];

int main() {
	if(fopen("kingdom.in", "r")) {
		freopen("kingdom.in", "r", stdin);
		freopen("kingdom.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	cin >> N >> D;
	for(int i = 0; i < N; i++) {
		int x, y;
		cin >> x >> y;
		P[i] = pt(x, y);
	}
}
