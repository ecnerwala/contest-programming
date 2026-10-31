#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int R, C; cin >> R >> C;
	int N; cin >> N;
	int xstart, ystart; cin >> xstart >> ystart;
	string S, T; cin >> S >> T;
	int xlo = 1, xhi = R, ylo = 1, yhi = C; // inclusive range of A winning
	for (int i = N-1; i >= 0; i--) {
		assert(xlo <= xhi && ylo <= yhi);

		switch (T[i]) {
			case 'L':
				yhi++;
				break;
			case 'R': 
				ylo--;
				break;
			case 'U':
				xhi++;
				break;
			case 'D':
				xlo--;
				break;
			default: assert(false);
		}

		assert(xlo <= xhi && ylo <= yhi);

		xlo = max(xlo, 1);
		xhi = min(xhi, R);
		ylo = max(ylo, 1);
		yhi = min(yhi, C);

		assert(xlo <= xhi && ylo <= yhi);

		//cerr << xlo << '-' << xhi << ' ' << ylo << '-' << yhi << '\n';

		switch (S[i]) {
			case 'L':
				ylo++;
				break;
			case 'R': 
				yhi--;
				break;
			case 'U':
				xlo++;
				break;
			case 'D':
				xhi--;
				break;
			default: assert(false);
		}

		//cerr << xlo << '-' << xhi << ' ' << ylo << '-' << yhi << '\n';

		if (!(xlo <= xhi && ylo <= yhi)) {
			cout << "NO" << '\n';
			exit(0);
		}

		assert(xlo <= xhi && ylo <= yhi);
	}

	bool good = (xlo <= xstart && xstart <= xhi) && (ylo <= ystart && ystart <= yhi);
	cout << (good ? "YES" : "NO") << '\n';

	return 0;
}
