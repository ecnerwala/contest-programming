#include "shoes.h"
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
ll count_swaps(vector<int> S) {
	int N = int(S.size());
	unordered_map<int, queue<int>> unpaired;
	vector<int> bit(N+1);
	ll ans = 0;
	for (int i = 0; i < N; i++) {
		int j;
		if (unpaired[-S[i]].empty()) {
			j = i+1;
			unpaired[S[i]].push(j);
		} else {
			j = unpaired[-S[i]].front(); unpaired[-S[i]].pop();
			ans += i;
			for (int a = j; a; a -= a & (-a)) {
				ans -= bit[a];
			}
			if (S[i] < 0) ans++;
		}
		for (int a = j; a <= N; a += a & (-a)) {
			bit[a]++;
		}
	}
	return ans;
}
