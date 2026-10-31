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

std::mt19937_64 mt(48);
struct prob_t {
	double pri;
	double p, t;
	int idx;
	friend bool operator < (const prob_t &a, const prob_t &b) {
		return a.pri < b.pri;
	}

	std::array<prob_t*, 2> c = {nullptr, nullptr};
};

void meld(prob_t*& a, prob_t* b) {
	if (!b) return;
	if (!a) {
		std::swap(a, b);
		return;
	}
	if (a->pri < b->pri) {
		std::swap(a, b);
	}
	meld(a->c[mt() & 1], b);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::vector<prob_t> probs(N+1);
	std::vector<std::vector<int>> adj(N+1);
	probs[0] = {-1, 0, 0, 0};
	for (int i = 1; i <= N; i++) {
		int c; double p; int d; cin >> c >> p >> d;
		p = 1-p;
		probs[i] = {p / double(c), p, double(c), i};
		adj[d].push_back(i);
	}

	std::vector<std::vector<int>> ch(N+1);
	std::y_combinator([&](auto self, int cur, int prv) -> prob_t* {
		prob_t* pq = nullptr;
		for (int nxt : adj[cur]) {
			assert(nxt != prv);
			auto pq2 = self(nxt, cur);
			meld(pq, pq2);
		}

		prob_t* p = &probs[cur];
		while (pq && (cur == 0 || p->pri <= pq->pri)) {
			ch[cur].push_back(pq->idx);
			if (cur != 0) {
				p->t += (1 - p->p) * pq->t;
				p->p += (1 - p->p) * pq->p;
				p->pri = p->p / p->t;
			}
			auto a = pq->c[0];
			pq = pq->c[1];
			meld(pq, a);
		}
		p->c[mt() & 1] = pq;
		return p;
	})(0, -1);

	std::vector<int> ans; ans.reserve(N+1);
	std::y_combinator([&](auto self, int cur) -> void {
		ans.push_back(cur);
		for (int nxt : ch[cur]) {
			self(nxt);
		}
	})(0);
	assert(int(ans.size()) == N+1);
	for (int i = 1; i <= N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
