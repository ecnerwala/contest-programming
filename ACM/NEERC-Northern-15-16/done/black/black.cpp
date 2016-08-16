#include<bits/stdc++.h>
using namespace std;

int main() {
	if(fopen("black.in", "r")) {
		freopen("black.in", "r", stdin);
		freopen("black.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	int b, w; cin >> b >> w;
	cout << 3 << ' ' << 2 * w + 2 * b - 3 << '\n';

	for(int i = 0; i < 2 * w + 2 * b - 3; i++) cout << '.';
	cout << '\n';
	for(int i = 0; i < 2 * w + 2 * b - 3; i++) {
		if(i % 2 == 0 || i >= 2 * b - 2) cout << '@';
		else cout << '.';
	}
	cout << '\n';
	for(int i = 0; i < 2 * w + 2 * b - 3; i++) {
		if(i % 2 == 0) cout << '@';
		else cout << '.';
	}
	cout << '\n';
}
