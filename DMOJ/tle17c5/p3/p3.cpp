#include<bits/stdc++.h>
using namespace std;

bool is_prime(int v) {
	if (v <= 1) return false;
	if (v % 2 == 0) return v == 2;
	for (int i = 3; i * i <= v; i++) {
		if (v % i == 0) return false;
	}
	return true;
}

const int MAXN = 300;
int N;
bool unit[MAXN];
bool irreducible[MAXN];
bool prime[MAXN];

int main() {
	scanf("%d", &N);
	assert(N >= 2);

	for (int u = 0; u < N; u++) {
		unit[u] = false;
		for (int v = 0; v < N; v++) {
			if (u * v % N == 1) {
				unit[u] = true;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		irreducible[i] = !(i == 0 || unit[i]);
	}
	for (int a = 0; a < N; a++) {
		if (unit[a]) continue;
		for (int b = 0; b < N; b++) {
			if (unit[b]) continue;
			irreducible[a * b % N] = false;
		}
	}

	for (int i = 0; i < N; i++) {
		prime[i] = false;
		if (i == 0 || unit[i]) continue;
		int g = __gcd(i, N);
		assert(g != 1);
		if (is_prime(g)) prime[i] = true;
	}

	printf("Units:\n");
	for (int i = 0; i < N; i++) {
		if (unit[i]) printf("%d\n", i);
	}
	printf("Irreducibles:\n");
	for (int i = 0; i < N; i++) {
		if (irreducible[i]) printf("%d\n", i);
	}
	printf("Primes:\n");
	for (int i = 0; i < N; i++) {
		if (prime[i]) printf("%d\n", i);
	}
	return 0;
}
