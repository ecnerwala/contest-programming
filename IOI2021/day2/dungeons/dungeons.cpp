#include "dungeons.h"

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

namespace ecnerwala {
const int64_t INF = 1e18;
struct node_t {
	int s, p, w, l;
};
std::vector<node_t> nodes;
struct jump_t {
	int dest;
	int64_t tot;
	int64_t cap;
};
const int LG = 25;
std::array<std::vector<jump_t>, LG> jumps;
};

void init(int N, std::vector<int> S_, std::vector<int> P_, std::vector<int> W_, std::vector<int> L_) {
	using namespace ecnerwala;
	nodes.resize(N);
	for (int i = 0; i < N; i++) {
		nodes[i] = {S_[i], P_[i], W_[i], L_[i]};
	}
	auto merge = [](jump_t a, jump_t b) -> jump_t {
		return jump_t{b.dest, a.tot + b.tot, std::min(a.cap, b.cap - a.tot)};
	};
	auto node_jump = [](int cur, int baseline) -> jump_t {
		if (nodes[cur].s <= baseline) {
			return jump_t{nodes[cur].w, nodes[cur].s, INF};
		} else {
			return jump_t{nodes[cur].l, nodes[cur].p, nodes[cur].s};
		}
	};
	for (int l = 0; l < LG; l++) {
		auto& jump = jumps[l];
		jump.resize(N);
		// Funny thing: we can initialize jumps to the 1-edge jump, and then consider them updated when we set jump_len >= 0
		for (int i = 0; i < N; i++) {
			jump[i] = node_jump(i, 1 << l);
		}
		std::vector<int> jump_len(N+1, -1); // this is for bookkeeping our skew-binary representation
		jump_len[N] = 0;
		auto solve = std::y_combinator([&](auto self, int cur) -> void {
			if (jump_len[cur] >= 0) return;
			if (jump_len[cur] == -2) {
				// we're a cycle
				jump_len[cur] = 0;
				while (jump[cur].dest != cur) {
					assert(jump_len[jump[cur].dest] == -2);
					jump[cur] = merge(jump[cur], jump[jump[cur].dest]);
				}
				assert(jump[cur].dest == cur);
				return;
			}
			jump_len[cur] = -2;
			int nxt = jump[cur].dest;
			self(nxt);
			if (jump_len[cur] == 0) {
				// we're a cycle we found later
				return;
			}
			if (jump_len[nxt] > 0 && jump_len[jump[nxt].dest] == jump_len[nxt]) {
				jump_len[cur] = 1 + 2 * jump_len[nxt];
				jump[cur] = merge(jump[cur], jump[jump[cur].dest]);
				jump[cur] = merge(jump[cur], jump[jump[cur].dest]);
			} else {
				jump_len[cur] = 1;
				// no-op
			}
			assert(jump_len[cur] > 0);
		});
		for (int i = 0; i < N; i++) {
			solve(i);
		}
	}
}

long long simulate(int X, int Z_) {
	using namespace ecnerwala;
	int64_t Z = Z_;
	while (X != int(nodes.size())) {
		assert(Z >= 1);
		int k = std::min<int>(LG-1, 8 * sizeof(Z) - 1 - __builtin_clzll(Z));
		jump_t j = jumps[k][X];
		if (Z < j.cap) {
			if (j.dest == X) {
				// let's go for as many cycles as necessary
				Z += (j.cap - 1 - Z) / j.tot * j.tot;
			}
			Z += j.tot;
			X = j.dest;
		} else {
			// just walk forwards by 1
			node_t n = nodes[X];
			if (Z >= n.s) {
				Z += n.s;
				X = n.w;
			} else {
				Z += n.p;
				X = n.l;
			}
		}
	}
	return Z;
}
