#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>

using namespace std;

typedef long long ll;

const int MAXD = 1500, MAXS = 1500;
const int MAXN = 2e4 + 20;

int D, S;
int N;

int maxVal[MAXS * 2];
int addVal[MAXS * 2];

//[a, b)
int insert(int a, int b, int v, int ind = 0, int l = 0, int r = 0) {
	if(ind == 0) {
		return insert(a, b, v, 1, 0, S);
	}
	if(b - a == r - l) {
		addVal[ind] += v;
		maxVal[ind] += v;
	} else {
		int m = (l + r) / 2;
		if(a < m) {
			insert(a, min(b, m), v, 2 * ind, l, m);
		}
		if(b > m) {
			insert(max(a, m), b, v, 2 * ind + 1, m, r);
		}
		maxVal[ind] = addVal[ind] + max(maxVal[2 * ind], maxVal[2 * ind + 1]);
	}
	return maxVal[ind];
}

#define ITER(i, v) for(__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++) 
vector<pair<pair<int, int>, int> > ins[MAXD], rem[MAXD];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> D >> S >> N;
	for(int i = 0; i < N; i++) {
		int d, s, w, x, y;
		cin >> d >> s >> w >> x >> y;
		ins[x].push_back(make_pair(make_pair(y, y + s), w));
		rem[x + d].push_back(make_pair(make_pair(y, y + s), -w));
	}
	int res = 0;
	for(int i = 0; i < D; i++) {
		ITER(it, rem[i]) {
			insert(it->first.first, it->first.second, it->second);
			//insert negative, no need to check;
		}
		ITER(it, ins[i]) {
			int val = insert(it->first.first, it->first.second, it->second);
			if(val > res) res = val;
		}
	}
	cout << res << '\n';
	return 0;
}
