#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::priority_queue<int> pq;
	int64_t ans = 0;
	while (N--) {
		int p; cin >> p;
		ans += p;
		pq.push(-p);
		pq.push(-p);
		ans += pq.top(); pq.pop();
	}
	cout << ans << '\n';

	return 0;
}
