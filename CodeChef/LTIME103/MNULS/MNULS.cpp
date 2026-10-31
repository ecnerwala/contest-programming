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

std::vector<std::vector<int>> get_optimal_paths(int K) {
	using loss_t = std::pair<int64_t, int64_t>;
	std::vector<int> precomp(K+1);
	precomp[K] = 0;
	for (int i = K; i >= 0; i--) {
		precomp[i] = std::gcd(K, i) == 1 ? precomp[i+1] + 1 : 0;
	}
	auto greedy_loss = [&](int64_t cur, int steps) -> loss_t {
		while (cur*cur <= K) cur += cur * cur, steps++;
		int64_t ans = steps * K - cur;
		int v = precomp[ans%K];
		ans += v, steps += v;
		return {ans, -steps};
	};

	loss_t max_loss;
	{
		int64_t cur = 1, steps = 1;
		while (cur * cur <= K) cur += cur * cur, steps++;
		assert(!(cur & 1));
		//std::cerr << "Greedy " << steps << ' ' << cur << ' ' << cur+K << '\n';
		for (int l = K; true; l--) {
			assert(l >= 1);
			if (cur+l <= (cur+1) * std::gcd(l, cur)) {
				if (l >= K-1 || ((cur+l)%2 == 0)) {
					cur = cur+l;
					steps++;
					break;
				}
			}
		}
		//std::cerr << "Greedy " << steps << ' ' << cur << '\n';
		int64_t ls = steps * K - cur;
		int v = precomp[ls%K];
		ls += v, steps += v;
		max_loss = {ls, -steps};
		//std::cerr << "Greedy " << steps << ' ' << cur << '\n';
	}

	auto is_terminal = [&](int cur) -> bool {
		return (cur+K) <= (cur+1) * std::gcd(cur, K) || (cur+K-1) <= (cur+1) * std::gcd(cur, K-1);
	};

	std::vector<std::vector<int>> optimals;

	std::vector<int> cur_path;
	cur_path.push_back(1);
	std::y_combinator([&](auto self) -> void {
		int cur = cur_path.back();
		int len = int(cur_path.size());
		//std::cerr << "dfs " << len << ' ' << cur << '\n';
		if (int(optimals.size()) <= len) {
			optimals.resize(len+1);
		}
		if (optimals[len].empty() || cur > optimals[len].back()) {
			optimals[len] = cur_path;
			if (cur_path.size() >= 2 && is_terminal(cur)) {
				optimals.resize(len+1);
				int ls = len*K - cur;
				int steps = len;
				int v = precomp[ls%K];
				ls += v, steps += v;
				max_loss = std::min(max_loss, {ls, -steps});
				return;
			}
		} else if (!optimals[len].empty() && is_terminal(optimals[len][len-1])) {
			// we're worse than them
			return;
		}

		for (int l = K; l >= 1; l--) {
			int nxt = cur + l;
			if (greedy_loss(nxt, len+1) > max_loss) break;
			if (nxt <= (cur+1) * std::gcd(l, cur)) {
				cur_path.push_back(nxt);
				self();
				cur_path.pop_back();
			}
		}
	})();

	for (int l = 1; l+1 < int(optimals.size()); l++) {
		if (optimals[l+1][l-1] != optimals[l][l-1]) {
			std::cout << "BAD" << ' ' << K << ' ' << l+1 << '\n';
			{
				int cur = optimals[l][l-1];
				int nxt = cur+K;
				for (;; nxt--) {
					if (nxt <= (cur+1) * std::gcd(cur, nxt)) break;
				}
				optimals[l].push_back(nxt);
			}
			for (auto x : optimals[l]) { std::cout << x << ' '; } std::cout << '\n';
			optimals[l].pop_back();
			for (auto x : optimals[l+1]) { std::cout << x << ' '; } std::cout << '\n';
		}
	}
	return optimals;
}

