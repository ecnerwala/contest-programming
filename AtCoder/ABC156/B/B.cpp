#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	int ans = 0;
	for (; N; N /= K) ans++;
	cout << ans << '\n';

	return 0;
}
