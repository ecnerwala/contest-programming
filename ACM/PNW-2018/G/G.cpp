#include<bits/stdc++.h>
using namespace std;

int d(int a, int l, int r) {
	if (l <= a && a <= r) return 0;
	return min(abs(l-a), abs(r-a));
}

int main() {

	int X, Y, X1, Y1, X2, Y2; cin >> X >> Y >> X1 >> Y1 >> X2 >> Y2;
	int dx = d(X, X1, X2);
	int dy = d(Y, Y1, Y2);
	cout << fixed << setprecision(3) << sqrt(double(dx * dx + dy * dy)) << '\n';

	return 0;
}