void test(int K) {
	using std::cout;
	using std::cerr;

	const int INF = int(1e9);
	std::vector<int> dist(5*K+1, INF);
	std::vector<int> prv(5*K+1, -1);
	std::vector<int> q;
	dist[1] = 1;
	q.push_back(1);
	auto print_path = [&](int st) -> void {
		std::vector<int> path;
		for (int cur = st; cur != -1; cur = prv[cur]) {
			path.push_back(cur);
		}
		std::reverse(path.begin(), path.end());
		for (int v : path) { cout << v << ' '; } cout << '\n';
	};
	for (int d = 2; true; d++) {
		std::vector<int> nq;
		for (int A : q) {
			for (int l = 1; l <= K; l++) {
				if (A+l <= (std::gcd(l, A) * (A+1))) {
					assert(A+l < int(dist.size()));
					if (dist[A]+1 < dist[A+l]) {
						dist[A+l] = dist[A]+1;
						prv[A+l] = A;
						nq.push_back(A+l);
					}
				}
			}
		}
		std::sort(nq.begin(), nq.end(), std::greater<>());
		if (prv[nq.front()] != q.front()) {
			cout << "BAD " << K << ' ' << d << '\n';
			int cur = q.front();
			int nxt = cur + 1;
			for (int64_t l = K; l >= 1; l--) {
				if (cur+l <= (std::gcd(l, cur) * (cur+1))) {
					nxt = cur+l;
					break;
				}
			}
			assert(nxt < nq.front());
			assert(prv[nxt] == cur);
			print_path(nxt);
			print_path(nq.front());
		}
		if (nq.front() - prv[nq.front()] >= K-1) {
			break;
		}
		q = std::move(nq);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int K = 3; K <= 10000; K++) {
		cerr << "testing " << K << '\n';
		auto r = get_optimal_paths(K);
		//for (const auto& v : r) { for (auto x : v) { cerr << x << ' '; } cerr << '\n'; }
	}
	exit(0);
	for (int K = 3; K <= 10000; K++) {
		cerr << "testing " << K << '\n';
		test(K);
	}

	int64_t N, K; cin >> N >> K;
	int64_t cur = 1;
	int64_t ans = 1;
	while (cur < N) {
		int64_t nxt = cur+1;
		for (int64_t l = K; l >= 1; l--) {
			if (cur+l <= (std::gcd(l, cur) * (cur+1))) {
				nxt = cur+l;
				break;
			}
		}
		if (nxt == cur + K) {
			ans += (N - cur + K-1) / K;
			cur += (N - cur + K-1) / K * K;
			break;
		}
		ans++;
		cur = nxt;
	}
	cout << ans << '\n';

	return 0;
	const int INF = int(1e9);
	std::vector<int> dist(K*K+1, INF);
	std::vector<int> prv(K*K+1, -1);
	dist[1] = 1;
	std::vector<int> best(K*K+1, -1);
	for (int A = 1; A < int(dist.size()); A++) {
		best[dist[A]] = A;
		for (int l = 1; l <= K; l++) {
			if (A+l >= int(dist.size())) break;
			if (A+l <= (std::gcd(l, A) * (A+1))) {
				if (dist[A]+1 <= dist[A+l]) {
					dist[A+l] = dist[A]+1;
					prv[A+l] = A;
				}
			}
		}
	}

	for (int d = 1; d < int(dist.size()); d++) {
		if (best[d] == -1) continue;
		cerr << d << '\n';
		std::vector<int> path;
		for (int cur = best[d]; cur != -1; cur = prv[cur]) {
			path.push_back(cur);
		}
		std::reverse(path.begin(), path.end());
		for (int v : path) { cerr << v << ' '; } cerr << '\n';
		if (best[d] == best[d-1] + K) {
			cerr << "steady state" << '\n';
			break;
		}
	}
	/*
	for (int i = 1; i <= 10*K; i++) {
		cerr << i << ' ' << dist[i] << '\n';
	}
	*/

	return 0;
}
