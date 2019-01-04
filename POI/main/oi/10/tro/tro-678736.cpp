#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int dp[3];
int ndp[3];

int go(ll N, ll I) {
	dp[0] = 1;
	dp[1] = 0;
	dp[2] = 0;
	while(N) {
		int n = N % 3;
		for(int v = 0; v < 3 && v <= I; v++) {
			int i = (I - v) % 3;
			for(int a = 0; a < 3; a++) {
				int b = (i + a) % 3;
				assert((b + a + a) % 3 == i);
				assert((2 * a + b + v) % 3 == I % 3);
				assert(((2 * a + b + v) - I % 3) / 3 < 3);
				if(a + b > n) {
					continue;
				}
				int c = n - a - b;
				int val = 1;
				if(bool(a) + bool(b) + bool(c) == 2) {
					val = 2;
				}
				ndp[((2 * a + b + v) - I % 3) / 3] += val * dp[v];
			}
		}
		for(int v = 0; v < 3; v++) {
			dp[v] = ndp[v] % 3;
			ndp[v] = 0;
		}
		N /= 3;
		I /= 3;
	}
	assert(I < 3);
	return dp[I];
}

int main() {
	int K;
	cin >> K;
	while(K--) {
		int N, I;
		cin >> N >> I;
		cout << go(N, I) % 3 << '\n';
	}
	return 0;
}
