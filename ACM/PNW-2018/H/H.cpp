#include<bits/stdc++.h>
using namespace std;

const int MAXV = 2e6;
const int V = 1.1e6;

bool isPrime[MAXV];

int main() {
	for (int i = 3; i <= V; i+= 2) {
		isPrime[i] = true;
	}
	for (int p = 3; p <= V; p += 2) {
		if (!isPrime[p])  continue;
		for (int j = p + p; j <= V; j+= p) {
			isPrime[j] = false;
		}
	}
	int x; cin >> x;
	int res = 0;
	while (x > 4) {
		for (int p = 3; true; p += 2) {
			if (isPrime[p] && isPrime[x-p]) {
				res++;
				x -= p + p;
				break;
			}
		}
	}
	assert(x <= 4);
	if (x == 4) res ++, x = 0;
	cout << res << '\n';

	return 0;
}
