#include <bits/stdc++.h>
using namespace std;

const int MAXJ = 8;
const int MAXR = 256;

vector<pair<int,int> > box_mst[MAXJ][MAXR][MAXR];
vector<pair<int,int> > box_mst2[MAXR][MAXR];

struct UF {
    int n;
    vector<int> par;
    UF(int _n) : n(_n) {
        for(int i = 0; i < n; i++) par.push_back(i);
    }
    int find(int a){
        if(a != par[a]) par[a] = find(par[a]);
        return par[a];
    }
    bool join(int a, int b){
        a = find(a);
        b = find(b);
        par[a] = b;
        return (a != b);
    }
};

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int n, r, c, Q;
	cin >> n >> r >> c >> Q;
	vector<vector<int> > u(r, vector<int>(c));
	vector<vector<int> > v(r, vector<int>(c));
	vector<vector<int> > w(r, vector<int>(c));
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			cin >> u[i][j] >> v[i][j] >> w[i][j];
			u[i][j]--; v[i][j]--;
		}
	}
	UF uf(n);
	auto get_tree = [&](vector<pair<int,int> >& edges){
		for(int i = 0; i < n; i++) uf.par[i] = i;
		vector<pair<int,int> > merged_edges;
		for(auto e : edges){
			int x = u[e.first][e.second];
			int y = v[e.first][e.second];
			if(uf.join(x, y)) merged_edges.push_back(e);
		}
		return merged_edges;
	};

	auto merge = [&](vector<pair<int,int> >& s1, vector<pair<int,int> >& s2){
		vector<pair<int,int> > edges(s1.size() + s2.size());
		std::merge(s1.begin(), s1.end(), s2.begin(), s2.end(), edges.begin(), 
			[&](pair<int,int> a, pair<int,int> b){
				return w[a.first][a.second] < w[b.first][b.second];
			}
		);
		return get_tree(edges);
	};
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			box_mst[0][i][j] = {{i, j}};
		}
	}
	for(int b = 1; b < MAXJ; b++){
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				box_mst2[i][j] = box_mst[b-1][i][j];
				int ni = i + (1 << (b-1));
				if(ni < r) box_mst2[i][j] = merge(box_mst2[i][j], box_mst[b-1][ni][j]);
			}
		}
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				box_mst[b][i][j] = box_mst2[i][j];
				int nj = j + (1 << (b-1));
				if(nj < c) box_mst[b][i][j] = merge(box_mst[b][i][j], box_mst2[i][nj]);
			}
		}
	}
	while(Q--){
		int x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;
		x1--; y1--;
		int B = 0;
		while((1 << (B+1)) <= min(x2 - x1, y2 - y1)) B += 1;
		vector<pair<int,int> > cedges = {};
		for(int g = x1; g < x2; g += (1 << B)){
			for(int h = y1; h < y2; h += (1 << B)){
				vector<pair<int,int> >& z = box_mst[B][min(g, x2 - (1 << B))][min(h, y2 - (1 << B))];
				cedges.insert(cedges.end(), z.begin(), z.end());
			}
		}
		sort(cedges.begin(), cedges.end(),
			[&](pair<int,int> a, pair<int,int> b){
				return w[a.first][a.second] < w[b.first][b.second];
			}
		);
		cedges = get_tree(cedges);
		if((int)cedges.size() == n-1){
			int sum = 0;
			for(pair<int,int> e : cedges) sum += w[e.first][e.second];
			cout << sum << '\n';
		} else {
			cout << -1 << '\n';
		}
	}
}