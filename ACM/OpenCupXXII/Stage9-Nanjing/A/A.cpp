#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, A, B; cin >> N >> A >> B;
	if (A-1 <= N-A) {
		cout << string(N-1, 'U');
		cout << string(A-1, 'D');
	} else {
		cout << string(N-1, 'D');
		cout << string(N-A, 'U');
	}

	if (B-1 <= N-B) {
		cout << string(N-1, 'L');
		cout << string(B-1, 'R');
	} else {
		cout << string(N-1, 'R');
		cout << string(N-B, 'L');
	}
	cout << '\n';

	return 0;
}
