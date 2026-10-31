#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t tot = 0;

	int T; cin >> T;
	while (T--) {
		int n, m; cin >> n >> m;
		string A, B; cin >> A >> B;
		if (A.back() == 'R') std::swap(A, B);
		assert(A.back() == 'D');
		assert(B.back() == 'R');
		assert(A.front() == 'R');
		assert(B.front() == 'D');

		std::reverse(A.begin(), A.end());
		std::reverse(B.begin(), B.end());
		int vl = 0, vr = 0;
		for (int i = 1; i+1 < int(A.size()); i++) {
			bool l_grow = A[i] == 'D';
			bool r_grow = B[i] == 'R';
			if (l_grow) {
				vl--;
				if (vl > 0) {
					vr -= vl;
					vl = 0;
				}
			} else {
				vl++;
			}

			if (r_grow) {
				vr++;
				if (vr < 0) {
					vl -= vr;
					vr = 0;
				}
			} else {
				vr--;
			}
		}
		assert(vl == vr);
		tot += vl;
	}

	cout << (tot > 0 ? "First" : "Second") << '\n';

	return 0;
}
