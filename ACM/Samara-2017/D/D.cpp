#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int N, X; cin >> N >> X;
	X = abs(X);
	int g = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		g = __gcd(g, a);
	}
	if (X % g == 0) {
		cout << "YES\n";
	} else {
		cout << "NO\n";
	}

	return 0;
}
