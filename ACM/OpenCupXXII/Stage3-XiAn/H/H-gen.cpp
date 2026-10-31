#include <bits/stdc++.h>
using namespace std;

int main() {
	int T = 5e4;
	cout << T << '\n';
	std::mt19937 mt(48);
	std::uniform_int_distribution<int> dist(-1e9, 1e9);
	while (T--) {
		int N = 40;
		cout << N << '\n';
		for (int i = 0; i < N; i++) {
			int x = dist(mt);
			int y = dist(mt);
			int z = dist(mt);
			cout << x << ' ' << y << ' ' << z << '\n';
		}
	}
}
