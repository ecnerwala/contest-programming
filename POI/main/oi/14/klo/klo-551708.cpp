#include<iostream>
#include<algorithm>
#include<cstring>

using namespace std;

typedef pair<int, int> pii;

const int MAXN = 2e5;

int N;
int A[MAXN];

int prv[MAXN];

pii bit[MAXN];

void insert(int i, pii v) {
	i++;
	while(i <= N) {
		bit[i] = max(bit[i], v);
		i += i & (-i);
	}
}

pii query(int i) {
	i++;
	pii res = pii(0, -1);
	while(i) {
		res = max(res, bit[i]);
		i -= i & (-i);
	}
	return res;
}

int res;
int rem[MAXN];
void go() {
	for(int i = 0; i < N; i++) {
		int dist = i - A[i];
		if(dist >= 0) {
			pii val = query(dist);
			prv[i] = val.second;
			insert(dist, pii(val.first + 1, i));
		}
	}

	res = 0;

	int cur = query(N - 1).second;
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
