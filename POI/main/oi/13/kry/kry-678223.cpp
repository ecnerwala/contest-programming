#include<bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
const ull ONE = 1ull;

int N;
ull M[60];

ull res;

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%llu", &M[i]);
		M[i] ++;
	}

	for(int e = 32; e >= -1; e--) {
		if(e == -1) {
			res ++;
			break;
		}
		ull p = ONE << e;
		ull dp0 = 0, dp1 = 0;
		ull prod = 1;
		ull cnt = 0; // current number of 1's
		for(int i = 0; i < N; i++) {
			ull &m = M[i];
			if(m > p) {
				m -= p;
				ull ndp0 = dp0 * p + dp1 * m, ndp1 = dp1 * p + dp0 * m;
				dp0 = ndp0, dp1 = ndp1;
				if(cnt % 2 == 0) {
					dp0 += prod;
				} else {
					dp1 += prod;
				}
				cnt++;
				prod *= m;
			} else {
				dp0 *= m;
				dp1 *= m;
				prod *= m;
			}
		}
		cerr << e << ' ' << dp0 << '\n';
		res += dp0;
		if(cnt % 2 == 1) {
			break;
		}
	}
	res--;
	printf("%llu\n", res);
}
