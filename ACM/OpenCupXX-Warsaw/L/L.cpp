#pragma GCC optimize("Ofast", "unroll-loops")

#include<bits/stdc++.h>
using namespace std;

using ull = uint64_t;
ull nimProd2[64][64];
ull makeNimProd2(int i, int j) {
	if (nimProd2[i][j]) return nimProd2[i][j];
	if ((i & j) == 0) return nimProd2[i][j] = 1ull << (i|j);
	int a = (i&j) & -(i&j);
	return nimProd2[i][j] = makeNimProd2(i ^ a, j) ^ makeNimProd2((i ^ a) | (a-1), (j ^ a) | (i & (a-1)));
}
ull nimProdSlow(ull x, ull y) {
	ull res = 0;
	for (int i = 0; i<64 && x >> i; i++)
		if ((x >> i) & 1)
			for (int j = 0; j<64 && y >> j; j++)
				if ((y >> j) & 1)
					res ^= nimProd2[i][j];
	return res;
}

ull precomp[8][8][256][256];

ull nimProd(ull x, ull y) {
	ull ans = 0;
	for (int a = 0; a < 8; a++) {
		for (int b = 0; b < 8; b++) {
			ans ^= precomp[a][b][(x >> (a*8))&255][(y>>(b*8))&255];
		}
	}
	//assert(ans == nimProdSlow(x, y));
	return ans;
}

ull nimPow(ull x, ull y) {
	ull ans = 1;
	while (y) {
		if (y & 1) ans = nimProd(ans, x);
		x = nimProd(x, x);
		y /= 2;
	}
	return ans;
}

ull nimInv(ull x) {
	ull y = nimPow(x, ull(-2));
	assert(nimProd(x, y) == 1);
	return y;
}

int N;
ull A[200][200];
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			makeNimProd2(i,j);
		}
	}
	for (int a = 0; a < 8; a++) {
		for (int b = 0; b < 8; b++) {
			for (ull x = 0; x < 256; x++) {
				for (ull y = 0; y < 256; y++) {
					for (int i = 0; i < 8; i++)
						if (x & (1<<i))
							for (int j = 0; j < 8; j++)
								if (y & (1<<j))
									precomp[a][b][x][y] ^= nimProd2[8*a+i][8*b+j];
				}
			}
		}
	}

	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> A[i][j];
		}
	}

	ull ans = 1;
	for (int i = 0; i < N; i++) {
		{
			int k;
			for (k = i; k < N; k++) {
				if (A[k][i]) {
					break;
				}
			}
			if (k == N) {
				ans = 0;
				break;
			}
			if (k > i) {
				for (int j = i; j < N; j++) {
					swap(A[i][j], A[k][j]);
				}
			}
		}
		ans = nimProd(ans, A[i][i]);
		ull inv = nimInv(A[i][i]);
		for (int j = i+1; j < N; j++) {
			A[i][j] = nimProd(inv, A[i][j]);
		}
		for (int k = i+1; k < N; k++) {
			for (int j = i+1; j < N; j++) {
				A[k][j] ^= nimProd(A[k][i], A[i][j]);
			}
		}
	}
	//cerr << ans << '\n';
	cout << (ans == 0 ? "Second" : "First") << '\n';

	return 0;
}
