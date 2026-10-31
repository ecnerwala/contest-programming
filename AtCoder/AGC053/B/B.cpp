#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> V(2*N);
	for (auto& v : V) cin >> v;

	priority_queue<int, vector<int>, greater<int>> pq;
	for (int i = 0; i < N; i++) {
		pq.push(V[N-1-i]);
		pq.push(V[N+i]);
		pq.pop();
	}
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += pq.top(); pq.pop();
	}
	cout << ans << '\n';

	return 0;
}
