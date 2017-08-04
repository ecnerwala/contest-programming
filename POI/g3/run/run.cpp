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

	void div_print(int a, char* out) {
		bool printed = false;
		int rem = 0;
		for (int i = LEN-1; i >= 0; i--) {
			rem *= BASE;
			rem += vals[i];
			int v = rem / a;
			rem %= a;
			if (!printed) {
				if (v) {
					out += sprintf(out, "%d", v);
					printed = true;
				}
			} else {
				out += sprintf(out, "%04d", v);
			}
		}
		if (!printed) {
			out += sprintf(out, "0");
		}
		out += sprintf(out, "\n");
		assert(rem == 0);
	}

	void set_to(int k) {
		assert(k < BASE);
		memset(vals, 0, sizeof(vals));
		vals[0] = k;
	}
} V;

int gcd(int a, int b) { return a ? gcd(b % a, a) : b; }

char outputs[600][600];

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
	for (int i = 1; i <= 500; i++) {
		if (has[i]) V.div_print(i, outputs[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("%s", outputs[T[i]]);
	}
	return 0;
}
