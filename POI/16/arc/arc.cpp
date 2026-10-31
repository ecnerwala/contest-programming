#include "arc.h"

#include <vector>

int main() {
	int K = inicjuj()+1;
	std::vector<int> pref; pref.reserve(K);
	std::vector<int> suff(K);
	int lo = 0, hi = K-1;
	for (int z = 0; z < K-1; z++) {
		suff[z] = wczytaj();
	}

	while (true) {
		int v = wczytaj();
		if (v == 0) break;

		suff[hi] = v;
		hi++; if (hi == K) hi = 0;
		while (pref.empty() || (int(pref.size()) != K && pref.back() >= suff[lo])) {
			pref.push_back(suff[lo]);
			lo++; if (lo == K) lo = 0;
		}
		pref.pop_back();
	}

	for (int v : pref) {
		wypisz(v);
	}
	for (int i = lo; i != hi; ) {
		wypisz(suff[i]);
		i++; if (i == K) i = 0;
	}
	return 0;
}
