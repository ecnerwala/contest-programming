#include <bits/stdc++.h>

std::mt19937_64 mt(48);

struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937_64::result_type pri = mt();
	std::pair<int, int> v;
	int sz;
	std::array<int64_t, 4> vals;

	void update() {
		sz = 0;
		vals.fill(0);
		if (c[0]) {
			for (int z = 0; z < 4; z++) vals[z] += c[0]->vals[z];
			sz += c[0]->sz;
		}
		vals[sz&3] += v.first;
		sz++;
		if (c[1]) {
			for (int z = 0; z < 4; z++) vals[(sz+z)&3] += c[1]->vals[z];
			sz += c[1]->sz;
		}
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
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

std::tuple<treap_node*, treap_node*, treap_node*> split3(treap_node* a, std::pair<int, int> v) {
	if (!a) return {nullptr, nullptr, nullptr};
	treap_node *x, *m, *y;
	if (v < a->v) {
		y = a;
		std::tie(x, m, y->c[0]) = split3(a->c[0], v);
	} else if (v > a->v) {
		x = a;
		std::tie(x->c[1], m, y) = split3(a->c[1], v);
	} else if (v == a->v) {
		m = a;
		x = a->c[0], y = a->c[1];
		m->c[0] = m->c[1] = nullptr;
	} else assert(false);
	a->update();
	return {x, m, y};
}

std::pair<treap_node*, treap_node*> split(treap_node* a, std::pair<int, int> v) {
	auto [x, m, y] = split3(a, v);
	assert(m == nullptr);
	return {x, y};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q; cin >> N >> Q;

	std::vector<std::array<int, 2>> A(N);
	for (auto& a : A) {
		cin >> a[0] >> a[1];
	}

	std::vector<treap_node> nodes(N);
	treap_node* goods = nullptr;
	treap_node* bads = nullptr;
	int64_t base_cost = 0;

	auto ins_val = [&](int i) -> void {
		base_cost += A[i][1];
		std::pair<int, int> v(A[i][0] - A[i][1], i);

		nodes[i].v = v;
		nodes[i].update();

		treap_node*& t = (A[i][0] <= A[i][1]) ? goods : bads;
		auto [x, y] = split(t, v);
		t = merge(x, merge(&nodes[i], y));
	};

	auto del_val = [&](int i) -> void {
		base_cost -= A[i][1];

		treap_node*& t = (A[i][0] <= A[i][1]) ? goods : bads;
		auto [x, m, y] = split3(t, nodes[i].v);
		assert(m == &nodes[i]);
		t = merge(x, y);
	};

	auto get_ans = [&]() -> int64_t {
		int64_t cost = base_cost;
		int sz = 0;
		if (goods) {
			cost += goods->vals[0] + goods->vals[3];
			sz += goods->sz;
		}
		if (bads) {
			cost += bads->vals[(sz&1) + 0] + bads->vals[(sz&1) + 2];
			sz += bads->sz;
		}
		assert(sz == N);
		return cost;
	};

	for (int i = 0; i < N; i++) {
		ins_val(i);
	}
	cout << get_ans() << '\n';
	for (int q = 0; q < Q; q++) {
		int i; cin >> i; i--;
		del_val(i);
		cin >> A[i][0] >> A[i][1];
		ins_val(i);
		cout << get_ans() << '\n';
	}

	return 0;
}
