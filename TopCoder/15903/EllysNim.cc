#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EllysNim {

public:

	int first_bit(int64_t v) {
		assert(v);
		return 63 - __builtin_clzll(v);
	}
	int64_t p2(int b) {
		return int64_t(1) << b;
	}

	long long getMin(vector <int> A_) {
		using i64 = int64_t;
		const vector<i64> A(A_.begin(), A_.end());
		int N = int(A.size());
		i64 bestAns = 1e18;
		i64 initXor = 0;
		for (i64 v : A) {
			initXor ^= v;
		}
		if (!initXor) return 0;
		for (int b0 = first_bit(initXor); b0 <= 33; b0++) {
			i64 ans = 0;
			i64 curXor = initXor;
			vector<i64> curA = A;
			bool first = true;
			while (curXor) {
				int b = first ? b0 : first_bit(curXor); first = false;
				bool found = false;
				i64 best = -1;
				int bestInd = -1;
				for (int i = 0; i < N; i++) {
					i64 v = curA[i];
					if (v & p2(b)) continue;
					found = true;
					v &= (p2(b)-1);
					if (v > best) {
						best = v;
						bestInd = i;
					}
				}
				if (!found) goto oops;

				ans -= curA[bestInd];
				curXor ^= curA[bestInd];
				curA[bestInd] ^= p2(b) ^ best;
				curXor ^= curA[bestInd];
				ans += curA[bestInd];
			}
			bestAns = min(bestAns, ans);
oops:;
		}
		return bestAns;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << EllysNim().getMin({3,5,7}) << '\n';
	cerr << EllysNim().getMin({786071738, 716807484, 788183376, 679065628, 602303196, 547670373, 646844377, 653347044, 596095298, 678219163, 564821004, 675666879, 724015473, 738446025, 597846433, 766119413, 799744302, 706038837, 622504742, 700625136, 698434461, 572025741, 651000276, 595713571, 762196741, 792284796, 752478640, 789451123, 714896642, 798892861, 660369107, 615569513, 796340162, 623837194, 557364426, 798170340, 575251216, 657479943, 731083629, 707957765, 650116215, 766327288, 616897825, 686755468, 545231809, 600044690, 591519522, 600066835, 646035633, 618081547, 743195309, 612423912, 635913791, 687876825, 666087596, 733734087, 678094055, 582345553, 694372917, 541216673, 549892706, 695019625, 781636195, 574890495, 711145345, 595591573, 779001840, 592100151, 581012988, 592512038, 803584049, 731532093, 538083491, 610996093, 551315612, 641385727, 640970829, 781769195, 723756061, 713172941, 774100317, 580199224, 645759544, 622855763, 599222929, 572322389, 666636878, 612227529, 554735470, 616807937, 734220239, 716834989, 691467061, 621600516, 713577918, 791473729, 678470289, 580551706, 640126690}) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::EllysNim;

// vim:ft=cpp
