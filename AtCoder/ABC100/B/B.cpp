#include<bits/stdc++.h>
using namespace std;

int main() {
	int N, D;
	cin >> D >> N;

	N--;

	cout << 1 + (N % 99) + (N / 99) * 100;

	for (int i = 0; i < D; i++) {
		cout << "00";
	}
	cout << '\n';

	return 0;
}
