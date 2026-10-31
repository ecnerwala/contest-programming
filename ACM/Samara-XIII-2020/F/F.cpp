#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	cout << N/2+1 << '\n';
	for (int i = 0; i < N/2; i++) {
		cout << 2 * i+1 << ' ';
	}
	cout << N << '\n';

	return 0;
}
