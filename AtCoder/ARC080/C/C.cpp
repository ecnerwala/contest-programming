#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int c4 = 0;
	int c2 = 0;
	int c1 = 0;
	for (int i = 0; i < N; i++) {
		int v; cin >> v;
		if (v % 4 == 0) c4++;
		else if (v % 2 == 0) c2++;
		else c1++;
	}
	if (c2) c1++;
	cout << (c1 <= c4 + 1 ? "Yes" : "No") << '\n';

	return 0;
}
