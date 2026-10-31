#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	int64_t cur_slope = 0;
	priority_queue<int64_t> pq;
	int64_t ans = 0;

	pq.push(-cur_slope);
	pq.push(-cur_slope);
	ans += pq.top() + cur_slope; pq.pop();

	while (N--) {
		int a; cin >> a;
		cur_slope += a;

		pq.push(-cur_slope);
		pq.push(-cur_slope);
		ans += pq.top() + cur_slope; pq.pop();
	}
	cout << ans << '\n';

	return 0;
}
