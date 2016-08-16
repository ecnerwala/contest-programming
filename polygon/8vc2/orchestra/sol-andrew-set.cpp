// runs in n^2 k log n
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
		multiset<int> st;
		st.insert(0);
		st.insert(C + 1);
		int r = l;
		ll val = 0;
		for(int i = 0; i < N; i++) {
			if(P[i].first < l) continue;
			assert(P[i].first >= r);
			res += val * (P[i].first - r);
			r = P[i].first;
			int y = P[i].second;

			multiset<int>::iterator it = st.insert(y), tmp;

			vector<int> vals;
			vals.push_back(y);

			tmp = it;
			for(int z = 0; z < K; z++) {
				if(tmp == st.begin()) {
					break;
				}
				tmp--;
				vals.push_back(*tmp);
			}
			reverse(vals.begin(), vals.end());

			tmp = it;
			for(int z = 0; z < K; z++) {
				tmp++;
				if(tmp == st.end()) {
					break;
				}
				vals.push_back(*tmp);
			}

			for(int z = 0; z + K + 1 < int(vals.size()); z++) {
				val += ll(vals[z + 1] - vals[z]) * ll(vals[z + K + 1] - vals[z + K]);
				assert(val >= 0);
			}
		}
		assert(r <= R);
		assert(val >= 0);
		res += val * (R + 1 - r);
	}
	cout << res << '\n';
	return 0;
}
