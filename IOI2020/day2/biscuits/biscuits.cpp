#include "biscuits.h"

#include <vector>
#include <iostream>
#include <cassert>

long long count_tastiness(long long X, std::vector<long long> A) {
	auto get_A = [&](int i) { return i < int(A.size()) ? A[i] : 0; };
	std::vector<long long> len(121);
	len[0] = 1;
	std::vector<long long> max_elt(121); // floor of the max element
	max_elt[0] = 0;
	for (int i = 0; i < 120; i++) {
		max_elt[i+1] = (get_A(i) + max_elt[i]) / 2;
		len[i+1] = 0;

		long long cutoff = 0;
		for (int j = i; true; j--) {
			assert(cutoff >= 0);
			cutoff *= 2;
			cutoff -= get_A(j);
			if (max_elt[j] >= cutoff + X) {
				len[i+1] += len[j];
				cutoff += X;
			}
			if (cutoff <= 0) {
				len[i+1] += len[j];
				break;
			}
		}
	}
	return len.back();
}

// poss_carry = [0]
// for i in range(120):
//   poss_carry = [a[i] + v for v in poss_carry]
//   poss_carry = [v - x for v in poss_carry if v >= x] + poss_carry
//   poss_carry = [v / 2 for v in poss_carry]
//   assert max(poss_carry) - min(poss_carry) < x
//   mem[i+1] = poss_carry
// return len(poss_carry)

// We can move the threshold step to the end:
// poss_carry = [0]
// for i in range(120):
//   poss_carry = [v - x for v in poss_carry] + poss_carry
//   poss_carry = [a[i] + v for v in poss_carry]
//   poss_carry = [v / 2 for v in poss_carry]
//   poss_carry = [v for v in poss_carry if v >= 0]
//   assert max(poss_carry) - min(poss_carry) < x
//   mem[i+1] = poss_carry
// return len(poss_carry)
