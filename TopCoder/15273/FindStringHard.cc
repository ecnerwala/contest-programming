#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FindStringHard {

	int c2(int a) {
		return a * (a-1) / 2;
	}

	int altCnt(int n) {
		return c2(n/2 + 1) + c2(n - (n/2+1) + 1);
	}
	
	int getK(int n) {
		int k = 1;
		assert(altCnt(k) == 0);
		assert(altCnt(k) <= n);
		while (altCnt(k+1) <= n) {
			k++;
		}
		return k;
	}

	string go(int n) {
		assert(n >= 0);
		vector<int> ks;
		while (n) {
			if (!ks.empty() && ks.back() % 2 == 0) {
				n--;
			}
			int k = getK(n);
			ks.push_back(k);
			n -= altCnt(k);
		}
		for (int i = 0; i + 1 < int(ks.size()); i++) {
			assert(ks[i] > ks[i+1]);
		}
		string s;
		char c = 'a';
		s += 'a';
		for (int i = 0; i < int(ks.size()); i++) {
			if (i % 2 == 1) {
				s += c;
			}
			int k = ks[i];
			for (int i = 0; i < k; i++) {
				s += c;
				c = 'a'^'b'^c;
			}
			c = 'a'^'b'^c;
		}
		return s;
	}

	int cntAntiPal(string s) {
		int res = 0;
		for (int i = 0; i < s.size(); i++) {
			for (int j = i; j < s.size(); j++) {
				bool good = true;
				for (int k = i; k <= j; k++) {
					if (s[k] == s[i+j-k]) {
						good = false;
					}
				}
				res += good;
			}
		}
		return res;
	}
	void test() {
		FindStringHard fsh;
		for (int i = 0; i <= 1000; i++) {
			string res = fsh.go(i);
			assert(res.size() <= 100);
			cerr << i << ' ' << res.size() << ' ' << cntAntiPal(res) << ' ' << res << '\n';
			assert(cntAntiPal(res) == i);
		}
		cerr <<" Tests  passed" << '\n';
	}

public:
	string withAntipalindromicSubstrings(int n) {
		//test();
		return go(n);
	}

};

}  // namespace ecnerwala

using ecnerwala::FindStringHard;

// vim:ft=cpp
