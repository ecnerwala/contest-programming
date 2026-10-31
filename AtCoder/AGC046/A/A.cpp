#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int X; cin >> X;
	cout << 360 / __gcd(X, 360) << '\n';

	return 0;
}
