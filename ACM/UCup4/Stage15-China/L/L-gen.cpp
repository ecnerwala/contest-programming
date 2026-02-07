#include <bits/stdc++.h>
using namespace std;
int main() {
	std::mt19937 mt(48);
	int T = 1000;
	cout << T << '\n';
	while (T--) {
		int N = std::uniform_int_distribution<int>(1, 50)(mt);
		cout << N << '\n';
		cout << 0;
		for (int i = 2; i <= N; i++) {
			cout << ' ' << std::uniform_int_distribution<int>(1, i-1)(mt);
		}
		cout << '\n';
	}
}
