#include <bits/stdc++.h>

int main() {
	using namespace std;
	std::mt19937 mt(48);
	int K = 1000;
	std::cout << K << '\n';
	for (int i = 0; i < K; i++) {
		int d = std::uniform_int_distribution<int>(0, 100 * 10000)(mt);
		int r = std::uniform_int_distribution<int>(100, 100 * 10000)(mt);
		int t = std::uniform_int_distribution<int>(100, 100 * 10000)(mt);
		std::cout << fixed << setprecision(4);
		cout << d / 1e4 << ' ' << r / 1e4 << ' ' << t / 1e4 << '\n';;
	}
}
