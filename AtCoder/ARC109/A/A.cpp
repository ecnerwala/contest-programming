#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int A, B, X, Y; cin >> A >> B >> X >> Y;
	cout << X + min(2*X,Y) * (B >= A ? B - A : A-1-B) << '\n';

	return 0;
}
