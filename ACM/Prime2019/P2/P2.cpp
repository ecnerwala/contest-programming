#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
char S[MAXN];

int suffArr[MAXN];
int suffVal[MAXN];
int suffLCP[MAXN];

const int SSZ = 1 << 17;
int lcpSeg[2*SSZ];

int bucketSize[MAXN];
int suffArr2[MAXN];
int suffVal2[MAXN];

void buildSuffArr() {
	int M = 257;
	for (int i = 0; i < N; i++) {
		suffVal[i] = S[i] + 1;
	}
	suffArr[0] = N;
	suffVal[N] = 0;

	for (int l = 1; l <= N; l *= 2) {
		for (int j = 0; j <= M; j++) bucketSize[j] = 0;
		for (int i = 0; i <= N; i++) bucketSize[suffVal[min(i+l,N)]]++;
		for (int j = 0; j < M; j++) bucketSize[j+1] += bucketSize[j];

		// doesn't matter what order
		for (int i = 0; i <= N; i++) suffArr2[--bucketSize[suffVal[min(i+l,N)]]] = i;

		for (int j = 0; j <= M; j++) bucketSize[j] = 0;
		for (int i = 0; i <= N; i++) bucketSize[suffVal[i]]++;
		for (int j = 0; j < M; j++) bucketSize[j+1] += bucketSize[j];

		for (int i = N; i >= 0; i--) suffArr[--bucketSize[suffVal[suffArr2[i]]]] = suffArr2[i];

		assert(suffArr[0] == N);

		suffVal2[suffArr[0]] = M = 0;
		for (int i = 1; i <= N; i++) suffVal2[suffArr[i]] = (M += (suffVal[suffArr[i]] != suffVal[suffArr[i-1]] || suffVal[min(suffArr[i]+l, N)] != suffVal[min(suffArr[i-1]+l, N)]));

		for (int i = 0; i <= N; i++) suffVal[i] = suffVal2[i];
	}
	assert(M == N);

	suffLCP[0] = suffLCP[N+1] = 0;
	for (int i = 0, l = 0; i < N; i++) {
		int j = suffArr[suffVal[i]-1];
		while (S[i+l]==S[j+l]) ++l;
		suffLCP[suffVal[i]] = l;
		if (l) --l;
	}

	for (int i = 0; i <= N+1; i++) {
		lcpSeg[SSZ + i] = suffLCP[i];
	}
	for (int i = SSZ-1; i; --i) {
		lcpSeg[i] = min(lcpSeg[2*i], lcpSeg[2*i+1]);
	}
}

int getLCP(int l, int r) {
	assert(l != r);
	if (l > r) swap(l, r);
	assert(l < r);
	int res = int(1e9);
	for (int a = SSZ+l+1, b = SSZ+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			res = min(res, lcpSeg[a++]);
		}
		if (b & 1) {
			res = min(res, lcpSeg[--b]);
		}
	}
	return res;
}

pair<int, int> getLCPRange(int x, int len) {
	assert(len >= 0);
	assert(0 <= x && x <= N);
	if (len == 0) {
		return pair<int, int>(0, N);
	}
	pair<int, int> ans;
	for (int b = SSZ+x+1; b; b /= 2) {
		if (b & 1) {
			if (lcpSeg[b-1] >= len) {
				b--;
			} else {
				int a = b;
				while (a < SSZ) {
					a *= 2;
					if (lcpSeg[a-1] >= len) {
						--a;
					}
				}
				ans.first = a-SSZ-1;
				goto doneLo;
			}
		}
	}
	assert(false);
doneLo:
	for (int a = SSZ+x+1; a; a /= 2) {
		if (a & 1) {
			if (lcpSeg[a] >= len) {
				a++;
			} else {
				int b = a;
				while (b < SSZ) {
					b *= 2;
					if (lcpSeg[b] >= len) {
						++b;
					}
				}
				ans.second = b-SSZ-1;
				goto doneHi;
			}
		}
	}
	assert(false);
doneHi:
	return ans;
}

int aliveSeg[2*SSZ];
void initAlive() {
	for (int i = 1; i < 2 * SSZ; i++) {
		aliveSeg[i] = N+1;
	}
}
void updateAlive(int x, int v) {
	for (int a = SSZ+x; a; a /= 2) {
		aliveSeg[a] = v;
	}
}
int queryAlive(int l, int r) {
	int ans = N+1;
	for (int a = SSZ+l, b = SSZ+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) ans = min(ans, aliveSeg[a++]);
		if (b & 1) ans = min(ans, aliveSeg[--b]);
	}
	return ans;
}

uint64_t c2(ll v) {
	return (v & 1) ? (v+1)/2*v : v/2*(v+1);
}

struct {
	uint64_t s0 = 0, s1 = 0, s2 = 0;
} cntSeg[SSZ*2];

uint64_t cntArr[SSZ*2];
void updateSubSuff(int a, int v) {
	for (int i = SSZ+a; i; i /= 2) {
		cntSeg[i].s0 += v;
		cntSeg[i].s1 += uint64_t(a) * v;
		cntSeg[i].s2 += c2(a) * v;
	}
}
void updateSub(int l, int r, int v) {
	assert(l <= r);
	updateSubSuff(l, +v);
	updateSubSuff(r, -v);
}
uint64_t querySubPref(int b) {
	uint64_t s0 = 0, s1 = 0, s2 = 0;
	for (int r = SSZ+b; r > 1; r /= 2) {
		if (r & 1) {
			--r;
			s0 += cntSeg[r].s0;
			s1 += cntSeg[r].s1;
			s2 += cntSeg[r].s2;
		}
	}
	return s0 * c2(b) - s1 * uint64_t(b+1) + s2;
}

uint64_t querySub(int l, int r) {
	assert(l <= r);
	return querySubPref(r) - querySubPref(l);
}

int minLen[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S;
	N = int(strlen(S));

	buildSuffArr();
	//for (int i = 0; i <= N; i++) { cerr << suffArr[i] << ' ' << suffLCP[i] << ' ' << (S+suffArr[i]) << '\n'; }

	set<int> suffArrLocs;
	suffArrLocs.insert(-1);
	suffArrLocs.insert(N+1);
	initAlive();

	ll totSub = 0;
	uint64_t numBad = 0;

	for (int i = N-1; i >= 0; i--) {
		int x = suffVal[i];
		auto it = suffArrLocs.lower_bound(x);
		int rx = *it;
		--it;
		int lx = *it;

		int lcp = max(getLCP(x, lx), getLCP(x, rx));
		//cerr << i << ' ' << x << ' ' << lx << ' ' << rx << ' ' << lcp << '\n';

		for (int l = lcp; l > 0; ) {
			pair<int, int> rng = getLCPRange(x, l);
			int mostRecent = queryAlive(rng.first, rng.second);
			updateSub(mostRecent + minLen[mostRecent], mostRecent + l, -1);
			swap(l, minLen[mostRecent]);
		}

		minLen[i] = 0;
		updateSub(i, N, +1);

		totSub += N-i-lcp;
		numBad += querySub(i+lcp, N);

		suffArrLocs.insert(x);
		updateAlive(x, i);
	}

	uint64_t ans = c2(totSub) - numBad;

	cout << ans << '\n';
	return 0;
}
