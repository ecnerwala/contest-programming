#include "vision.h"

#include <bits/stdc++.h>
using namespace std;

void construct_network(int H, int W, int K) {
	vector<int> diags[2];
	for (int z = 0; z < 2; z++) {
		diags[z].resize(H+W-1);
		for (int s = 0; s <= H-1 + W-1; s++) {
			vector<int> pts;
			for (int x = 0; x < H; x++) {
				int y = s-x;
				if (z) y = (W-1) - y;
				if (0 <= y && y < W) {
					pts.push_back(x * W + y);
				}
			}
			diags[z][s] = add_or(pts);
		}
	}
	vector<int> pref[2];
	for (int z = 0; z < 2; z++) {
		pref[z].resize(diags[z].size());
		pref[z][0] = diags[z][0];
		for (int v = 1; v <= H-1 + W-1; v++) {
			pref[z][v] = add_or(vector<int>({pref[z][v-1], diags[z][v]}));
		}
	}
	auto get_far = [&](int k) {
		vector<int> pts;
		for (int z = 0; z < 2; z++) {
			for (int v = k; v <= H-1 + W-1; v++) {
				pts.push_back(add_and(vector<int>({pref[z][v-k], diags[z][v]})));
			}
		}
		return add_or(pts);
	};
	int ans;
	if (K == H-1 + W-1) {
		ans = get_far(K);
	} else {
		ans = add_xor(vector<int>({get_far(K), get_far(K+1)}));
	}
}
