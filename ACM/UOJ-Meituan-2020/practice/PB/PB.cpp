#include "find.h"

#include<bits/stdc++.h>

int find_pos(int n, int x) {
	int mi = 0, ma = n+1;
	while (ma - mi > 1) {
		int md = (mi+ma) / 2;
		if (query(md) <= x) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}
