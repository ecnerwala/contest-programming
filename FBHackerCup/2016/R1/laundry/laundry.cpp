#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXL = 2e6;
const int MAXN = 2e5;
int L, N, M;
ll D;
ll W[MAXN];

int main() {
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> L >> N >> M >> D;
		priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
		for(int i = 0; i < N; i++) {
			cin >> W[i];
			pq.emplace(W[i], W[i]);
		}

		ll res = 0;
		for(int i = 0; i < L; i++) {
			ll t = pq.top().first, w = pq.top().second;
			pq.pop();
			res = max(res, t + ((L - i - 1) / M + 1) * D);
			pq.emplace(t + w, w);
		}

		cout << "Case #" << T << ": " << res << '\n';
	}
	return 0;
}
