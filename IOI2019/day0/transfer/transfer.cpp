#include "transfer.h"
#include <bits/stdc++.h>
using namespace std;

std::vector<int> get_attachment(std::vector<int> source) {
	int N = int(source.size());
	int v = 0;
	for (int i = 0; i < N; i++) {
		v ^= source[i];
	}
	source.push_back(v);
	vector<int> result({v});
	for (int i = 0; (1 << i) < N+1; i++) {
		result.push_back(0);
		for (int j = 0; j < N+1; j++) {
			if (j & (1 << i)) {
				result.back() ^= source[j];
			}
		}
	}
	return result;
}

std::vector<int> retrieve(std::vector<int> data) {
	int N;
	if (data.size() == 63 + 7) {
		N = 63;
	} else if (data.size() == 255 + 9) {
		N = 255;
	} else assert(false);
	vector<int> source(data.begin(), data.begin()+N);
	vector<int> badAttachment = get_attachment(source);
	vector<int> trueAttachment(data.begin()+N, data.end());
	if (badAttachment[0] == trueAttachment[0]) {
		return source;
	} else {
		int ind = 0;
		for (int i = 0; (1 << i) < N+1; i++) {
			if ((badAttachment[i+1]^1) != trueAttachment[i+1]) {
				ind |= (1 << i);
			}
		}
		if (ind < N) {
			source[ind] ^= 1;
		}
		return source;
	}
}
