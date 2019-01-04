#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
int K;
pair<pair<int, int>, int> P[MAXN];
int ans[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> K;
		for (int i = 0; i < N; i++) {
			cin >> P[i].first.first >> P[i].first.second;
			P[i].second = i;
		}
		sort(P, P + N);
		int cur = -1;
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
		for (int i = 0; i < K; i++) {
			pq.emplace(-1, i);
		}
		int len = 0;
		for (int i = 0; i < N; i++) {
			int l = P[i].first.first;
			assert(l >= cur);
			len += max(min(l, pq.top().first) - cur, 0);
			cur = l;
			auto it = pq.top(); pq.pop();
			it.first = max(it.first, P[i].first.second);
			ans[P[i].second] = it.second;
			pq.push(it);
		}
		len += max(pq.top().first - cur, 0);
		cout << len << '\n';
		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i==(N-1)];
		}
	}

	return 0;
}
