#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	ios_base::sync_with_stdio(0);

	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {

		int N;
		ll P;

		cin >> N >> P;

		ll sum = 0;

		queue<ll> q;
		q.push(sum);

		ll res = 0;

		for(int i = 0; i < N; i++) {
			ll v; cin >> v; sum += v;
			while(!q.empty() && sum - q.front() > P) { q.pop(); }
			res += q.size();
			q.push(sum);
		}

		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
