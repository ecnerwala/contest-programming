#include<bits/stdc++.h>
using namespace std;

const int MAXV = 6e8;

int phi[MAXV];

long long g(int n) {
	for(int i = 1; i <= n; i++) {
		phi[i] = i;
	}

	for(int i = 2; i <= n; i++) {
		if(phi[i] == i) {
			for(int j = i; j <= n; j += i) {
				phi[j] /= i;
				phi[j] *= i - 1;
			}
		}
	}

	long long res = 0;
	for(int i = 1; i <= n; i++, i++) {
		res += phi[i];
	}
	return res;
}

int main() {
	cout << g(500000000) << '\n';
	return 0;
}
