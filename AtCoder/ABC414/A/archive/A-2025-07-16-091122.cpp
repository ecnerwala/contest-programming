#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, L, R; cin >> N >> L >> R;
	int ans = 0;
	for (int i = 0; i < N; i++) {
		int X, Y; cin >> X >> Y;
		ans += (X <= L && R <= Y);
	}
	cout << ans << '\n';

	return 0;
}
