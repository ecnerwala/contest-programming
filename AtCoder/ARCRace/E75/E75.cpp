#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
int K;
int A[MAXN];
ll pref[MAXN];

int cnt[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i] -= K;
	}
	vector<ll> vals;
	vals.push_back(pref[0] = 0);
	for (int i = 0; i < N; i++) {
		vals.push_back(pref[i+1] = pref[i] + A[i]);
	}
	sort(vals.begin(), vals.end());
	vals.resize(unique(vals.begin(), vals.end()) - vals.begin());

	ll ans = 0;
	for (int i = 0; i <= N; i++) {
		int v = int(lower_bound(vals.begin(), vals.end(), pref[i]) - vals.begin());
		for (int a = v+1; a; a -= a & (-a)) {
			ans += cnt[a];
		}
		for (int a = v+1; a <= int(vals.size()); a += a & (-a)) {
			cnt[a]++;
		}
	}
	cout << ans << '\n';

	return 0;
}
