#include <bits/stdc++.h>

int main() {
	using namespace std;
	int T = 1; cout << T << '\n';
	std::mt19937 mt(48);
	while (T--) {
		int N = 1000;
		cout << N << '\n';
		for (int z = 0; z < N; z++) cout << mt() % 1000 << ' ' << mt() % 1000 << '\n';
		cout << -1000000000 << ' ' << -1000000000 << '\n';
		for (int z = 0; z < N; z++) cout << mt() % 1000 << ' ' << mt() % 1000 << '\n';
	}
}
