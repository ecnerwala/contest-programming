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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

		struct data_t {
			int base_val = 0;
			int min_val = 0;
			int num_neg = 0;
			int num_pos = 0;
			std::vector<int> others;
		};
		auto merge = [&](data_t& a, data_t& b) -> data_t {
			data_t r;
			r.base_val = a.base_val + b.base_val;
			auto a_it = a.others.begin();
			auto b_it = b.others.begin();
			r.min_val = r.base_val;
			while (true) {
				int nxt_a;
				if (a.num_neg > 0 && (a_it == a.others.end() || -1 <= *a_it)) {
					nxt_a = -1;
					a.num_neg--;
				} else if (a.num_pos > 0 && (a_it == a.others.end() || 1 <= *a_it)) {
					nxt_a = 1;
					a.num_pos--;
				} else if (a_it != a.others.end()){
					nxt_a = *a_it;
					a_it++;
				} else {
					break;
				}

				int nxt_b;
				if (b.num_neg > 0 && (b_it == b.others.end() || -1 <= *b_it)) {
					nxt_b = -1;
					b.num_neg--;
				} else if (b.num_pos > 0 && (b_it == b.others.end() || 1 <= *b_it)) {
					nxt_b = 1;
					b.num_pos--;
				} else if (b_it != b.others.end()){
					nxt_b = *b_it;
					b_it++;
				} else {
					break;
				}

				int v = nxt_a + nxt_b;
				assert(r.others.empty() || v >= r.others.back());
				r.others.push_back(v);
				if (v < 0) r.min_val += v;
			}
			return r;
		};

		std::vector<int> ans(N);
		std::vector<data_t> cur_data(N);
		for (int i = N-1; i >= 0; i--) {
			ans[i] = cur_data[i].min_val;
			if (S[i] == '0') {
				cur_data[i].num_pos++;
			} else if (S[i] == '1') {
				cur_data[i].num_neg++;
				cur_data[i].base_val++;
			} else assert(false);

			int p = P[i];
			if (cur_data[p].num_pos == 0 && cur_data[p].num_neg == 0 && cur_data[p].others.empty()) {
				assert(cur_data[p].base_val == 0);
				assert(cur_data[p].min_val == 0);
				std::swap(cur_data[i], cur_data[p]);
			} else {
				cur_data[p] = merge(cur_data[p], cur_data[i]);
			}
			cur_data[i] = {};
		}

		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
