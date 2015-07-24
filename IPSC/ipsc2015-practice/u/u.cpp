#include<bits/stdc++.h>
using namespace std;

int main() {

	int T; cin >> T;
	while(T--) {

		int d;
		long double p;
		cin >> d >> p;

		cout << fixed << setprecision(18) << (p + (1 - p) / (d - 2) + 1) / d << '\n';
	}

	return 0;
}

// probability 1 / d: 0
// (d - 1) / d * (p * (2/(d-1)) + (1 - p) * (1 / (d - 2)))
