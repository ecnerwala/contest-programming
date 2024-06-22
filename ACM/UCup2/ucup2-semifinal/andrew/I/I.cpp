#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	const int P = 7;
	for (int a00 = 0; a00 < P; a00++) {
		for (int a01 = 0; a01 < P; a01++) {
			for (int a02 = 0; a02 < P; a02++) {
				for (int a10 = 0; a10 < P; a10++) {
					for (int a11 = 0; a11 < P; a11++) {
						for (int a12 = 0; a12 < P; a12++) {
							int b00 = (a00 * a00 + a01 * a01 + a02 * a02) % 5;
							int b01 = (a00 * a10 + a01 * a11 + a02 * a12) % 5;
							int b10 = (a10 * a00 + a11 * a01 + a12 * a02) % 5;
							int b11 = (a10 * a10 + a11 * a11 + a12 * a12) % 5;
							if (b01 == 0 && b10 == 0 && b00 == 2 && b11 == 2) {
								cerr << a00 << ' ' << a01 << ' ' << a02 << '\n';
								cerr << a10 << ' ' << a11 << ' ' << a12 << '\n';
								exit(0);
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
