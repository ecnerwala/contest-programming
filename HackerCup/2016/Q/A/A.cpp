#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 3e4;
int N;
ll X[MAXN], Y[MAXN];

inline ll sq(ll a) { return a * a; }

int main() {
	ios_base::sync_with_stdio(0);

	int T; cin >> T;
	for(int __T = 1; __T <= T; __T ++) {
		cin >> N;
		for(int i = 0; i < N; i++) {
			cin >> X[i] >> Y[i];
		}
		int res = 0;
		for(int i = 0; i < N; i++) {
			unordered_map<ll, int> mp;
			for(int j = 0; j < N; j++) {
				ll dist = sq(X[i] - X[j]) + sq(Y[i] - Y[j]);
				res += mp[dist]++;
			}
		}
		cout << "Case #" << __T << ": " << res << '\n';
	}
	return 0;
}
