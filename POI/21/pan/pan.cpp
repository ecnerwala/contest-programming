#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

// exclusive, inclusive
void gen_ranges(vector<pii> &res, int mi, int ma) {
	int i;
	for(i = 1; ma / i >= i; i++) {
		if(mi / i < ma / i) {
			res.push_back(pii(ma / i, mi / i));
			res.push_back(pii(i, i-1));
		}
	}
	sort(res.begin(), res.end());
}

int solve(int si, int sa, int wi, int wa) {
	si--, wi--; // make these exclusive

	vector<pii> l, r;
	gen_ranges(l, si, sa);
	gen_ranges(r, wi, wa);

	for(vector<pii>::reverse_iterator i = l.rbegin(), j = r.rbegin(); i != l.rend() && j != r.rend(); ) {
		pii a = *i, b = *j;
		if(a.A <= b.B) j++; // a's right endpoint it too small, move b
		else if(b.A <= a.B) i++;
		else {
			return min(a.A, b.A);
		}
	}
	assert(false);
	return 1;
}

int main() {
	int N; scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		int si, sa, wi, wa;
		scanf("%d%d%d%d", &si, &sa, &wi, &wa);
		printf("%d\n", solve(si, sa, wi, wa));
	}
	return 0;
}
