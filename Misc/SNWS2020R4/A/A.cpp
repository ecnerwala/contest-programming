#include<bits/stdc++.h>
using namespace std;


int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	// there are 3 moves: up, down-left, down-right
	// a + b + c = 0, so there are at most 2 of them used at any point
	// the points that are valid are a <= 5, b <= 5, c <= 5
	// two points are connected by a rook's move if it's (y,x,x) in some order
	
	int a0 = 0, b0 = 0, c0 = 0;
	{
		char col; int row; cin >> col >> row;
		a0 = row - 6;
		if (col <= 'f') {
			c0 -= 'f' - col;
		} else {
			b0 -= col - 'f';
		}
		int m = min({a0, b0, c0});
		a0 -= m, b0 -= m, c0 -= m;
	}
	int a1 = 0, b1 = 0, c1 = 0;
	{
		char col; int row; cin >> col >> row;
		a1 = row - 6;
		if (col <= 'f') {
			c1 -= 'f' - col;
		} else {
			b1 -= col - 'f';
		}
		int m = min({a1, b1, c1});
		a1 -= m, b1 -= m, c1 -= m;
	}
	cerr << a0 << ' ' << b0 << ' ' << c0 << '\n';
	cerr << a1 << ' ' << b1 << ' ' << c1 << '\n';

	auto isGood = [&](int da, int db, int dc) {
		return ((da == db) + (db == dc) + (dc == da)) == 1;
	};
	int tot = 0;
	int ans = 0;
	for (int a = 0; a <= 5; a++) {
		for (int b = 0; b <= 5; b++) {
			for (int c = 0; c <= 5; c++) {
				if (a && b && c) continue;
				tot++;
				if (isGood(a0-a,b0-b,c0-c) && isGood(a1-a,b1-b,c1-c)) {
					ans++;
				}
			}
		}
	}
	assert(tot == 91);
	cout << ans << '\n';

	return 0;
}
