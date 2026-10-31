#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int K, N; cin >> K >> N;
	string A, B; cin >> A >> B;

	cout << 5 * K + 5 << '\n';

	// generate 2^0
	cout << "and " << 1 << ' ' << 2 << ' ' << (30 + 0) << '\n';
	// propogate 2^0
	cout << "or " << 1 << ' ' << 2 << ' ' << (60 + 0) << '\n';
	for (int i = 0; i < K; i++) {
		// generate 2^(i+1)
		cout << "shift " << (30 + i) << ' ' << (1 << i) << ' ' << (30 + i+1) << '\n';
		cout << "and " << (60 + i) << ' ' << (30 + i+1) << ' ' << (30 + i+1) << '\n';
		cout << "or " << (30 + i) << ' ' << (30 + i+1) << ' ' << (30 + i+1) << '\n';

		// propogate 2^i+1
		cout << "shift " << (60 + i) << ' ' << (1 << i) << ' ' << (60 + i+1) << '\n';
		cout << "and " << (60 + i) << ' ' << (60 + i+1) << ' ' << (60 + i+1) << '\n';
	}

	cout << "shift " << (30 + K) << ' ' << 1 << ' ' << 100 << '\n';
	cout << "xor " << 1 << ' ' << 100 << ' ' << 100 << '\n';
	cout << "xor " << 2 << ' ' << 100 << ' ' << 100 << '\n';

	cout << "! " << 100 << '\n';

	return 0;
}
