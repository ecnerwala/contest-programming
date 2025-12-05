#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

struct node {
	node *l, *r;
	ll diff;
	// string representative;
	bool operator == (const node v){
		return l == v.l && r == v.r && diff == v.diff;
	}
};

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K;
	cin >> N >> K;
	auto ask = [&](int a, string S) -> ll {
		assert((int)S.size() == N);
		cout << "? " << a << ' ' << S << '\n';
		cout << flush;
		ll ans;
		cin >> ans;
		return ans;
	};
	// find classes of vertices
	mt19937 mt(48);
	vector<vector<node> > nodes(N+1);
	nodes[N].push_back(
		node {
			nullptr,
			nullptr,
			0,
			// string(N, 'L'),
		}
	);

	auto compute_node = [&](int st, string ns) -> pair<ll, node*> {
		int level = (int)ns.size();
		auto base_val = ask(st, ns + string(N-level, 'L'));
		vector<node*> candidates;
		for(int i = 0; i < (int)nodes[level].size(); i++){
			candidates.push_back(&nodes[level][i]);
		}
		while(candidates.size() > 1){
			node* x = candidates[0];
			node* y = candidates[1];
			string suffix;

			ll xsum = 0;
			ll ysum = 0;
			while((int)suffix.size() < N-level){
				if(x->l != y->l || (ysum - xsum != 0)){
					x = x->l;
					y = y->l;
					suffix += "L";
				} else if(x->r != y->r || (ysum - xsum + y->diff - x->diff != 0)){
					ysum += y->diff;
					xsum += x->diff;
					x = x->r;
					y = y->r;
					suffix += "R";
				} else assert(false);
			}
			ll val = ask(st, ns + suffix) - base_val;

			vector<node*> new_candidates;
			for(auto v : candidates){
				node* cv = v;
				ll vsum = 0;
				for(char c : suffix){
					if(c == 'L'){
						cv = cv->l;
					} else {
						vsum += cv->diff;
						cv = cv->r;
					}
				}
				if(vsum == val){
					new_candidates.push_back(v);
				}
			}
			assert(new_candidates.size() < candidates.size());
			candidates = new_candidates;
		}
		return {base_val, candidates.front()};
	};

	vector<vector<vector<string> > > diff_string(N+1);
	for(int level = N-1; level >= 0; level--){
		const int ITS = 400;
		int L = ITS;
		for(int it = 0; it < L; it++){
			int st = mt() % K;
			string s;
			for(int i = 0; i < level; i++) s += "LR"[mt() % 2];

			vector<ll> base_val(2);
			vector<node*> ch(2);
			for(int dir = 0; dir < 2; dir++){
				string ns = s + "LR"[dir];
				tie(base_val[dir], ch[dir]) = compute_node(st, ns);
			}
			ll new_diff = base_val[1] - base_val[0];
			bool exists = false;
			node new_node {
				ch[0],
				ch[1],
				new_diff,
			};
			for(int i = 0; i < (int)nodes[level].size(); i++){
				if(nodes[level][i] == new_node){
					exists = true;
				}
			}
			if(!exists){
				nodes[level].push_back(new_node);
			}
		}
	}
	vector<pair<ll, node*>> starts(K);
	for(int st = 0; st < K; st++){
		starts[st] = compute_node(st, "");
	}
	cout << "!" << '\n';
	cout << flush;
	int Q;
	cin >> Q;
	while(Q--){
		int st;
		cin >> st;
		string S;
		cin >> S;
		ll score;
		node* loc;
		tie(score, loc) = starts[st];
		for(char c : S){
			if(c == 'L'){
				loc = loc->l;
			} else {
				score += loc->diff;
				loc = loc->r;
			}
		}
		cout << score << '\n';
		cout << flush;
	}
}
