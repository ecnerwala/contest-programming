#include <bits/stdc++.h>

int main() {
	using namespace std;

	std::mt19937 mt(48);

	int N = int(5e5);
	int Q = int(3e5);
	std::vector<int> par(N, -1);
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
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	for (int e = 0; e < N-1; e++) {
		int u, v;
		while (true) {
			u = std::uniform_int_distribution(0, N-1)(mt);
			v = std::uniform_int_distribution(0, N-1)(mt);
			if (merge(u, v)) break;
		}
		cout << u+1 << ' ' << v+1 << '\n';
	}
	for (int q = 0; q < Q; q++) {
		char op = "AD"[std::uniform_int_distribution(0, 1)(mt)];
		if (op == 'A') {
			int x = std::uniform_int_distribution(0, N-1)(mt);
			int y = std::uniform_int_distribution(0, N-1)(mt);
			int z = std::uniform_int_distribution(1, 370)(mt);
			cout << op << ' ' << x+1 << ' ' << y+1 << ' ' << z << '\n';
		} else if (op == 'D') {
			int x = std::uniform_int_distribution(0, N-1)(mt);
			int h = std::uniform_int_distribution(1, int(1e8))(mt);
			cout << op << ' ' << x+1 << ' ' << h << '\n';
		} else assert(false);
	}
}
