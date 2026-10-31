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
		for (auto& p : pts) {
			for (auto& x : p.x) {
				cin >> x; x--;
			}
		}
		pts[0].c = 'A';
		pts[1].c = 'B';
		pts[2].c = 'C';
		if (pts[0].cnt >= pts[1].cnt && pts[1].cnt >= pts[2].cnt) {
		} else {
			sort(pts.begin(), pts.end(), [&](const auto& a, const auto& b) { return a.cnt < b.cnt; });
			reverse(pts.begin(), pts.end());
		}
		//cerr << "Start case" << '\n'; cerr << N << ' ' << M << '\n'; for (auto p : pts) cerr << p.cnt << ' ' << p.x[0] << ',' << p.x[1] << '\n'; cerr << '\n';
		auto P = pts[0];
		auto A = pts[1];
		auto B = pts[2];
		bool transpose = false;

		auto mod = [&](int a, int m) -> int {
			a %= m;
			if (a < 0) a += m;
			return a;
		};

		if (P.x[0] == B.x[0] || (P.x[1] != B.x[1] && P.x[0] == A.x[0])) {
			transpose = true;
			swap(N, M);
			swap(P.x[0], P.x[1]);
			swap(A.x[0], A.x[1]);
			swap(B.x[0], B.x[1]);
		}
		assert(P.x[0] != B.x[0]);
		//cerr << "transpose = " << transpose << '\n';

		vector<string> ans(N, string(M, '\0'));

		auto take_col = [&](int y, pt_t& V) {
			for (int z = -1; z <= 1; z += 2) {
				assert(ans[P.x[0]][y]);
				int x = mod(P.x[0]-z, N); while (!ans[x][y]) x = mod(x-z, N);
				for (x = mod(x+z, N); V.cnt && !ans[x][y]; x = mod(x+z, N)) {
					ans[x][y] = V.c;
					V.cnt--;
				}
			}
		};

		for (int j = 0; j < M; j++) {
			ans[P.x[0]][j] = P.c;
			P.cnt--;
		}

		ans[A.x[0]][A.x[1]] = A.c;
		A.cnt--;
		if (A.x[0] == P.x[0]) P.cnt++;

		ans[B.x[0]][B.x[1]] = B.c;
		B.cnt--;

		for (int x = mod(A.x[0]+1, N); A.cnt > 0 && !ans[x][A.x[1]]; x = mod(x+1, N)) {
			ans[x][A.x[1]] = A.c;
			A.cnt--;
		}
		for (int x = mod(A.x[0]-1, N); A.cnt > 0 && !ans[x][A.x[1]]; x = mod(x-1, N)) {
			ans[x][A.x[1]] = A.c;
			A.cnt--;
		}
		for (int x = mod(B.x[0]+1, N); B.cnt > 0 && !ans[x][B.x[1]]; x = mod(x+1, N)) {
			ans[x][B.x[1]] = B.c;
			B.cnt--;
		}
		for (int x = mod(B.x[0]-1, N); B.cnt > 0 && !ans[x][B.x[1]]; x = mod(x-1, N)) {
			ans[x][B.x[1]] = B.c;
			B.cnt--;
		}

		if (A.x[1] == B.x[1]) {
			// same column we need to do something special
			// If there's a gap, then both A.cnt and B.cnt are already 0, so we're gonna be done anyways
			int dA = 0, dB = 0;
			if (A.cnt) {
				dA = ans[mod(P.x[0]-1, N)][A.x[1]] == A.c ? -1 : 1;
				assert(ans[mod(P.x[0]+dA, N)][A.x[1]] == A.c);
			}
			if (B.cnt) {
				dB = ans[mod(P.x[0]-1, N)][B.x[1]] == B.c ? -1 : 1;
				assert(ans[mod(P.x[0]+dB, N)][B.x[1]] == B.c);
			}
			assert(M >= 3);
			for (int x = mod(P.x[0]+dA, N); A.cnt && !ans[x][mod(A.x[1]-1, M)]; x = mod(x+dA, N)) {
				ans[x][mod(A.x[1]-1, M)] = A.c;
				A.cnt--;
			}
			for (int x = mod(P.x[0]+dB, N); B.cnt && !ans[x][mod(B.x[1]+1, M)]; x = mod(x+dB, N)) {
				ans[x][mod(B.x[1]+1, M)] = B.c;
				B.cnt--;
			}

			// Now if A and B need to grow further, they have the full row, so we can just do whatever
			for (int y = mod(A.x[1]-2, M); A.cnt; y = mod(y-1, M)) {
				take_col(y, A);
			}
			for (int y = mod(B.x[1]+2, M); B.cnt; y = mod(y+1, M)) {
				take_col(y, B);
			}
		} else {
			for (int y = mod(A.x[1]+1, M); A.cnt && y != B.x[1]; y = mod(y+1, M)) {
				take_col(y, A);
			}
			for (int y = mod(A.x[1]-1, M); A.cnt; y = mod(y-1, M)) {
				take_col(y, A);
			}
			for (int y = mod(B.x[1]-1, M); B.cnt && y != A.x[1]; y = mod(y-1, M)) {
				take_col(y, B);
			}
			for (int y = mod(B.x[1]+1, M); B.cnt; y = mod(y+1, M)) {
				take_col(y, B);
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (!ans[i][j]) {
					ans[i][j] = P.c;
					P.cnt--;
				}
			}
		}

		//for (string s : ans) cerr << s << '\n';

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

		//cerr << P.cnt << ' ' << A.cnt << ' ' << B.cnt << '\n';
		assert(P.cnt == 0 && A.cnt == 0 && B.cnt == 0);

		vector<int> par(N*M, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int num_cc = N*M;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (ans[i][j] == ans[mod(i+1,N)][j]) {
					num_cc -= merge(i*M+j, mod(i+1,N)*M+j);
				}
				if (ans[i][j] == ans[i][mod(j+1,M)]) {
					num_cc -= merge(i*M+j, i*M+mod(j+1,M));
				}
			}
		}

		int num_P = 0;
		int num_A = 0;
		int num_B = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (get_par(i*M+j) != i*M+j) continue;
				if (ans[i][j] == P.c) num_P++;
				else if (ans[i][j] == A.c) num_A++;
				else if (ans[i][j] == B.c) num_B++;
				else assert(false);
			}
		}

		if (num_cc != 3) {
			assert(A.x[0] == P.x[0]);
			assert(false);
		}
	}

	return 0;
}
