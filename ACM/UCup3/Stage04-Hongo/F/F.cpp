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
		gcd = x;
		inv_B = c_x;
	}
	// Now what...
	// Now we want to find the first x so that (S * x^k - T) == 0 mod gcd
	// And also ((S * x^k - T) * inv_B) % (A / gcd) has to have small degree

	int d_gcd = deg(gcd);

	// Let's take it mod gcd
	bs S_gcd = S;
	for (int a = deg(S); a >= d_gcd; a--) {
		if (S_gcd[a]) S ^= (gcd << (a - d_gcd));
	}
	bs T_gcd = T;
	for (int a = deg(T); a >= d_gcd; a--) {
		if (T_gcd[a]) T ^= (gcd << (a - d_gcd));
	}

	int d_A = deg(A);

	bs S_inv_B;
	bs T_inv_B;
	for (int i = deg(inv_B); i >= 0; i--) {
		S_inv_B <<= 1;
		if (S_inv_B[d_A]) S_inv_B ^= A;
		T_inv_B <<= 1;
		if (T_inv_B[d_A]) T_inv_B ^= A;
		if (inv_B[i]) {
			S_inv_B ^= S;
			T_inv_B ^= T;
		}
	}

	const int MX = int(1e6);
	int ans = -1;
	for (int k = 0; k <= MX; k++) {
		if (S_gcd == T_gcd) {
			int d = deg(S_inv_B ^ T_inv_B);
			if (d < k + d_gcd) {
				ans = k;
				break;
			}
		}
		S_gcd <<= 1;
		if (S[d_gcd]) S_gcd ^= gcd;
		S_inv_B <<= 1;
		if (S_inv_B[d_A]) S_inv_B ^= A;
	}
	cout << ans << '\n';

	return 0;
}
