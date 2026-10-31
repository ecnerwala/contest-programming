#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

using ld = long double;

struct prob_t {
	ld pri;
	ld p, t;
	int idx;
	friend bool operator < (const prob_t &a, const prob_t &b) {
		return a.pri > b.pri;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::vector<prob_t> probs(N+1);
	std::vector<std::vector<int>> adj(N+1);
	vector<int> tree_par(N+1);
	probs[0] = {-1, 0, 0, 0};
	for (int i = 1; i <= N; i++) {
		int c; ld p; int d; cin >> c >> p >> d;
		p = 1-p;
		probs[i] = {p / ld(c), p, ld(c), i};
		adj[d].push_back(i);
		tree_par[i] = d;
	}

	std::set<prob_t> pq;
	vector<int> cc_id(N+1);
	for(int i = 0; i <= N; i++){
		cc_id[i] = -1;
	}
	auto get_par = [&](int a) {
		while (cc_id[a] >= 0) {
			if (cc_id[cc_id[a]] >= 0) cc_id[a] = cc_id[cc_id[a]];
			a = cc_id[a];
		}
		return a;
	};
	vector<int> linked_list_child(N+1, -1);
	vector<int> linked_list_bottom(N+1);
	for(int i = 0; i <= N; i++) linked_list_bottom[i] = i;
	auto merge = [&](int a, int b) {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		cc_id[b] = a; // b's par is a
		return true;
	};
	for(int i = 1; i <= N; i++){
		pq.insert(probs[i]);
	}
	while(!pq.empty()){
		auto q = (*pq.begin());
		pq.erase(q);
		int v = q.idx;
		int c = get_par(tree_par[v]);
		linked_list_child[linked_list_bottom[c]] = v;
		linked_list_bottom[c] = linked_list_bottom[v];
		pq.erase(probs[c]);
		merge(c, v);
		if(c > 0){
			probs[c].t += (1 - probs[c].p) * probs[v].t;
			probs[c].p += (1 - probs[c].p) * probs[v].p;
			probs[c].pri = probs[c].p / probs[v].t;
			pq.insert(probs[c]);
		}
	}

	vector<int> ans = {0};
	while((int)ans.size() <= N){
		ans.push_back(linked_list_child[ans.back()]);
		assert(ans.back() != -1);
	}
	assert(int(ans.size()) == N+1);
	for (int i = 1; i <= N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}