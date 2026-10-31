#include <bits/stdc++.h>
#include "ckuclib.h"

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int N = dajn();
	const int E = daje();

	vector<array<vector<int>, 2>> dps;
	vector<int> idx;
	while (idx.empty() || idx.front() < N) {
		int z = 0;
		while (z < int(idx.size()) && (idx[z] == 1 || !(idx[z] & 1))) z++;
		for (; z >= 0; z--) {
			if (z == int(idx.size())) {
				dps.push_back({vector<int>({0}), vector<int>({jedno(1)})});
				idx.push_back(1);
				continue;
			}
			idx[z]++;

			std::swap(dps[z][0], dps[z][1]);

			int v1 = jedno(idx[z]);
			int v2 = dwa(idx[z]);
			int vh = polowa(idx[z]);

			for (auto& v : dps[z][1]) v += v2;
			for (int e = 0; e < int(dps[z+1][1].size()); e++) {
				if (e+1 == int(dps[z][1].size())) {
					dps[z][1].push_back(dps[z+1][1][e] + vh);
				} else {
					setmin(dps[z][1][e+1], dps[z+1][1][e] + vh);
				}
			}
			assert(int(dps[z][1].size()) >= int(dps[z][0].size()));
			for (int e = 0; e < int(dps[z][0].size()); e++) {
				setmin(dps[z][1][max(0, e-1)], dps[z][0][e] + v1);
			}
		}
	}

	int ans = *min_element(dps[0][1].begin(), dps[0][1].begin() + min(E+1, int(dps[0][1].size())));
	odpowiedz(ans);

	return 0;
}
