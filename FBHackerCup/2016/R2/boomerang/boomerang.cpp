#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N;
char A[MAXN], B[MAXN];
int lcost[MAXN];
int rcost[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> N >> A >> B;
		assert(N < MAXN);
		lcost[0] = 0;
		lcost[1] = 1;
		for(int i = 1; i < N; i++) {
			lcost[i + 1] = lcost[i] + (B[i] != B[i - 1]);
		}
		// l is the first same point
		// lcost[1] = 0 differs
		rcost[N] = 0;
		rcost[N - 1] = 1;
		for(int i = N - 1; i > 0; i--) {
			rcost[i - 1] = rcost[i] + (B[i] != B[i - 1]);
		}
		// r is the first different point
		// rcost[N - 1] = rcost given r is differetn

		int res = N;
		// [l,r) is all the same
		for(int l = 0, r = 0; r <= N; r ++) {
			res = min(res, max(lcost[l], rcost[r]));
			if(A[r] != B[r]) {
				l = r + 1;
			}
		}
		cout << "Case #" << T << ": " << res << '\n';
	}
	return 0;
}
