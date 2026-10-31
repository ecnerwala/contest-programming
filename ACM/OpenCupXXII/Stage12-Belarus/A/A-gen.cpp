#include <bits/stdc++.h>
int main(int argc, char const *argv[]) {
	using namespace std;
	assert(argc >= 3);

	int N = atoi(argv[1]);
	mt19937 mt(atoi(argv[2]));
	cout << N << '\n';
	for (int i = 1; i < N; i++) {
		cout << (mt() % i)+1 << ' ' << i+1 << '\n';
	}
	int Q = N;
	cout << Q << '\n';
	for (int q = 0; q < Q; q++) {
		cout << (mt() % N) + 1 << '\n';
	}
}
