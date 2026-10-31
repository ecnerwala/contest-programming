#include <bits/stdc++.h>
using namespace std;

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

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
	void addEdge(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
					return p;
				}
		}
		return 0;
	}
	ll calc(int s, int t) {
		ll flow = 0; q[0] = s;
		rep(L,0,31) do { // 'int L=30' maybe faster for random data
			lvl = ptr = vi(sz(q));
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}
	bool leftOfMinCut(int a) { return lvl[a] != 0; }
};



int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	auto solve_perfect = [&](string S) -> pair<int, vector<int>> {
		// cerr << "perfect " << S << '\n';
		int N = (int)S.size();
		int L = N / 10;
		assert(N == L*10);
		if(N == 0) return {0, {}};
		for(int parity = 0; parity < 2; parity++){
			vector<int> H(10, L * 4);
			vector<int> seq(N, -1);
			vector<int> allowed(N, 0);

			int skip_I = 0;
			if(parity == 0){
				while(S[skip_I] != 'I') skip_I++;
			} else {
				int n_I = 0;
				for(int i = N-1; i >= 0; i--){
					if(S[i] == 'I'){
						if(n_I == 2*L-1) skip_I = i;
						n_I++;
					}
				}
			}

			// int cur_bucket = -1;
			vector<pair<int,int>> I_pairs;
			int cur_I = -1;

			int n_I = 0;
			for(int i = N-1; i >= 0; i--){
				if(S[i] == 'I'){
					if(n_I < L){
						seq[i] = 9;
					} else if(n_I < 2*L-1 || i == skip_I){
						seq[i] = 8; // skip
					} else if(cur_I == -1){
						// no pair
						cur_I = i;
						// cur_bucket = 0;
						// for(int a = 0; a < 8; a += 2){
						// 	if(H[a] > H[cur_bucket]) cur_bucket = a;
						// }
						// seq[i] = cur_bucket;
					} else {
						// has pair
						I_pairs.push_back({i, cur_I});
						// seq[i] = cur_bucket+1;
						cur_I = -1;
						// cur_bucket = -1;
					}
					if(seq[i] >= 0) H[seq[i]] -= 4;
					n_I++;
				} else if(S[i] == 'O'){
					if(n_I < L){
						for(int a = 0; a < 8; a += 2){
							if(H[a] % 4 == 2){
								seq[i] = a;
							}
						}
						if(seq[i] == -1){
							int best = 0;
							for(int a = 0; a < 8; a += 2){
								if(H[a] > H[best]) best = a;
							}
							seq[i] = best;
						}
					}
					if(seq[i] >= 0){
						H[seq[i]] -= 2;
						H[seq[i]+1] -= 2;
					} else {
						if(cur_I == -1) allowed[i] = (1 << 4) - 1;
						// if(cur_bucket >= 0) allowed[i] ^= (1 << (cur_bucket / 2));
					}
				} else assert(false);
			}
			for(auto [x, y] : I_pairs){
				cerr << x << '-' << y << ' ';
			}
			cerr << '\n';
			sort(I_pairs.begin(), I_pairs.end(), [&](pair<int,int> a, pair<int,int> b){
				return a.second-a.first < b.second-b.first;
			});
			for(auto [l, r] : I_pairs){
				int cur_bucket = 0;
				for(int a = 0; a < 8; a += 2){
					if(H[a] > H[cur_bucket]) cur_bucket = a;
				}
				seq[l] = cur_bucket;
				seq[r] = cur_bucket+1;
				H[seq[l]] -= 4; H[seq[r]] -= 4;
				for(int i = l+1; l < r; l++){
					allowed[i] = ((1 << 4) - 1) ^ (1 << (cur_bucket / 2));
				}
			}

			int ok = 1;
			for(int i = 0; i < 10; i++){
				if(H[i] < 0) ok = 0;
			}
			for(int x : H) cerr << x << ' ';
			cerr << '\n';
			if(!ok) continue;
			Dinic flow(N+4+2);
			int st = N+4;
			int en = N+5;
			for(int a = 0; a < 8; a += 2){
				assert(H[a] == H[a+1]);
				flow.addEdge(N+(a/2), en, H[a] / 2);
			}
			int need_flow = 0;
			for(int i = 0; i < N; i++){
				if(seq[i] != -1) continue;
				need_flow++;
				for(int j = 0; j < 4; j++){
					if((allowed[i] >> j) & 1){
						flow.addEdge(i, N+j, 1);
					}
				}
				flow.addEdge(st, i, 1);
			}
			int val = flow.calc(st, en);
			if(val != need_flow) continue;
			for(int i = 0; i < N; i++){
				if(seq[i] != -1) continue;
				for(auto& edge : flow.adj[i]){
					if(edge.to >= N && edge.to < N+4){
						if(edge.flow() == 1){
							assert(seq[i] == -1);
							seq[i] = (edge.to-N) * 2;
						}
					}
				}
			}
			return {L, seq};
		}
		assert(false);
	};
	auto solve = y_combinator([&](auto self, string S) -> pair<int, vector<int>> {
		int tot_I = 0;
		for(char c : S) tot_I += (c == 'I');
		if(tot_I % 2 == 1){
			int i = 0;
			while(S[i] != 'I') i++;
			string V = S.substr(0, i) + S.substr(i+1);
			auto res = self(V);
			res.second.insert(res.second.begin() + i, 8); // trash
			return res;
		};

		int N = (int)S.size();
		int s = 0; // yes
		int e = min(N / 10, tot_I / 2) + 1; // no
		vector<int> s_use(N, 0);
		while(s + 1 < e){
			int m = (s+e)/2;
			vector<int> use(N, 0);
			int need = 10*m;
			for(int i = N-1; i >= 0; i--){
				if(S[i] == 'I' && need){
					need--;
					use[i] = 1;
				}
			}
			for(int i = 0; i < N; i++){
				if(S[i] == 'O' && need){
					need--;
					use[i] = 1;
				}
			}
			assert(need == 0);

			int n_I = 0;
			int n_O = 0;
			int bad = 0;
			for(int i = N-1; i >= 0; i--){
				if(!use[i]) continue;
				if(S[i] == 'I'){
					if(n_I < m && n_O > 8 * n_I){
						bad = 1;
					}
					n_I++;
				} else if(S[i] == 'O'){
					n_O++;
				} else assert(false);
			}
			if(bad){
				e = m;
			} else {
				s = m;
				s_use = use;
			}
		}
		auto use = s_use;
		string NS;
		for(int i = 0; i < N; i++){
			if(use[i]){
				NS += S[i];
			}
		}
		auto [r, seq] = solve_perfect(NS);
		vector<int> H(10);
		int idx = 0;
		vector<int> nseq;
		for(int i = 0; i < N; i++){
			if(use[i]){
				nseq.push_back(seq[idx]);
				if(S[i] == 'I'){
					H[seq[idx]] += 4;
				} else if(S[i] == 'O'){
					H[seq[idx]] += 2;
					H[seq[idx]+1] += 2;
				} else assert(false);
				idx++;
			} else if(S[i] == 'I'){
				nseq.push_back(8); // trash
			} else if(S[i] == 'O'){
				int a = 0;
				while(H[a] != H[a+1]){
					a += 2;
				}
				nseq.push_back(a);
			} else assert(false);
		}
		return {r, nseq};
	});
	int N;
	cin >> N;
	string S;
	cin >> S;
	auto [r, seq] = solve(S);
	cout << r << '\n';
	for(int i = 0; i < N; i++){
		cout << (seq[i] + 1) << " \n"[i+1 == N];
	}
}
