#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

std::optional<std::array<std::array<int, 5>, 2>> solve_grid(int N, int M, std::array<std::array<int, 5>, 5> A, int D) {
	std::array<int, 5> X;
	for (int i = 0; i < N; i++) X[i] = i+1;
	while (true) {
		std::array<int, 5> Y_hi;
		for (int j = 0; j < M; j++) {
			Y_hi[j] = A[0][j] + D;
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				setmin(Y_hi[j], (A[i][j] + D) / X[i]);
			}
		}
		for (int j = M-1; j > 0; j--) {
			setmin(Y_hi[j-1], Y_hi[j] - 1);
		}

		if (Y_hi[M-1] < X[N-1]) return std::nullopt;
		for (int j = 0; j < M; j++) {
			if (Y_hi[j] < 1) return std::nullopt;
		}

		bool changed = false;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if ((std::max(1, A[i][j] - D) + X[i] - 1) / X[i] > Y_hi[j]) {
					changed = true;
					X[i]++;
					break;
				}
			}
		}

		if (!changed) {
			return {{X, Y_hi}};
		}

		for (int i = 1; i < N; i++) {
			setmax(X[i], X[i-1]+1);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::array<std::array<int, 5>, 5> A;
	std::array<std::array<int, 5>, 5> A_trans;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> A[i][j];
			A_trans[j][i] = A[i][j];
		}
	}

	auto solve = [&](int D) -> std::optional<std::array<std::array<int, 5>, 2>> {
		auto r = solve_grid(N, M, A, D);
		if (r) return r;
		r = solve_grid(M, N, A_trans, D);
		if (r) {
			std::swap((*r)[0], (*r)[1]);
			return r;
		}
		return std::nullopt;
	};

	int lo = -1;
	int hi = 0;
	std::array<std::array<int, 5>, 2> sol;
	for (int i = 0; i < N; i++) sol[0][i] = i+1;
	for (int j = 0; j < M; j++) sol[1][j] = j+1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			hi = std::max<int>(hi, std::abs(A[i][j] - sol[0][i] * sol[1][j]));
		}
	}

	while (hi - lo > 1) {
		int md = lo + (hi - lo) / 2;
		auto cnd = solve(md);
		if (cnd) {
			hi = md;
			sol = *cnd;
		} else {
			lo = md;
		}
	}
	cout << hi << '\n';
	for (int i = 0; i < N; i++) {
		cout << sol[0][i] << " \n"[i+1==N];
	}
	for (int j = 0; j < M; j++) {
		cout << sol[1][j] << " \n"[j+1==M];
	}

	return 0;
}
