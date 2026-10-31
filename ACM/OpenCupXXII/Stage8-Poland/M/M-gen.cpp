#include <bits/stdc++.h>
using namespace std;

int main() {
	int T = 1;
	cout << T << '\n';
	for (int z = 0; z < T; z++) {
		int N = 6561;
		int M = 1e9;
		std::mt19937_64 mt(48);
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) {
			A[i] = std::uniform_int_distribution<int>(0, M-1)(mt);
		}
		for (int z = 0; z < 30; z++) {
			A[std::uniform_int_distribution<int>(0, N)(mt)] = -1;
		}
		cout << N << ' ' << M << '\n';
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}
}
