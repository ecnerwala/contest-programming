#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FixedNumberOfDigits {

public:

	long long sum(int start_, int step_, long long numberOfDigits) {
		long long start = start_;
		long long step = step_;

		while (true) {
			long long curLen = int(to_string(start).size());
			if (curLen >= numberOfDigits) {
				return stoll(to_string(start).substr(0, numberOfDigits));
			}
			long long cnt = min((stoll(string(curLen, '9')) - start) / step + 1, (numberOfDigits-1) / curLen);
			start += step * cnt;
			numberOfDigits -= cnt * curLen;
		}
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FixedNumberOfDigits;

// vim:ft=cpp
