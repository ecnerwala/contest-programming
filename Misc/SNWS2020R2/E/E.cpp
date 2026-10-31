#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int N, Q;

unordered_map<int, ll> waitTime;
ll getWait(int r) {
	assert(0 < r && r <= N);
	auto it = waitTime.find(r);
	if (it == waitTime.end()) { return r; }
	else return it->second;
}

unordered_map<int, ll> seg;
ll getSeg(int a) {
	assert(a > 0);
	if (a > N) return 0;

	auto it = seg.find(a);
	if (it != seg.end()) {
		return it->second;
	}

	if (2*a > N) {
		return seg[a] = getWait(a);
	} else if ((N >> (__builtin_clz(2*a) - __builtin_clz(N))) == 2*a) {
		return seg[a] = getWait(a) + getSeg(2*a);
	} else {
		return seg[a] = getWait(a) + getSeg(2*a+1);
	}
}

void updateSeg(int a) {
	assert(a <= N);
	seg[a] = getWait(a) + max(getSeg(2*a), getSeg(2*a+1));
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> Q;

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int r; ll n; cin >> r >> n;
			waitTime[r] = n;
			for (int a = r; a; a /= 2) updateSeg(a);

		} else if (op == 2) {
			int r; cin >> r;
			ll w1 = getSeg(2*r);
			ll w2 = getSeg(2*r+1);
			ll pathUp = 0;
			ll curPath = 0;
			for (int a = r; a >= 2; a /= 2) {
				curPath += getWait(a/2);
				pathUp = max(pathUp, curPath + getSeg(a^1));
			}
			cout << getWait(r) + w1 + w2 + pathUp - min({w1, w2, pathUp}) << '\n';
		} else assert(false);
	}

	return 0;
}
