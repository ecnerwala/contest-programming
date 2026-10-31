#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 400;
int N;
ll C[MAXN], J[MAXN];
ll go() {
	cin >> N;
	for (int i = 0; i < N; i++) { cin >> C[i] >> J[i]; }
	vector<pair<ll, ll>> slopes;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ll a = C[i] - C[j];
			ll b = J[j] - J[i];
			if (a > 0 && b > 0) {
				ll g = __gcd(a, b);
				a /= g, b /= g;
				slopes.emplace_back(a, b);
			}
		}
	}
	sort(slopes.begin(), slopes.end());
	slopes.resize(unique(slopes.begin(), slopes.end()) - slopes.begin());
	return int(slopes.size()) + 1;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		ll res = go();

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
