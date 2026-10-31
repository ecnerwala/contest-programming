#include <bits/stdc++.h>

// Step 1: think about string of > and <
//INF 3 2 1 4 5 INF
//   > > > < < <
// We're looking for consecutive > and <
// Step 2: pseudo-binary search:
//  If the middle symbol is <, then there's a local min somewhere to the left
//  If the middle symbol is >, then there's a local min somewhere to the right
//  Eventually, your range will shink to just 2 adjacent elements
// If your current range starts > and ends <, then there's always a local min somewhere, even if it's not monotonic.

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int lo = 0;
	int hi = N;
	while (hi - lo > 1) {
		int md = (lo + hi) / 2;
		cout << '?' << ' ' << md << '\n' << flush;
		int a0; cin >> a0;
		cout << '?' << ' ' << md+1 << '\n' << flush;
		int a1; cin >> a1;
		if (a0 < a1) {
			hi = md;
		} else {
			lo = md;
		}
	}
	cout << '!' << ' ' << hi << '\n';

	return 0;
}
