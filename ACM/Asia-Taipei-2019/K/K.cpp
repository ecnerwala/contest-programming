#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		priority_queue<int, vector<int>, greater<int>> pq;
		for (int i = 0; i < N; i++) {
			int a; cin >> a; pq.push(a);
		}
		long long ans = 0;
		while (pq.size() >= 2) {
			int a = pq.top(); pq.pop();
			int b = pq.top(); pq.pop();
			ans += a+b;
			pq.push(a+b);
		}
		cout << ans << '\n';
	}

	return 0;
}
