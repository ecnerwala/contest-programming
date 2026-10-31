#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int A, B, N; cin >> A >> B >> N;
	cout << (N - A - 1) / (B - A) * 2 + 1 << '\n';

	return 0;
}
