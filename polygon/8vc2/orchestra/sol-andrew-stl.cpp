// runs in n^2 k + n^2 log n
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 4000;
const int MAXR = 4000;
const int MAXC = 4000;
const int MAXK = 20;
int N, R, C, K;

pair<int, int> P[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	scanf("%d %d %d %d", &R, &C, &N, &K);
	for(int i = 0; i < N; i++) scanf("%d %d", &(P[i].first), &(P[i].second));
	sort(P, P + N);
	ll res = 0;
	for(int l = 1; l <= R; l++) {
		list<int> lst;
		lst.push_back(0);
		lst.push_back(C+1);
		map< int, list<int>::iterator > st;
		st[0] = lst.begin();
		st[C + 1] = (++lst.begin());
		int r = l;
		ll val = 0;
		for(int i = 0; i < N; i++) {
			if(P[i].first < l) continue;
			assert(P[i].first >= r);
			res += val * (P[i].first - r);
			r = P[i].first;
			int y = P[i].second;

			list<int>::iterator it = lst.insert(st.lower_bound(y)->second, y);
			st[y] = it;

			if(int(lst.size()) < K + 2) continue;

			list<int>::iterator le = it;

			for(int z = 0; z < K; z++) {
				if(le == lst.begin()) {
					break;
				}
				le--;
			}

			list<int>::iterator re = le;
			for(int z = 0; z < K; z++) {
				re++;
				assert(re != lst.end());
			}

			while(le != it) {
				ll tl = *le, tr = *re;
				le++;
				re++;
				if(re == lst.end()) break;
				val += (*re - tr) * (*le - tl);
			}

		}
		assert(r <= R);
		assert(val >= 0);
		res += val * (R + 1 - r);
	}
	cout << res << '\n';
	return 0;
}
