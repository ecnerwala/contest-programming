#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e5) + 200;
int N;
int T[MAXN];
bool has[600];

struct bignum {
	static const int BASE = int(1e4);
	static const int LEN = 60;

	int vals[LEN];

	bignum& operator *= (int a) {
		int carry = 0;
		for (int i = 0; i < LEN; i++) {
			vals[i] *= a;
			vals[i] += carry;
			carry = vals[i] / BASE;
			vals[i] %= BASE;
		}
		assert(!carry);
		return *this;
	}

	int operator % (int a) {
		int res = 0;
		for (int i = LEN-1; i >= 0; i--) {
			res *= BASE;
			res += vals[i];
			res %= a;
		}
		return res % a;
	}

	void div_print(int a) {
		bool printed = false;
		int rem = 0;
		for (int i = LEN-1; i >= 0; i--) {
			rem *= BASE;
			rem += vals[i];
			int v = rem / a;
			rem %= a;
			if (!printed) {
				if (v) {
					printf("%d", v);
					printed = true;
				}
			} else {
				printf("%04d", v);
			}
		}
		if (!printed) {
			printf("0");
		}
		assert(rem == 0);
	}

	void set_to(int k) {
		assert(k < BASE);
		memset(vals, 0, sizeof(vals));
		vals[0] = k;
	}
} V;

int gcd(int a, int b) { return a ? gcd(b % a, a) : b; }

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &T[i]);
	}
	V.set_to(1);
	memset(has, 0, sizeof(has));
	for (int i = 0; i < N; i++) {
		if (has[T[i]]) continue;
		has[T[i]] = true;
		int m = T[i] / gcd(V % T[i], T[i]);
		if (m == 1) continue;
		V *= m;
	}
	for (int i = 0; i < N; i++) {
		V.div_print(T[i]);
		printf("\n");
	}
	return 0;
}
