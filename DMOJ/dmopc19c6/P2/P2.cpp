#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int A, B; cin >> A >> B;
	int ans = int(1e9);
	for (int p = 2; p <= A; p++) {
		if (A % p) continue;
		int e = 0;
		while (A % p == 0) A /= p, e++;
		int v = 0;
		for (int x = B; x; x /= p) v += x/p;
		ans = min(ans, v / e);
	}
	cout << ans << '\n';

	return 0;
}
