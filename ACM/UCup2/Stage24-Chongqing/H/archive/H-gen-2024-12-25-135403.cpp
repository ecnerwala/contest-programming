#include <bits/stdc++.h>

int main() {
	using namespace std;
	std::mt19937 mt(48);
	int N = 10;
	int Q = 100;
	cout << N << ' ' << Q << '\n';
	std::vector<int> par(N);
	par[0] = -1;
	for (int i = 1; i < N; i++) {
		par[i] = std::uniform_int_distribution(0, i-1)(mt);
		cout << par[i]+1 << " \n"[i+1==N];
	}
	std::vector<int> A(N);
	for (int i = 0; i < N; i++) {
		A[i] = std::uniform_int_distribution(0, 100)(mt);
		cout << A[i] << " \n"[i+1==N];
	}
	std::vector<int> B(N);
	for (int i = 0; i < N; i++) {
		B[i] = std::uniform_int_distribution(0, 100)(mt);
		cout << B[i] << " \n"[i+1==N];
	}

	for (int q = 0; q < Q; q++) {
		int op = std::uniform_int_distribution(1, 2)(mt);
		if (op == 1) {
			int p = std::uniform_int_distribution(0, N-1)(mt);
			int a = std::uniform_int_distribution(0, 100)(mt);
			int b = std::uniform_int_distribution(0, 100)(mt);
			cout << op << ' ' << p+1 << ' ' << a << ' ' << b << '\n';
		} else if (op == 2) {
			int c = std::uniform_int_distribution(0, N-1)(mt);
			int d = std::uniform_int_distribution(0, N-2)(mt);
			d += (d >= c);
			cout << op << ' ' << c+1 << ' ' << d+1 << '\n';
		} else assert(false);
	}
}
