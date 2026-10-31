#include<bits/stdc++.h>
using namespace std;

bool winning[200][200];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	long long X, Y;
	cin >> X >> Y;
	cout << (abs(X-Y) <= 1 ? "Brown" : "Alice") << '\n';
	return 0;
	for (int s = 0; s < 200; s++) {
		for (int a = 0; a <= s; a++) {
			int b = s - a;
			winning[a][b] = false;
			for (int i = 1; 2*i <= a; i++) {
				if (!winning[a-2*i][b+i]) winning[a][b] = true;
			}
			for (int i = 1; 2*i <= b; i++) {
				if (!winning[b-2*i][a+i]) winning[a][b] = true;
			}
			if (!winning[a][b]) cerr << a << ' ' << b << '\n';
		}
	}

	return 0;
}
