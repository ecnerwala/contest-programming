#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int MAXN = 2.1e5;
int random[MAXN * 2];
int D[MAXN];
int T[MAXN];

int bit[MAXN];

class SchedulingWoes {

public:

	int study(int N, int seed, vector <int> Dprefix, int maxD, vector <int> Tprefix, int factor) {
		random[0] = seed;
		for (int i = 1; i <= 2*N-1; i++) {
			random[i] = (1ll * random[i-1] * 1103515245 + 12345) % (1ll<<31);
		}

		for(int i = 0; i <= int(Dprefix.size())-1; i++) {
			D[i] = Dprefix[i];
			T[i] = Tprefix[i];
		}

		for (int i = int(Dprefix.size()); i <= N-1; i++) {
			D[i] = 1 + (random[2*i] % maxD);
			int maxT = max(1, D[i] / factor);
			T[i] = 1 + (random[2*i+1] % maxT);
		}

		vector<int> days(N+1);
		for (int i = 0; i < N; i++) { days[i] = D[i]; }
		days[N] = 0;
		sort(days.begin(), days.end());
		days.resize(unique(days.begin(), days.end()) - days.begin());

		assert(days[0] == 0);

		int ND = int(days.size()) - 1;
		for (int i = 1; i <= ND; i++) { bit[i] = 0; }

		map<int, int> amt;
		for (int i = 1; i <= ND; i++) {
			amt[i] += days[i] - days[i-1];
			for (int a = i; a <= ND; a += a & -a) {
				bit[a] += days[i] - days[i-1];
			}
		}

		vector<pair<int, int>> cnds(N);
		for (int i = 0; i < N; i++) { cnds[i] = {T[i], D[i]}; }
		sort(cnds.begin(), cnds.end());

		int ans = 0;
		for (auto it : cnds) {
			int d = int(lower_bound(days.begin(), days.end(), it.second) - days.begin());
			int amtLeft = 0;
			for (int a = d; a > 0; a -= a & -a) {
				amtLeft += bit[a];
			}

			if (amtLeft < it.first) {
				continue;
			}

			int t = it.first;
			while (t > 0) {
				auto ait = amt.upper_bound(d);
				assert(ait != amt.begin());
				--ait;
				int curI = ait->first;
				int v = min(t, ait->second);
				t -= v;
				if (v == ait->second) {
					amt.erase(ait);
				} else {
					amt[curI] -= v;
				}

				for (int a = curI; a <= ND; a += a & (-a)) {
					bit[a] -= v;
				}
			}
			ans++;
		}
		return ans;
	}

};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::SchedulingWoes;

// vim:ft=cpp
