#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int H1, W1, H2, W2;
	cin >> H1 >> W1 >> H2 >> W2;
	int T1 = (H2 - 1) * W1;
	int T2 = (H1 - 1) * W2;
	if (T1 < T2) {
		cout << 1 << '\n';
	} else if (T2 < T1) {
		cout << 2 << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
