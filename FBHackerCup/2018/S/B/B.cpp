#include<bits/stdc++.h>
using namespace std;

using ll = long long;

using point = ll;

const point LEFT = 0ll;
const point RIGHT = -1ll;

point readPoint() {
	ll a; cin >> a; a++;
	return a;
}

// true for left, false for right
point chompTo(point p, bool b) {
	assert(p > 0);
	if (p == 1) {
		return b ? LEFT : RIGHT;
	}
	assert(p >= 2);
	if ((p & 1) == b) {
		return p / 2;
	} else {
		return chompTo(p / 2, b);
	}
}

unordered_map<point, ll> bfs(point a) {
	unordered_map<point, ll> dist;
	queue<point> q;
	dist[a] = 0;
	q.push(a);
	dist[chompTo(a, 0)] = 0;
	q.push(chompTo(a, 0));
	dist[chompTo(a, 1)] = 0;
	q.push(chompTo(a, 1));
	while (!q.empty()) {
		point cur = q.front(); q.pop();
		if (cur <= 0) {
			assert(cur == LEFT || cur == RIGHT);
			point nxt = LEFT ^ RIGHT ^ cur;
			if (dist.count(nxt)) continue;
			dist[nxt] = dist[cur] + 1;
			q.push(nxt);
			continue;
		}
		for (int z = 0; z < 2; z++) {
			point nxt = chompTo(cur, z);
			if (dist.count(nxt)) continue;
			dist[nxt] = dist[cur] + 1;
			q.push(nxt);
		}
	}
	return dist;
}

ll go() {
	point a = readPoint();
	point b = readPoint();
	auto apar = bfs(a);
	auto bpar = bfs(b);
	assert(apar.count(LEFT));
	assert(bpar.count(LEFT));
	ll res = apar[LEFT] + bpar[LEFT];
	for (auto it : apar) {
		if (bpar.count(it.first)) {
			res = min(res, it.second + bpar[it.first]);
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N_CASES; cin >> N_CASES;
	for (int case_num = 1; case_num <= N_CASES; case_num ++) {
		auto res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
