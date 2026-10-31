#include "Anna.h"
#include <vector>
#include <array>
#include <cassert>
#include <cstdint>

namespace {
struct big_num {
	std::array<std::int64_t, 3> v{0,0,0};
	big_num& operator += (const big_num& o) {
		int64_t c = 0;
		for (int i = 2; i >= 0; i--) {
			v[i] += c;
			v[i] += o.v[i];
			c = (v[i] >> 60);
			v[i] -= (c << 60);
		}
		return *this;
	}
	friend bool operator < (const big_num& a, const big_num& b) { return a.v < b.v; }
	friend bool operator <= (const big_num& a, const big_num& b) { return a.v <= b.v; }
	friend bool operator > (const big_num& a, const big_num& b) { return a.v > b.v; }
	friend bool operator >= (const big_num& a, const big_num& b) { return a.v >= b.v; }
	big_num& operator -= (const big_num& o) {
		int64_t c = 0;
		for (int i = 2; i >= 0; i--) {
			v[i] += c;
			v[i] -= o.v[i];
			c = (v[i] >> 60);
			v[i] -= (c << 60);
		}
		return *this;
	}
};
std::array<big_num, 250> fib;
void init_fib() {
	fib[0] = {{0,0,1}};
	fib[1] = {{0,0,2}};
	for (int i = 2; i < 250; i++) {
		fib[i] = fib[i-1];
		fib[i] += fib[i-2];
	}
}
}

void Anna(int N, std::vector<char> S) {
	init_fib();
	std::vector<bool> keep(N);
	int x0 = 0;
	while (x0 < N && S[x0] != 'X') x0++;
	int z0 = N-1;
	while (z0 >= 0 && S[z0] != 'Z') z0--;
	int y0 = z0;
	while (y0 >= 0 && S[y0] != 'Y') y0--;
	if (x0 >= y0) {
		// just don't send anything, we can't do anything useful anyways
		return;
	}

	assert(0 <= x0 && x0 < y0 && y0 < z0 && z0 < N);

	for (int i = x0+1; i+1 < y0; i++) {
		if (S[i] == 'Y' && S[i+1] == 'X') {
			keep[i] = true;
		}
	}
	keep.resize(int(1e5));

	for (int b = 0; b < int(1e5); b += 250) {
		big_num cur;
		for (int i = 0; i < 250; i++) {
			if (keep[b+i]) {
				cur += fib[i];
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int l = 0; l < (i ? 60 : 54); l++) {
				Send((cur.v[i] >> l) & 1);
			}
		}
	}

	for (int i = 0; i < 20; i++) { Send((x0 >> i) & 1); }
	for (int i = 0; i < 20; i++) { Send((y0 >> i) & 1); }
	for (int i = 0; i < 20; i++) { Send((z0 >> i) & 1); }
}
