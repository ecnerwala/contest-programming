#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		int64_t v_all = 0;
		for (auto a : A) v_all ^= a;

		const int L = 60;
		std::array<int64_t, 60> basis; basis.fill(0);
		for (auto a : A) {
			// We don't care about bits in v_all
			a &= ~v_all;
			for (int b = L-1; b >= 0; b--) {
				if ((a >> b) & 1) {
					if (!basis[b]) {
						basis[b] = a;
						for (int o = b+1; o < L; o++) basis[o] = std::min(basis[o], basis[o] ^ a);
					}
					a ^= basis[b];
				}
			}
		}

		// now, we have an RREF basis, so just take the max
		int64_t v_some = 0;
		for (auto b : basis) v_some ^= b;
		cout << std::max(v_all, v_some) << '\n';
	}

	return 0;
}
