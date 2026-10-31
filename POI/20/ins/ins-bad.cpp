#include<bits/stdc++.h>

struct query_t {
	int t, id, val;
};
int solve(int N, int M, std::vector<query_t> Q) {
	struct seg_node {
		int val;
		int lazy;
	};
	std::vector<seg_node> seg(2*M, seg_node{N+1, 0});
	auto update_node = [&](int a) {
		if (a >= M) {
			// noop
		} else {
			seg[a].val = seg[a].lazy + std::min(seg[2*a].val, seg[2*a+1].val);
		}
	};
	auto update_all = [&](int a) {
		for(; a; a >>= 1) update_node(a);
	};
	auto update_all_changed = [&](int a) {
		a >>= __builtin_ctz(a);
		update_all(a >> 1);
	};
	auto decr_range = [&](int l, int r) {
		assert(l <= r);
		for (int a = M+l, b = M+r+1; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				seg[a].val--;
				seg[a].lazy--;
				a++;
			}
			if (b & 1) {
				--b;
				seg[b].val--;
				seg[b].lazy--;
			}
		}
		update_all_changed(M+l);
		update_all_changed(M+l+r);
	};

	std::vector<std::array<int, 2>> person_range(N, {-1, -1});

	auto add_query = [&](query_t q) -> bool {
		auto [t, id, val] = q;
		if (seg[M+t].val - seg[M+t].lazy == N+1) {
			seg[M+t].val = seg[M+t].lazy + val;
			update_all((M+t)>>1);
		} else if (seg[M+t].val - seg[M+t].lazy != val) {
			return false;
		}

		if (person_range[id] == std::array<int, 2>{-1, -1}) {
			seg[M+t].lazy--;
			seg[M+t].val--;
			update_all((M+t)>>1);
			person_range[id] = {t,t};
		} else if (t < person_range[id][0]) {
			decr_range(t, person_range[id][0]-1);
			person_range[id][0] = t;
		} else if (t > person_range[id][1]) {
			decr_range(person_range[id][1]+1, t);
			person_range[id][1] = t;
		}
		return seg[1].val >= 0;
	};

	int ans = 0;
	for (auto q : Q) {
		if (add_query(q)) {
			ans++;
		} else {
			break;
		}
	}
	return ans;
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<query_t> Q(M);
		for (auto& q : Q) {
			std::cin >> q.t >> q.id >> q.val; 
			q.t--, q.id--, q.val++;
		}
		std::cout << solve(N, M, std::move(Q)) << '\n';
	}

	return 0;
}
