#include<bits/stdc++.h>
using namespace std;

// cleaning tile a_1 \ldots a_c (base k) checks if at least one of a_1 \ldots a_c is a G

typedef long long ll;

int main() {

	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num++) {
		ll K, C, S;
		cin >> K >> C >> S;
		cout << "Case #" << case_num << ":";
		if(S * C < K) cout << ' ' << "IMPOSSIBLE" << '\n';
		else {
			for(ll i = 0; i < (K + C - 1) / C; i++) {
				ll v = 0;
				for(ll j = 0; j < C; j++) {
					v *= K;
					v += min(K - 1, i * C + j);
				}
				cout << ' ' << v + 1;
			}
			cout << '\n';
		}
	}

	return 0;
}
