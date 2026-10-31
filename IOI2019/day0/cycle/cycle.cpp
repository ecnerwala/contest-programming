#include "cycle.h"

void escape(int n) {
	int st = 1, en = n;
	while (en - st > 1) {
		int md = (st + en) / 2;
		if (jump(md)) {
			st = 0, en = en - md;
		} else {
			st = n - md + st;
			en = n;
		}
	}
	if (st) {
		jump(st);
	}
}
