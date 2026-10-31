#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CardDrawGame {

public:

	double winChance(int target, vector <int> count) {
		int L = int(count.size());

		int totCnt = 0;
		for (int v : count) totCnt += v;

		double ans = 0;
		for (int i = 0; i < L; i++) {
			if (!count[i]) continue;
			double probI = double(count[i]) / totCnt;
			count[i] --;
			totCnt --;
			for (int j = i; j < L; j++) {
				if (!count[j]) continue;
				double probJ = probI * count[j] / totCnt;
				count[j] --;
				totCnt --;
				for (int k = j; k < L; k++) {
					if (!count[k]) continue;
					double probK = probJ * count[k] / totCnt;
					count[k] --;
					totCnt --;

					// accounting for reordering
					double stateProb = probK * (i == k ? 1 : ((i == j || j == k) ? 3 : 6));

					double winProb = 0;
					if (i+j+k == target) {
						winProb = 1;
					}
					auto check2 = [&](int a, int b) {
						int v = target - a - b;
						if (0 <= v && v < L) winProb = max(winProb, double(count[v]) / totCnt);
					};
					check2(i, j);
					check2(i, k);
					check2(j, k);

					double winIProb = 0;
					double winJProb = 0;
					double winKProb = 0;

					double win3Prob = 0;
					// pick some subset to keep
					for (int a = 0; a < L; a++) {
						if (!count[a]) continue;
						double probA = double(count[a]) / totCnt;
						count[a]--;
						totCnt --;
						for (int b = a; b < L; b++) {
							if (!count[b]) continue;
							double probB = probA * count[b] / totCnt;
							count[b]--;
							totCnt --;

							{
								double prob2 = probB * (a == b ? 1 : 2);
								if (a+b+i==target) winIProb += prob2;
								if (a+b+j==target) winJProb += prob2;
								if (a+b+k==target) winKProb += prob2;
							}

							{
								int c = target - a - b;
								if (c < b || c >= L || !count[c]) goto doneC;
								double probC = probB * count[c] / totCnt;
								count[c]--;
								totCnt --;

								win3Prob += probC * (a == c ? 1 : ((a == b || b == c) ? 3 : 6));

								count[c]++;
								totCnt ++;
							}
doneC:
							count[b]++;
							totCnt ++;
						}
						count[a]++;
						totCnt ++;
					}

					winProb = max(winProb, winIProb);
					winProb = max(winProb, winJProb);
					winProb = max(winProb, winKProb);
					winProb = max(winProb, win3Prob);

					ans += stateProb * winProb;

					count[k] ++;
					totCnt ++;
				}
				count[j]++;
				totCnt ++;
			}
			count[i]++;
			totCnt ++;
		}

		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << CardDrawGame().winChance(30, vector<int>(20, 100)) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::CardDrawGame;

// vim:ft=cpp
