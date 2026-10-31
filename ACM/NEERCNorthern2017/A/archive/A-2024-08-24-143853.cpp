#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int w, h; cin >> w >> h;
	int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
	bool swap = false;
	if (x1 == x2) {
		swap = true;
		std::swap(w, h);
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	assert(x1 != x2);
	int a1, b1, a2, b2;
	if (y1 > y2) {
		a1 = x1, b1 = 0;
		a2 = x2, b2 = h;
	} else {
		a1 = x1, b1 = h;
		a2 = x2, b2 = 0;
	}

	if (swap) {
		std::swap(a1, b1);
		std::swap(a2, b2);
	}

	cout << a1 << ' ' << b1 << ' ' << a2 << ' ' << b2 << '\n';

	return 0;
}
