#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t n5, n4, n3, n2, n1; cin >> n5 >> n4 >> n3 >> n2 >> n1;
	int64_t ans = 0;
	ans += n5; n5 = 0;
	ans += n4; n1 -= min(n4, n1); n4 = 0;

	ans += n3;
	// as many 2's as possible
	{
		int64_t v = min(n3, n2);
		n2 -= v;
		n3 -= v;

		n1 -= min(n1, n3 * 2);
		n3 = 0;
	}

	// we have just 2's and 1's
	ans += max((n2 + 1) / 2, (n2 * 2 + n1 + 4) / 5);
	cout << ans << '\n';

	return 0;
}
