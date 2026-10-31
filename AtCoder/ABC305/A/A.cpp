#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int a = N / 5 * 5;
	if (a != N && N % 5 >= 3) {
		a += 5;
	}
	cout << a << '\n';

	return 0;
}
