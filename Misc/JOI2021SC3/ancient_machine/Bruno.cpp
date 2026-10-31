#include "Bruno.h"
#include <vector>
#include <array>
#include <cstdint>
#include <cassert>

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
			assert(0 <= v[i] && v[i] < (int64_t(1) << 60));
			assert(c == 0 || c == 1);
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
			assert(0 <= v[i] && v[i] < (int64_t(1) << 60));
			assert(c == 0 || c == -1);
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

void Bruno(int N, int L, std::vector<int> A) {
	init_fib();
	if (!L) {
		for (int i = 0; i < N; i++) {
			Remove(i);
		}
		return;
	}

	std::vector<bool> keep(1e5);
	auto it = A.begin();
	for (int b = 0; b < int(1e5); b += 250) {
		big_num cur;
		for (int i = 0; i < 3; i++) {
			for (int l = 0; l < (i ? 60 : 54); l++) {
				if (*it++) cur.v[i] |= int64_t(1) << l;
			}
		}
		for (int i = 249; i >= 0; i--) {
			if (cur >= fib[i]) {
				cur -= fib[i];
				keep[b+i] = true;
				// also set +1
				keep[b+i+1] = true;
			}
		}
	}
	int x0 = 0, y0 = 0, z0 = 0;
	for (int i = 0; i < 20; i++) { if (*it++) x0 += (1 << i); }
	for (int i = 0; i < 20; i++) { if (*it++) y0 += (1 << i); }
	for (int i = 0; i < 20; i++) { if (*it++) z0 += (1 << i); }
	assert(it == A.end());

	keep[x0] = keep[y0] = keep[z0] = true;

	std::vector<int> to_keep; to_keep.reserve(N);
	for (int i = 0; i < N; i++) {
		if (keep[i]) to_keep.push_back(i);
		else Remove(i);
	}
	assert((int(to_keep.size()) & 1));
	for (int i = int(to_keep.size())-2; i >= 0; i--) {
		Remove(to_keep[i]);
	}
	Remove(to_keep.back());
}
