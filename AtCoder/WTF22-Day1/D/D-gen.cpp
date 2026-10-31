#include <bits/stdc++.h>
using namespace std;
int main() {
	int N = int(1e6);
	int M = int(1e6);
	cout << N << ' ' << M << '\n';
	std::mt19937 rng(48);
	std::uniform_int_distribution<int> dist(1, N);
	for (int i = 0; i < M; i++) {
		int x = dist(rng);
		int y = dist(rng);
		if (x > y) std::swap(x, y);
		cout << x << ' ' << y << '\n';
	}
}
