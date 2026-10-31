#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	long long v = 0;
	long long ans = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		v += a;
		ans = max(ans, -v);
	}
	cout << ans << '\n';

	return 0;
}
