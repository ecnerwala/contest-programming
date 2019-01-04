#include<iostream>
#include<algorithm>
#include<cstring>
#include<map>

using namespace std;

typedef pair<int, int> pii;

const int MAXN = 2e5;

int N;
int A[MAXN];

int prv[MAXN];

//pii bit[MAXN][MAXN];
map<pii, pii> bit;

void insert(int i, int j, pii v) {
	i++;
	j++;
	for(int x = i; x <= N; x += x & (-x)) {
		for(int y = j; y <= N; y += y & (-y)) {
			//bit[x][y] = max(bit[x][y], v);
			pii loc(x, y);
			if(bit.count(loc)) {
				bit[loc] = max(bit[loc], v);
			} else {
				bit[loc] = v;
			}
		}
	}
}

pii query(int i, int j) {
	i++;
	j++;
	pii res = pii(0, -1);
	for(int x = i; x > 0; x -= x & (-x)) {
		for(int y = j; y > 0; y -= y & (-y)) {
			//res = max(res, bit[x][y]);
			pii loc(x, y);
			if(bit.count(loc)) {
				res = max(res, bit[loc]);
			}
		}
	}
	return res;
}

int res;
int rem[MAXN];
void go() {
	for(int i = 0; i < N; i++) {
		int dist = i - A[i];
		if(dist >= 0) {
			pii val = query(dist, A[i] - 1);
			prv[i] = val.second;
			insert(dist, A[i], pii(val.first + 1, i));
		}
	}

	res = 0;

	int cur = query(N - 1, N - 1).second;
	while(cur > 0) {
		int numRem = cur - A[cur];
		if(prv[cur] != -1) numRem -= prv[cur] - A[prv[cur]];
		for(int i = 1; i <= numRem; i++) {
			rem[res++] = cur - i;
		}
		cur = prv[cur];
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i], A[i]--;

	go();

	cout << res << '\n';
	for(int i = res - 1; i >= 0; i--) {
		cout << (rem[i]+1) << (i ? ' ' : '\n');
	}
	return 0;
}
