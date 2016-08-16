#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
struct frac {
	ll p, q;
	frac(ll num = 0, ll den = 1) : p(num), q(den) { }
};

bool operator == (const frac &a, const frac &b) {
	return a.p * b.q == b.p * a.q;
}

frac operator - (const frac &a, const frac &b) {
	return frac(a.p * b.q - b.p * a.q, a.q * b.q);
}

// checks if a is inside b
bool check_inside(const vector<int> &a, const vector<int> &b) {
	unordered_map<int, int> cnt;
	for(int i : b) cnt[i] ++;
	for(int i : a) if(!(cnt[i] --)) return false;
	return true;
}

frac simple_skewness(vector<int> &a) {
	ll tot_sum = 0;
	for(int i : a) tot_sum += i;
	sort(a.begin(), a.end());
	frac mean (tot_sum, int(a.size()));
	frac median;
	if(a.size() % 2 == 0) {
		median = frac(a[int(a.size()) / 2 - 1] + a[int(a.size()) / 2], 2);
	} else {
		median = frac(a[(int(a.size()) - 1) / 2], 1);
	}
	return mean - median;
}

int main(int argc, char * argv[]) {
	registerTestlibCmd(argc, argv);

	int N = inf.readInt();
	vector<int> X(N);
	for(int i = 0; i < N; i++) {
		X[i] = inf.readInt();
	}

	int ans_size = ans.readInt();
	quitif(ans_size <= 0, _fail, "Answer should be a non-empty subset");

	vector<int> ans_vals(ans_size);
	for(int i = 0; i < ans_size; i++) {
		ans_vals[i] = ans.readInt();
	}
	int extraInAnsCount = 0;
	while (!ans.seekEof()) {
		ans.readLong();
		extraInAnsCount++;
	}
	quitif(extraInAnsCount > 0, _fail, "Answer contains sequence of length = %d, but length given is %d elements",
			ans_size + extraInAnsCount, ans_size);

	quitif(!check_inside(ans_vals, X), _fail, "Answer is not a subset of input list");


	int ouf_size = ouf.readInt();
	quitif(ouf_size <= 0, _wa, "Output should be a non-empty subset");

	vector<int> ouf_vals(ouf_size);

	for(int i = 0; i < ouf_size; i++) {
		ouf_vals[i] = ouf.readInt();
	}
	int extraInOufCount = 0;
	while (!ouf.seekEof()) {
		ouf.readLong();
		extraInOufCount++;
	}
	quitif(extraInOufCount > 0, _wa, "Output contains sequence of length = %d, but length given is %d elements",
			ouf_size + extraInOufCount, ouf_size);

	quitif(!check_inside(ouf_vals, X), _wa, "Output is not a subset of input list");


	frac ans_skew = simple_skewness(ans_vals);
	frac ouf_skew = simple_skewness(ouf_vals);
	if (!(ans_skew == ouf_skew)) {
		quitf(_wa, "expected simple skewness %lld/%lld, found simple skewness %lld/%lld",
				ans_skew.p, ans_skew.q, ouf_skew.p, ouf_skew.p);
	}
	quitf(_ok, "simple skewness is %lld/%lld", ans_skew.p, ans_skew.q);
}
