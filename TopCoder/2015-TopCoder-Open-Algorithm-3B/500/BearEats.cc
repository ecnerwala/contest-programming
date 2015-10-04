#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
#define A first
#define B second

class BearEats {

	static const ll MOD = 1e9 + 7;

	static const int MAXN = 3e5;

	int N;
	pii C[MAXN]; // stored as (B[i], A[i] + B[i])

public:

	long long getDifference(int n, int R, int M, int D, int A_MAX, int B_MAX) {
		N = n;
		ll seed = R;
		for(int i = 0; i < N; i++) {
			C[i].B = (seed = (seed * M + D) % MOD) % A_MAX;
			C[i].A = (seed = (seed * M + D) % MOD) % B_MAX;
			C[i].B += C[i].A;
		}
		sort(C, C + N);
		reverse(C, C + N);

		priority_queue<ll, vector<ll>, greater<ll>> pq;

		ll tot = 0;
		for(int i = 0; i < N; i++) tot -= C[i].A;

		for(int i = 0; i < N; i++) {
			tot += C[i].B;
			pq.push(C[i].B);
			if(i % 2 == 1) {
				tot -= pq.top();
				pq.pop();
			}
		}

		return tot;
	}
};

// vim:ft=cpp
