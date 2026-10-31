#include <bits/stdc++.h>

std::mt19937 mt(48);

struct treap_node {
	std::array<treap_node*, 2> c = {nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	std::pair<int, int> key;

	int64_t val;
	int64_t sum;

	void update() {
		sum = val + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0);
	}

	~treap_node() {
		delete c[0];
		delete c[1];
	}
};

std::pair<treap_node*, treap_node*> split(treap_node* a, std::pair<int, int> key) {
	if (!a) return {nullptr, nullptr};
	treap_node *l, *r;
	if (a->key < key) {
		l = a;
		std::tie(l->c[1], r) = split(a->c[1], key);
	} else {
		r = a;
		std::tie(l, r->c[0]) = split(a->c[0], key);
	}
	a->update();
	return {l, r};
}

treap_node* drop_right(treap_node* a, int64_t num) {
	assert(0 <= num && num <= (a ? a->sum : 0));
	if (num == 0) return a;
	assert(a);
	if (num == a->sum) {
		delete a;
		return nullptr;
	}
	int64_t c1sum = (a->c[1] ? a->c[1]->sum : 0);
	if (num <= c1sum) {
		a->c[1] = drop_right(a->c[1], num);
		a->update();
		return a;
	} else {
		num -= c1sum;
		delete std::exchange(a->c[1], nullptr);
		if (num < a->val) {
			a->val -= num;
			a->update();
			return a;
		} else {
			num -= a->val;
			treap_node* na = std::exchange(a->c[0], nullptr);
			delete a;
			return drop_right(na, num);
		}
	}
}

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node *r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

treap_node* meld(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	if (a->pri > b->pri) std::swap(a, b);
	auto [l, r] = split(b, a->key);
	a->c[0] = meld(a->c[0], l);
	a->c[1] = meld(a->c[1], r);
	a->update();
	return a;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> P(N);
	vector<int> deg(N);
	vector<int> H(N);
	vector<int64_t> C(N);
	for (int i = 0; i < N; i++) {
		cin >> P[i] >> H[i] >> C[i]; P[i]--;
		deg[P[i]]++;
	}

	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!deg[i]) q.push_back(i);
	}

	int tiebreaker = 0;
	vector<treap_node*> costs(N, nullptr);

	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		assert(!deg[cur]);
		int par = P[cur];

		treap_node* mid_incr = new treap_node;
		mid_incr->key = {H[cur], --tiebreaker};
		mid_incr->val = C[cur];
		mid_incr->update();

		treap_node* left_incr = new treap_node;
		left_incr->key = {0, --tiebreaker};
		left_incr->val = C[cur];
		left_incr->update();

		auto [l, r] = split(std::exchange(costs[cur], nullptr), mid_incr->key);

		l = merge(left_incr, l);
		l = drop_right(l, C[cur]);
		l = merge(l, mid_incr);
		l = merge(l, r);

		costs[par] = meld(costs[par], l);
		if (!--deg[par]) q.push_back(par);
	}

	int64_t ans = 0;

	for (int st = 0; st < N; st++) {
		if (!deg[st]) continue;
		assert(deg[st] == 1);
		treap_node* n = nullptr;
		int64_t tot_cost = 0;
		map<int, int64_t> savings;
		for (int cur = st; deg[cur]; cur = P[cur]) {
			deg[cur]--;
			tot_cost += C[cur];
			savings[H[cur]] += C[cur];
			n = meld(n, std::exchange(costs[cur], nullptr));
		}

		auto query = [&](int cutoff) -> int64_t {
			auto [l, r] = split(n, {cutoff, tiebreaker-1});
			int64_t res = l ? l->sum : 0;
			n = merge(l, r);
			return res;
		};

		int64_t cur_val = tot_cost + query(1);
		for (auto [cutoff, save] : savings) {
			cur_val = min(cur_val, tot_cost - save + query(cutoff));
		}

		ans += cur_val;

		delete n;
	}

	cout << ans << '\n';

	return 0;
}
