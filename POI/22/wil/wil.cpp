#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 2.1e6;
int N;
ll W[MAXN+1];

int D;
ll P;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> P >> D;
	for (int i = 1; i <= N; i++) {
		int w; cin >> w;
		assert(w > 0);
		W[i] = W[i-1] + w;
	}

	assert(D <= N);
	int res = D;
	deque<pair<int, ll>> q;
	for (int l = 0, r = 0, dr = 0; l + D <= N; l++) {
		while (dr + D <= N && W[dr] - W[l] <= P) {
			ll v = W[dr + D] - W[dr];
			while (!q.empty() && q.back().second <= v) {
				q.pop_back();
			}
			q.emplace_back(dr, v);
			dr ++;
		}
		assert(dr > l);
		assert(!q.empty());
		assert(q.back().first == dr - 1);
		while (!q.empty() && q.front().first < l) {
			q.pop_front();
		}
		assert(!q.empty());
		assert(q.back().first == dr - 1);
	
		ll v = q.front().second + P;
		while (r <= N && W[r] - W[l] <= v) {
			res = max(res, r - l);
			r ++;
		}
	}
	cout << res << '\n';
	return 0;
}
