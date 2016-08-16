#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	if(fopen("distribution.in", "r")) {
		freopen("distribution.in", "r", stdin);
		freopen("distribution.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	int T;
	cin >> T;
	while(T--) {
		ll N; cin >> N;
		vector<ll> res;
		ll p2 = 1;
		ll p3 = 1;
		while(p3 <= N) p3 *= 3;
		while(N) {
			if(N % 2 == 0) {
				p2 *= 2;
				N /= 2;
			} else if (p3 > N){
				p3 /= 3;
			} else {
				N -= p3;
				res.push_back(p3 * p2);
			}
		}
		sort(res.begin(), res.end());
		cout << res.size() << '\n';
		for(int i = 0; i < int(res.size()); i++) {
			if(i) cout << ' ';
			cout << res[i];
		}
		cout << '\n';
	}
}
