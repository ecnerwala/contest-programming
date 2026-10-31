#include<bits/stdc++.h>

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
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

		std::map<std::string, int> topic_id;
		std::vector<std::vector<int>> topics(N);
		for (auto& t : topics) {
			int m; cin >> m;
			t.resize(m);
			for (auto& v : t) {
				std::string s; cin >> s;
				auto [it, did_insert] = topic_id.insert({s, int(topic_id.size())});
				assert(it != topic_id.end());
				v = it->second;
			}
		}
		int M = int(topic_id.size());
		std::vector<int> alive_topics(M);
		std::iota(alive_topics.begin(), alive_topics.end(), 0);
		std::vector<std::vector<bool>> topic_stacks(M);
		for (int m = 0; m < M; m++) {
			// can't go past the root
			topic_stacks[m].push_back(true);
		}

		std::vector<std::vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[P[i]].push_back(i);
		}
		std::y_combinator([&](auto self, int cur) -> void {
			for (int t : topics[cur]) {
				topic_stacks[t].push_back(false);
			}
			if (ch[cur].empty()) {
				int l = 0;
				for (int z = 0; z < int(alive_topics.size()); z++) {
					int t = alive_topics[z];
					if (!topic_stacks[t].back()) {
						topic_stacks[t].back() = true;
						alive_topics[l++] = t;
					}
				}
				alive_topics.resize(l);
			} else {
				for (int nxt : ch[cur]) {
					self(nxt);
				}
			}
			for (int t : topics[cur]) {
				topic_stacks[t].pop_back();
			}
		})(0);

		cout << "Case #" << case_num << ": " << alive_topics.size() << '\n';
	}

	return 0;
}
