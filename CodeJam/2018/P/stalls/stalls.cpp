#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		long long N, K;
		cin >> N >> K;

		priority_queue<pair<ll, ll>> q;
		q.emplace(N, 1);

		while (true) {
			assert(K);

			ll v = q.top().first;
			ll c = q.top().second;

			assert(v);
			v--;
			if (c < K) {
				K -= c;
				q.pop();
				q.emplace(v / 2, c);
				q.emplace(v / 2 + v % 2, c);
			} else {
				// last person is here
				break;
			}
		}

		ll v = q.top().first-1;
		cout << "Case #" << case_num << ": " << (v / 2 + v % 2) << ' ' << v / 2 << '\n';
	}

	return 0;
}
