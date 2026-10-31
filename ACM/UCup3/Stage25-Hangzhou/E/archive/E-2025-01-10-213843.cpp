#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, F; cin >> N >> F;
		struct person_t {
			int idx;
			int l, r;
		};
		std::vector<person_t> people(N);
		for (int i = 0; i < N; i++) {
			auto& p = people[i];
			p.idx = i;
			cin >> p.l >> p.r;
		}
		std::vector<person_t> orig_people = people;
		std::sort(people.begin(), people.end(), [&](person_t& a, person_t& b) { return a.l < b.l; });
		std::vector<std::pair<int, int>> stk; stk.reserve(N+1);
		std::vector<int> prv(N+1, -1);
		std::vector<int> nxt(N+1, -1);
		stk.push_back({F, N});
		prv[N] = N;
		nxt[N] = N;
		for (auto p : people) {
			while (stk.size() >= 2 && stk.back().first < p.l) stk.pop_back();
			int o = stk.back().second;
			prv[p.idx] = o;
			nxt[p.idx] = nxt[o];
			nxt[prv[p.idx]] = p.idx;
			prv[nxt[p.idx]] = p.idx;
			while (stk.size() >= 1 && stk.back().first <= p.r) stk.pop_back();
			stk.push_back({p.r, p.idx});
		}
		std::vector<int> perm; perm.reserve(N);
		int64_t ans = 0;
		int pos = 0;
		for (int i = nxt[N]; i != N; i = nxt[i]) {
			perm.push_back(i);
			ans += std::max(orig_people[i].l - pos, 0);
			ans += orig_people[i].r - orig_people[i].l;
			pos = orig_people[i].r;
		}
		cout << ans << '\n';
		assert(int(perm.size()) == N);
		for (int i = 0; i < N; i++) {
			cout << perm[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
