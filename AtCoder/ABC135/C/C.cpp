#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
ll A[MAXN];
ll B[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N+1; i++) {
		cin >> A[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> B[i];
	}
	ll ans = 0;
	for (int i = 0; i < N+1; i++) {
		if (i) {
			ll v = min(A[i], B[i-1]);
			ans += v;
			A[i] -= v;
			B[i-1] -= v;
		}
		if (i < N) {
			ll v = min(A[i], B[i]);
			ans += v;
			A[i] -= v;
			B[i] -= v;
		}
	}
	cout << ans << '\n';

	return 0;
}
