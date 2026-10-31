#include <bits/stdc++.h>

int main() {
	int N; std::cin >> N;
	std::vector<int> A(N); for (auto& a : A) { std::cin >> a; a--; }
	std::vector<int> D(N); for (auto& d : D) { std::cin >> d; }
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
	for (int i = 0; i < N; i++) {
		if (i - D[i] >= 0) merge(i, i - D[i]);
		if (i + D[i] <= N-1) merge(i, i + D[i]);
	}
	bool good = true;
	for (int i = 0; i < N; i++) {
		if (get_par(i) != get_par(A[i])) good = false;
	}
	std::cout << (good ? "YES" : "NO") << '\n';
	
}
