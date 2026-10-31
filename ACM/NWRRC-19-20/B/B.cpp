#include<bits/stdc++.h>
using namespace std;

using ld = long double;

const ld HALF_PI = atan2(1.l, 0.l);
const ld TAU = HALF_PI * 4;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int i = 1;
	while (fmod(i, TAU) * (N/2) >= HALF_PI) i++;
	for (int j = -(N/2); j < N-(N/2); j++) {
		cout << i*j << '\n';
	}

	return 0;
}
