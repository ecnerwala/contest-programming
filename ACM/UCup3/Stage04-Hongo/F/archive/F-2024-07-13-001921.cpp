#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	std::string S_; cin >> S_;
	std::string T_; cin >> T_;

	using bs = bitset<5020>;
	bs S;
	for (int i = 0; i < N; i++) S[i] = S_[i] - '0';
	bs T;
	for (int i = 0; i < N; i++) T[i] = T_[i] - '0';

	bs A;
	{
		A[N] = 1;
		A[0] = 1;
		int P; cin >> P;
		for (int z = 0; z < P; z++) {
			int a; cin >> a; a--;
			A.flip(a);
		}
	}
	bs B;
	{
		int Q; cin >> Q;
		for (int z = 0; z < Q; z++) {
			int b; cin >> b; b--;
			B.flip(b);
		}
	}

	auto deg = [](const bs& a) -> int {
		int r = -1;
		for (int i = 0; i < int(a.size()); i++) {
			if (a[i]) r = i;
		}
		return r;
	};
	// First, we run euclidean algorithm{
	bs gcd, inv_B;
	{
		bs x = A, y = B;
		bs c_x, c_y;
		c_y[0] = 1;
		while (true) {
			int dx = deg(x);
			int dy = deg(y);
			if (dx < dy) {
				std::swap(x, y);
				std::swap(c_x, c_y);
				std::swap(dx, dy);
			}
			assert(dx >= dy);
			if (dy == -1) break;
			assert(dx >= dy);
			x ^= (y << (dx - dy));
			c_x ^= (c_y << (dx - dy));
		}
		assert(!y.any());
	}

	return 0;
}
