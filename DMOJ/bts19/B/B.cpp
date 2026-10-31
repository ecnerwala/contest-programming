#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int X, Y, H, V; cin >> X >> Y >> H >> V;
	H--, V--;
	int T; cin >> T;
	for (int i = 1; i < T; i++) {
		// a + b = 3i
		// max(X, i) <= a <= X + H
		// max(Y, i) <= 3i - a <= Y + V
		// 3i - (Y + V) <= a <= 3i - (max(Y, i))
		if (max(3 * i - (Y + V), max(X, i)) <= min(X + H, 3 * i - max(Y, i))) {
			cout << "YES\n";
			exit(0);
		}
	}
	cout << "NO\n";
	exit(0);

	return 0;
}
