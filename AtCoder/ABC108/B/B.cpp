#include<bits/stdc++.h>
using namespace std;

int main() {
	using pt = complex<int>;
	int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
	pt a(x1, y1);
	pt b(x2, y2);
	pt c = b + (b - a) * pt(0, 1);
	pt d = c + (c - b) * pt(0, 1);
	cout << c.real() << ' ' << c.imag() << ' ' << d.real() << ' ' << d.imag() << '\n';

	return 0;
}
