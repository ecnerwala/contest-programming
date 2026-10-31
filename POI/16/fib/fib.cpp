#include<bits/stdc++.h>

std::optional<int> go(std::vector<int> V) {
	if (V == std::vector<int>({0})) return 0;

	int N = int(V.size());
	int lo = N-1;
	int hi = N;

	int lastMod = -1;
	int extra = 0;
	int t = 0;

	while (true) {
		if (hi == N) {
			V[--hi] = V[lo--];
		}

		if (lo >= 0 && V[hi] <= t) {
			if (lo > 0 && V[lo] <= V[hi]) {
				V[--hi] = V[lo--];
				continue;
			} else if ((V[lo] - V[hi]) % 2 == 0) {
				return std::nullopt;
			} else if ((lo == 0 && V[lo] <= V[hi]) || V[lo] == V[hi]+1) {
				V[lo]++;
				hi++;
				continue;
			} else if (V[lo] == V[hi]+3) {
				hi++;
				--V[lo];
				V[lo+1] = V[lo];
				lo++;
				continue;
			} else {
				return std::nullopt;
			}
			assert(false);
		} else {
			assert(hi == N-1);
			if (lo == -1) {
				if (V[hi] <= t) {
					V[hi] = t + 1 + ((t + 1 - V[hi]) & 1); // at least t + 1 without changing parity
				}
			}

			if (V[hi] >= t + 3) {
				t = V[hi] - 3;
			} else if (V[hi] >= t+1) {
				t = V[hi] - 1;
			} else assert(false);

			if (extra) {
				extra -= t - lastMod;
				lastMod = t + 1;
				if(extra < 1) extra = 1;
			}
			extra++;
			if (extra > 3) extra = 3;

			if (V[hi] - t == 1) {
				if (lo == -1) {
					return t + extra;
				}
				hi++;
			} else {
				V[hi] = t+2;
			}

			t++;
		}
	}
	assert(false);
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	std::vector<int> V(N); for (int& v : V) std::cin >> v;
	auto ans = go(V);
	if (ans) {
		std::cout << *ans << '\n';
	} else {
		std::cout << "NIE" << '\n';
	}

	return 0;
}
