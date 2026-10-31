#include<bits/stdc++.h>

void bit_set(int addr);
int bit_get(int addr);

namespace ecnerwala {

const int BASE_6 = 1;
constexpr int coord_6(int value) { return BASE_6 + (value >> 6); }
const int BASE_2 = coord_6(4095) + 1;
constexpr int coord_2(int value) { return BASE_2 + (value >> 10); }
const int BASE_4 = coord_2(4095) + 1;
constexpr int coord_4(int value) { return BASE_4 + ((value >> 8) & 3); }
const int BASE_10 = coord_4(4095) + 1;
constexpr int coord_10(int value) { return BASE_10 + ((value >> 2) & 15); }
const int BASE_8 = coord_10(4095) + 1;
constexpr int coord_8(int value) { return BASE_8 + ((value >> 4) & 3); }
const int BASE_12 = coord_8(4095) + 1;
constexpr int coord_12(int value) { return BASE_12 + ((value >> 0) & 3); }
const int END = coord_12(4095) + 1;
static_assert(END < 10240);

void remember(int value) {
	bit_set(coord_6(value));
	bit_set(coord_2(value));
	bit_set(coord_4(value));

	bit_set(coord_10(value));
	bit_set(coord_8(value));
	bit_set(coord_12(value));
}

int signum(int a) { return (a > 0) - (a < 0); }
int cmp(int a, int b) {
	return signum(b - a);
}

int compare(int value) {
	if (!bit_get(coord_6(value))) {
		// mismatch
		if (!bit_get(coord_2(value))) {
			if (bit_get(coord_2(value ^ (1 << 10)))) {
				return cmp(value ^ (1 << 10), value);
			} else {
				return cmp(value ^ (1 << 11), value);
			}
		} else {
			if (!bit_get(coord_4(value))) {
				// mismatch on first 4
				if (bit_get(coord_4(value ^ (1 << 8)))) {
					return cmp(value ^ (1 << 8), value);
				} else {
					return cmp(value ^ (1 << 9), value);
				}
			} else {
				// mismatch on first 6
				if (bit_get(coord_6(value ^ (1 << 6)))) {
					return cmp(value ^ (1 << 6), value);
				} else {
					return cmp(value ^ (1 << 7), value);
				}
			}
		}
	} else {
		if (!bit_get(coord_10(value))) {
			if (!bit_get(coord_8(value))) {
				// mismatch on first 8
				if (bit_get(coord_8(value ^ (1 << 4)))) {
					return cmp(value ^ (1 << 4), value);
				} else {
					return cmp(value ^ (1 << 5), value);
				}
			} else {
				// mismatch on first 10
				if (bit_get(coord_10(value ^ (1 << 2)))) {
					return cmp(value ^ (1 << 2), value);
				} else {
					return cmp(value ^ (1 << 3), value);
				}
			}
		} else {
			if (!bit_get(coord_12(value))) {
				// mismatch on first 12
				if (bit_get(coord_12(value ^ (1 << 0)))) {
					return cmp(value ^ (1 << 0), value);
				} else {
					return cmp(value ^ (1 << 1), value);
				}
			} else {
				return 0;
			}
		}
	}
}

} // namespace ecnerwala

void remember(int value) {
	ecnerwala::remember(value);
}

int compare(int value) {
	return ecnerwala::compare(value);
}

#define TESTING 0
#if TESTING
bool buf[10240];
void bit_set(int addr) { buf[addr] = true; }
int bit_get(int addr) { return buf[addr]; }

int main() {
	for (int a = 0; a < 4096; a++) {
		memset(buf, 0, sizeof(buf));
		remember(a);
		for (int b = 0; b < 4096; b++) {
			int expected = (b < a ? -1 : (b == a ? 0 : 1));
			assert(compare(b) == expected);
			//std::cerr << a << ' ' << b << ' ' << compare(b) << ' ' << expected << '\n';
		}
	}
}
#endif
