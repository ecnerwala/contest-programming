#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class LCMGCD {

	int cnt[3][3];

	bool go() {
		if(cnt[1][1] >= 2) {
			return true;
		} else if(cnt[1][1] == 1) {
			if(cnt[0][0] || cnt[1][0] || cnt[0][1] || cnt[1][2] || cnt[2][1] || cnt[2][2]) return true;
			if(cnt[0][2] && cnt[2][0]) return true;
			if(cnt[0][2] + cnt[2][0] == 0) return true;
			return false;
		} else {
			if(cnt[0][1] && cnt[1][0]) return true;
			if(cnt[2][1] && cnt[1][2]) return true;
			if(cnt[0][1] && cnt[1][2] && cnt[2][0]) return true;
			if(cnt[2][1] && cnt[1][0] && cnt[0][2]) return true;
			return false;
		}
		return false;
	}

public:
	string isPossible(vector <int> x, int t) {
		int ta = 0, tb = 0;
		while(t % 2 == 0) t /= 2, ta++;
		while(t % 3 == 0) t /= 3, tb++;
		assert(t == 1);
		memset(cnt,0,sizeof(cnt));
		for(int v : x) {
			int va = 0, vb = 0;
			while(v % 2 == 0) v /= 2, va++;
			while(v % 3 == 0) v /= 3, vb++;
			assert(v == 1);

			if(va < ta) va = 0;
			else if(va == ta) va = 1;
			else if(va > ta) va = 2;

			if(vb < tb) vb = 0;
			else if(vb == tb) vb = 1;
			else if(vb > tb) vb = 2;

			//cerr << va << ' ' << vb << '\n';

			cnt[va][vb] ++;
		}

		return go() ? string("Possible") : string("Impossible");
	}
};

// vim:ft=cpp
