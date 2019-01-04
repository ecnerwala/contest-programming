#include<bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;

const int MAXN = 6e5;
int N;
pair<int, pair<int, int>> pts[MAXN];

vector<pair<pii, pii>> lines;

// starting at it
void checkOne(multiset<int>::iterator it, multiset<int>::iterator end) {
	if (it == end) { return; }
	int a = *it;
	it++;

	if (it == end) { return; }
	int b = *it;
	it++;

	if (it == end) { return; }
	int c = *it;
	it++;

	if (it == end) { return; }
	int d = *it;
	it++;

	if (b < c) lines.emplace_back(pii(a, b), pii(c, d));
}

int bit[MAXN];
void bitUpdate(int i, int v) {
	assert(1 <= i && i <= N);
	for (; i <= N; i += (i & (-i))) {
		bit[i] = max(bit[i], v);
	}
}
int bitQuery(int i) {
	assert(1 <= i && i <= N);
	int res = 0;
	for (; i > 0; i -= (i & (-i))) {
		res = max(res, bit[i]);
	}
	return res;
}

const int INF = 1e9 + 200;
int go() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> pts[i].first >> pts[i].second.first >> pts[i].second.second;
	}
	pts[N++] = pair<int, pii>(-INF, pii(-INF, INF));
	pts[N++] = pair<int, pii>(-INF, pii(-INF, INF));
	pts[N++] = pair<int, pii>(+INF, pii(-INF, INF));
	pts[N++] = pair<int, pii>(+INF, pii(-INF, INF));
	sort(pts, pts + N);
	int cur = pts[0].first;
	pts[0].first = 1;
	for (int i = 1; i < N; i++) {
		if (pts[i].first == cur) {
			pts[i].first = pts[i-1].first;
		} else {
			cur = pts[i].first;
			pts[i].first = pts[i-1].first+1;
		}
	}

	priority_queue<pair<pair<int, bool>, int>> events;
	for (int i = 0; i < N; i++) {
		int a = pts[i].second.first;
		int b = pts[i].second.second;
		events.emplace(pair<int, bool>(b, 1), pts[i].first);
		events.emplace(pair<int, bool>(a, 0), pts[i].first);
	}
	multiset<int> dividers;
	while (!events.empty()) {
		auto evt = events.top();
		events.pop();
		int x = evt.second;
		multiset<int>::iterator it;
		//cerr << "EVT " << evt.first.first << ' ' << evt.first.second << ' ' << evt.second << '\n';
		if (evt.first.second) {
			// it's an insert
			it = dividers.insert(x);
			checkOne(it, dividers.end());
		} else {
			it = dividers.find(x);
			assert(it != dividers.end());
			it = dividers.erase(it);
		}
		if (it == dividers.begin()) continue;
		it--;
		checkOne(it, dividers.end());
		if (it == dividers.begin()) continue;
		it--;
		checkOne(it, dividers.end());
		if (it == dividers.begin()) continue;
		it--;
		checkOne(it, dividers.end());
	}

	sort(lines.begin(), lines.end());
	memset(bit, 0, sizeof(bit));

	priority_queue<pair<pii, int>, vector<pair<pii, int>>, greater<pair<pii, int>>> updates;

	int res = 0;
	for (auto it : lines) {
		//cerr << it.first.first << ' ' << it.first.second << ' ' << it.second.first << ' ' << it.second.second << '\n';
		while (!updates.empty() && updates.top().first <= it.first) {
			auto upd = updates.top(); updates.pop();
			//cerr << "UPD" << upd.first.first << ' ' << upd.first.second << ' ' << upd.second << '\n';
			bitUpdate(upd.first.second, upd.second);
		}
		int ind = it.first.second;
		int v = bitQuery(ind) + 1;
		res = max(res, v);
		updates.emplace(it.second, v);
	}
	lines.clear();
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N_CASES; cin >> N_CASES;
	for (int case_num = 1; case_num <= N_CASES; case_num ++) {
		cerr << "Case #" << case_num << " START\n";
		auto res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
