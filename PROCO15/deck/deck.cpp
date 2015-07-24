#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2000;
int N;
bool take[MAXN];

bool dp[MAXN];

bool isgood() {
	memset(dp, 0, sizeof(dp));
	dp[0] = true;
	for(int i = 0; i < N; i++) {
		if(!dp[i]) continue;
		if(i + 1 < N && take[i] + take[i + 1] <= 1) {
			dp[i + 2] = true;
		}
		if(i + 2 < N && take[i] + take[i + 1] + take[i + 2] <= 1) {
			dp[i + 3] = true;
		}
	}
	return dp[N];
}

int V[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		int v;
		cin >> v;
		v--;
		V[v] = i;
	}

	for(int v = N - 1; v >= 0; v--) {
		take[V[v]] = true;
		if(!isgood()) {
			take[V[v]] = false;
		} else {
			cout << v+1 << '\n';
		}
	}

	return 0;
}
