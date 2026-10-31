#include <bits/stdc++.h>

int main() {
	using namespace std;
	std::mt19937 mt(48);
	int T = 100;
	cout << T << '\n';
	for (int t = 0; t < T; t++) {
		int N = std::uniform_int_distribution<int>(1, 100)(mt);
		cout << N << '\n';
		int cur = 0;
		for (int i = 0; i < N; i++) {
		}
	}
}
