#include "perm.h"
#include <bits/stdc++.h>

std::vector<int> construct_permutation(long long k) {
	std::vector<int> res;
	res.reserve(120);
	int l = 0;
	while ((int64_t(1) << l) <= k) {
		l++;
	}

	int64_t v = 1;
	for (int z = l-2; z >= 0; z--) {
		res.push_back(int(res.size()));
		v *= 2;
		assert((k >> z) - v == 0 || (k >> z) - v == 1);
		if ((k >> z) - v == 1) {
			for (auto& x : res) x++;
			res.push_back(0);
			v++;
		}
	}
	assert(v == k);
	assert(int(res.size()) <= 120);
	return res;
}
