#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> first_adj(N);
	std::iota(first_adj.begin(), first_adj.end(), 0);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		if (a > b) swap(a, b);
		first_adj[b] = min(first_adj[b], a);
	}

	vector<int> need_adj(N+1);
	need_adj[N] = N-1;
	for (int i = N-1; i >= 0; i--) {
		need_adj[i] = min({need_adj[i+1], first_adj[i], i-1});
	}

	vector<int> cross_cnt(N+1);
	for (int i = 0; i < N; i++) {
		assert(first_adj[i] <= i);
		cross_cnt[first_adj[i]+1] ++;
		cross_cnt[i+1]--;
	}
	for (int i = 0; i < N; i++) {
		cross_cnt[i+1] += cross_cnt[i];
	}

	cross_cnt[0]++; // 0 to -1

	vector<int> dp(N+1, 0);
	dp[N] = 0;

	priority_queue<pair<int, int>> pq;
	int cur_val = 0;
	for (int i = N-1; i >= 1; i--) {
		while (!pq.empty() && pq.top().first >= i) {
			setmax(cur_val, pq.top().second);
			pq.pop();
		}
		dp[i] = cur_val + cross_cnt[i];
		pq.push({need_adj[i], dp[i]});
	}

	cout << N - 1 - dp[1] << '\n';

	return 0;
}
