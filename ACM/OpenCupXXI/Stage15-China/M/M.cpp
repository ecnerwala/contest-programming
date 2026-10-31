#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		struct pt_t {
			int cnt;
			array<int, 2> x;
			char c;
		};
		array<pt_t, 3> pts;
		for (auto& p : pts) cin >> p.cnt;
		for (auto& p : pts) for (auto& x : p.x) { cin >> x; x--; }
		pts[0].c = 'A';
		pts[1].c = 'B';
		pts[2].c = 'C';

		sort(pts.begin(), pts.end(), [&](const auto& a, const auto& b) { return a.cnt > b.cnt; });
		auto P = pts[0];
		auto A = pts[1];
		auto B = pts[2];

		bool transpose = false;
		if (P.x[1] == B.x[1] || (P.x[0] != B.x[0] && P.x[1] == A.x[1])) {
			transpose = true;
			swap(N, M);
			swap(P.x[0], P.x[1]);
			swap(A.x[0], A.x[1]);
			swap(B.x[0], B.x[1]);
		}
		assert(P.x[1] != B.x[1]);

		auto mod = [&](int a, int m) -> int {
			if (a == -1) return m-1;
			if (a == m) return 0;
			return a;
		};

		auto take_ray = [&](string& r, int y, int z, pt_t& V) {
			assert(z == 1 || z == -1);
			assert(r[y]);
			for (y = mod(y+z, M); V.cnt && !r[y]; y = mod(y+z, M)) {
				r[y] = V.c;
				V.cnt--;
			}
		};

		auto take_row = [&](string& r, pt_t& V) {
			for (int z = -1; V.cnt && z <= 1; z += 2) {
				assert(r[P.x[1]]);
				int y = mod(P.x[1]-z, M); while (!r[y]) y = mod(y-z, M);
				take_ray(r, y, z, V);
			}
		};

		vector<string> ans(N, string(M, '\0'));

		for (int i = 0; i < N; i++) {
			ans[i][P.x[1]] = P.c;
			P.cnt--;
		}

		ans[A.x[0]][A.x[1]] = A.c;
		A.cnt--;
		if (A.x[1] == P.x[1]) {
			P.cnt++;
			assert(A.x[0] != B.x[0]);
			// We actually don't need any special casing because A and B are in
			// different rows, and A.cnt > B.cnt, so there's no possibility of
			// both having Ps in A's row and the B's reaching both sides to
			// block them off.
		}

		ans[B.x[0]][B.x[1]] = B.c;
		B.cnt--;

		if (A.x[0] == B.x[0]) {
			// same row we need to do something special

			auto& r = ans[A.x[0]];
			take_ray(r, A.x[1], +1, A);
			take_ray(r, A.x[1], -1, A);
			take_ray(r, B.x[1], +1, B);
			take_ray(r, B.x[1], -1, B);

			// If there's a gap, then both A.cnt and B.cnt are already 0, so we're gonna be done anyways

			// Otherwise give A up and B down, and start them off on the full row
			assert(N >= 3);
			if (A.cnt) {
				int dA = r[mod(P.x[1]-1, M)] == A.c ? -1 : 1;
				assert(r[mod(P.x[1]+dA, M)] == A.c);
				take_ray(ans[mod(A.x[0]-1, N)], P.x[1], dA, A);
			}
			if (B.cnt) {
				int dB = r[mod(P.x[1]-1, M)] == B.c ? -1 : 1;
				assert(r[mod(P.x[1]+dB, M)] == B.c);
				take_ray(ans[mod(B.x[0]+1, N)], P.x[1], dB, B);
			}

			// Now if A and B need to grow further, they have the full row, so we can just do whatever
		} else {
			// Otherwise just fill in the part (strictly) in between however.
			for (int x = A.x[0]; A.cnt && x != B.x[0]; x = mod(x+1, N)) {
				take_row(ans[x], A);
			}
			for (int x = B.x[0]; B.cnt && x != A.x[0]; x = mod(x-1, N)) {
				take_row(ans[x], B);
			}
		}
		for (int x = mod(A.x[0]-1, N); A.cnt; x = mod(x-1, N)) {
			take_row(ans[x], A);
		}
		for (int x = mod(B.x[0]+1, N); B.cnt; x = mod(x+1, N)) {
			take_row(ans[x], B);
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (!ans[i][j]) {
					ans[i][j] = P.c;
					P.cnt--;
				}
			}
		}

		assert(P.cnt == 0 && A.cnt == 0 && B.cnt == 0);

		if (transpose) {
			for (int j = 0; j < M; j++) {
				for (int i = 0; i < N; i++) {
					cout << ans[i][j];
				}
				cout << '\n';
			}
		} else {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					cout << ans[i][j];
				}
				cout << '\n';
			}
		}
	}

	return 0;
}
