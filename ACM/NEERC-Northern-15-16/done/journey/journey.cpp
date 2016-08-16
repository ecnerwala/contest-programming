#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e5;

int N;
ll T;

int P[MAXN];
ll D[MAXN];

ll dp[MAXN];

ll get_time(int r) {
	dp[0] = 0;

	deque<ll> dq; // nondecreasing
	dq.push_back(0);

	for(int i = 1; i <= N; i++) {
		dp[i] = dq.front() + D[i];

		while(!dq.empty() && dq.back() > dp[i]) {
			dq.pop_back();
		}
		dq.push_back(dp[i]);

		if(i >= r) {
			if(dq.front() == dp[i - r]) dq.pop_front();
		}
		
		assert(!dq.empty());
	}

	return dp[N];
}

int main() {
	if(fopen("journey.in", "r")) {
		freopen("journey.in", "r", stdin);
		freopen("journey.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	cin >> N >> T;

	N--;

	T -= N;

	for(int i = 1; i <= N; i++) {
		cin >> P[i];
	}

	for(int i = N; i > 1; i--) {
		if(P[i] < P[i - 1]) {
			P[i - 1] = P[i];
		}
	}

	for(int i = 1; i < N; i++) {
		cin >> D[i];
	}

	int mi = 0, ma = N + 1;

	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(get_time(md) <= T) {
			ma = md;
		} else {
			mi = md;
		}
	}

	cout << P[ma] << '\n';
}
