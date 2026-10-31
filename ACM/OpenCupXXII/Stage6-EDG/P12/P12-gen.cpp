#include <bits/stdc++.h>
int main() {
	using namespace std;

	int N = 500;
	int Q = 300000;
	cout << N << ' ' << Q << '\n';
	mt19937 mt(48);
	for (int i = 0; i < N; i++) {
		cout << mt() % 10000 + 1 << " \n"[i+1==N];
	}
	for (int i = 0; i < N; i++) {
		cout << mt() % 10000 + 1 << " \n"[i+1==N];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << mt() % 10000 + 1 << " \n"[j+1==N];
		}
	}
	for (int q = 0; q < Q; q++) {
		int a = mt() % N + 1, b = mt() % N + 1;
		int c = mt() % N + 1, d = mt() % N + 1;
		cout << min(a, b) << ' ' << max(a, b) << ' ' << min(c, d) << ' ' << max(c, d) << '\n';
	}
}
