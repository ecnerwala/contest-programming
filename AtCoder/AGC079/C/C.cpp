#include <bits/stdc++.h>
#include <cassert>

// Old solution to POI/17/kol
// https://szkopul.edu.pl/problemset/problem/TJVrS_hRC8W5Q6ZBW6mETAIm/site/?key=statement
//
// Difference: when one stack is empty, you're allowed to tuck under the other stack
//
// Problems are symmetric: reverse time + invert perm is the same
//
// Queue means that if the front element is poppable, you might as well, and otherwise if the back is poppable, you must reverse => might as well pop; you never get anything by waiting to pop.
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	auto solve = [&](std::vector<int> A) -> int {
		int num_rem = N;
		vector<int> pred(N, -1);
		vector<int> succ(N, -1);
		struct lst_t {
			int head, tail;
			bool edge_neq;
		};
		// list.back().edge_neq = whether lst.back() is forced different from lst.end()[-2]
		vector<lst_t> lst; lst.reserve(N);

		vector<pair<int, bool>> rel(N, {-2, false});

		for (int i = N-1; i >= 0; i--) {
			while (A[i] < num_rem) {
				num_rem--;

				// insert num_rem
				lst.push_back(lst_t{num_rem, num_rem, false});
			}

			assert(!lst.empty());
			if (lst.back().head == A[i]) {
				if (lst.back().tail == lst.back().head) {
					bool edge_neq = lst.back().edge_neq;
					lst.pop_back();
					if (edge_neq) {
						assert(!lst.empty());
						rel[i] = {lst.back().head, true};
					} else {
						rel[i] = {-1, false};
					}
				} else {
					lst.back().head = succ[lst.back().head];
					rel[i] = {lst.back().head, false};
				}
			} else {
				lst_t l = lst.back(); lst.pop_back();
				while (!lst.empty() && lst.back().head != A[i]) {
					if (l.edge_neq) {
						return -1;
					}
					succ[l.tail] = lst.back().head;
					pred[lst.back().head] = l.tail;
					l.tail = lst.back().tail;
					l.edge_neq = lst.back().edge_neq;
					lst.pop_back();
				}
				if (lst.empty()) {
					if (l.tail == A[i]) {
						// We're actually okay by popping the bottom entry
						assert(l.head != A[i]);
						l.tail = pred[l.tail];
						rel[i] = {l.tail, false};
						continue;
					}
					return -1;
				}
				assert(lst.back().head == A[i]);

				l.edge_neq = true;
				if (lst.back().head == lst.back().tail) {
					rel[i] = {l.head, true};
					bool should_merge = lst.back().edge_neq;
					lst.pop_back();
					if (should_merge) {
						assert(!lst.empty());
						succ[l.tail] = lst.back().head;
						pred[lst.back().head] = l.tail;
						l.tail = lst.back().tail;
						l.edge_neq = lst.back().edge_neq;
						lst.back() = l;
					} else {
						l.edge_neq = false;
						lst.push_back(l);
					}
				} else {
					lst.back().head = succ[lst.back().head];
					rel[i] = {lst.back().head, false};
					lst.push_back(l);
				}
			}
		}

		vector<std::pair<int, bool>> ans_B(N);
		for (int i = 0; i < N; i++) {
			std::pair<int, bool> ans;
			assert(rel[i].first != -2);
			if (rel[i].first != -1) {
				ans = ans_B[rel[i].first];
				ans.second ^= rel[i].second;
			} else {
				ans = {A[i], false};
			}
			ans_B[A[i]] = ans;
		}
		std::vector<bool> is_set(N, false);
		std::vector<bool> base_dir(N, false);
		bool cur_dir = false;
		std::vector<int> q(2*N);
		int head = N, tail = N-1;
		int nxt = N-1;
		int ans = 0;
		for (int i = N-1; i >= 0; i--) {
			//std::cerr << i << ' ' << ans_B[i].first << ' ' << ans_B[i].second << '\n';
			if (!is_set[ans_B[i].first]) {
				base_dir[ans_B[i].first] = cur_dir ^ ans_B[i].second;
				is_set[ans_B[i].first] = true;
			}
			if (cur_dir != (base_dir[ans_B[i].first] ^ ans_B[i].second)) {
				cur_dir ^= 1;
				if (head != tail) ans++;
			}
			if (!cur_dir) {
				// push to the tail
				q[++tail] = i;
			} else {
				q[--head] = i;
			}
			while (head <= tail) {
				assert(nxt >= 0);
				if (A[nxt] == q[head] || A[nxt] == q[tail]) {
					if (q[cur_dir ? tail : head] != A[nxt]) {
						cur_dir ^= 1;
						if (head != tail) ans++;
					}
					if (!cur_dir) {
						assert(q[head] == A[nxt]);
						head++;
					} else {
						assert(q[tail] == A[nxt]);
						tail--;
					}
					nxt--;
				} else {
					break;
				}
			}
		}
		assert(nxt == -1);
		return ans;
	};

	int T; std::cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N); for (auto& a : A) { cin >> a, a--; }
	}

	return 0;
}
