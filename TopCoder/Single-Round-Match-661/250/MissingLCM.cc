#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class MissingLCM {

public:

	static const int MAXN = 2e6;
	bool prime[MAXN];
	bool ppow[MAXN];
	int getMin(int N) {
		for(int i = 1; i <= N; i++) {
			prime[i] = true;
			ppow[i] = false;
		}
		ppow[1] = true;
		for(int i = 2; i <= N; i++) {
			if(prime[i]) {
				for(long long v = i; v <= N; v *= i) {
					ppow[v] = true;
				}
				for(int j = i; j <= N; j += i) {
					prime[j] = false;
				}
			}
		}
		int M = N + 1;
		for(int i = 1; i <= N; i++) {
			if(ppow[i]) {
				M = max(M, (N / i + 1) * i);
			}
		}
		return M;
	}
};

// vim:ft=cpp
