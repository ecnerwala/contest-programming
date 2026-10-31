#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = 0;
		while ((ans/2 + 1) * ((ans+1)/2+1) < N) ans++;
		cout << ans << '\n';
	}

	return 0;
}
