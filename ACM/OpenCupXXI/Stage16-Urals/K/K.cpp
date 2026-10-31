#include <bits/stdc++.h>
using namespace std;

vector<pair<int,int> > moves = {{2, 1}, {2, -1}, {1, 2}, {1, -2}, {-2, 1}, {-2, -1}, {-1, 2}, {-1, -2}};

vector<int> found;
vector<int> z;
int nverts = 0;
void dfs(int cur, int st, vector<vector<int> >& edges, vector<int>& vis){
	if(!found.empty()) return;
	if(cur == st){
		if(vis[st]){
			if(z.size() == nverts){
				found = z;
			}
			return;
		} else {
			vis[st] = 1;
			z.push_back(st);
		}
	}
	for(int w : edges[cur]){
		if(w != st && vis[w]) continue;
		if(w != st){
			vis[w] = 1;
			z.push_back(w);
		}
		dfs(w, st, edges, vis);
		if(w != st){
			vis[w] = 0;
			z.pop_back();
		}
	}
}
vector<int> block_paths[2][2][2][2];

void gen(){
block_paths[0][0][0][0] = {2,3,21,20,24,25,11,7,6,10,28,29,15,14,18,30,31,19,5,4,22,23,27,9,8,26,12,0,1,13,17,16,};
block_paths[0][0][0][1] = {2,3,21,20,24,25,11,7,6,10,28,29,15,14,18,19,5,4,22,23,27,9,8,26,12,0,1,13,17,16,};
block_paths[0][0][1][0] = {2,3,21,20,24,25,11,7,6,10,28,29,15,14,18,19,31,30,12,26,8,9,5,4,22,23,27,13,17,16,};
block_paths[0][0][1][1] = {2,3,21,20,24,25,11,7,6,10,28,29,15,14,18,19,5,4,22,23,27,9,8,26,12,13,17,16,};
block_paths[0][1][0][0] = {2,3,29,28,44,45,23,22,30,31,47,46,20,21,43,42,16,17,39,38,12,13,5,4,26,0,1,27,19,35,9,8,34,18,40,14,36,37,15,7,6,32,10,11,33,41,25,24,};
block_paths[0][1][0][1] = {2,3,29,28,44,45,23,22,30,31,39,38,16,17,43,42,20,21,5,13,12,4,26,0,1,27,19,35,9,8,34,18,40,14,36,37,15,7,6,32,10,11,33,41,25,24,};
block_paths[0][1][1][0] = {2,3,29,28,44,45,23,22,30,31,47,46,20,21,43,42,16,17,39,38,12,13,5,4,26,27,19,35,9,8,34,18,40,14,36,37,15,7,6,32,10,11,33,41,25,24,};
block_paths[0][1][1][1] = {2,3,29,28,44,45,23,22,30,31,39,38,16,17,43,42,20,21,5,13,12,4,26,27,19,35,9,8,34,18,40,14,36,37,15,7,6,32,10,11,33,41,25,24,};
block_paths[1][0][0][0] = {2,3,21,20,38,39,43,42,28,29,47,46,34,35,17,16,12,13,31,30,44,26,40,41,37,36,24,25,11,7,6,10,22,23,5,4,8,9,27,45,33,32,18,0,1,19,15,14,};
block_paths[1][0][0][1] = {2,3,21,20,38,39,43,42,28,29,33,32,44,45,31,30,34,35,17,16,12,13,27,26,40,41,37,36,24,25,11,7,6,10,22,23,5,9,8,4,18,0,1,19,15,14,};
block_paths[1][0][1][0] = {2,3,21,20,38,39,43,42,28,29,47,46,34,35,17,16,12,13,31,30,44,45,27,26,40,41,37,36,24,25,11,7,6,10,22,23,5,9,8,4,18,32,33,19,15,14,};
block_paths[1][0][1][1] = {2,3,21,20,38,39,43,42,28,29,33,32,44,45,31,30,34,35,17,16,12,13,27,26,40,41,37,36,24,25,11,7,6,10,22,23,5,9,8,4,18,19,15,14,};
block_paths[1][1][0][0] = {2,3,29,28,54,55,71,70,44,45,67,66,40,41,63,62,36,37,53,52,68,69,47,46,20,21,43,42,64,65,39,61,60,38,16,32,58,59,33,11,10,18,19,35,57,56,34,8,9,31,23,22,6,7,15,14,30,4,12,13,5,27,49,48,26,0,1,17,25,51,50,24,};
block_paths[1][1][0][1] = {2,3,29,28,54,55,63,62,40,41,67,66,44,45,53,52,68,69,47,46,20,21,43,42,64,65,39,61,60,38,16,32,58,59,33,11,10,18,19,35,57,56,34,8,9,31,23,22,6,7,15,37,36,14,30,4,12,13,5,27,49,48,26,0,1,17,25,51,50,24,};
block_paths[1][1][1][0] = {2,3,29,28,54,55,71,70,44,45,67,66,40,41,63,62,36,37,53,52,68,69,47,46,20,21,43,42,64,65,39,61,60,38,16,32,58,59,33,11,10,18,19,27,49,48,26,4,12,13,5,31,23,22,6,7,15,14,30,56,57,35,34,8,9,17,25,51,50,24,};
block_paths[1][1][1][1] = {2,3,29,28,54,55,63,62,40,41,67,66,44,45,53,52,68,69,47,46,20,21,43,42,64,65,39,61,60,38,16,32,58,59,33,11,10,18,19,27,49,48,26,4,12,13,5,31,23,22,6,7,15,37,36,14,30,56,57,35,34,8,9,17,25,51,50,24,};
}

// void gen(){
// 	for(int xsz = 0; xsz < 2; xsz++){
// 		for(int ysz = 0; ysz < 2; ysz++){
// 			for(int tempty = 0; tempty < 2; tempty++){
// 				for(int bempty = 0; bempty < 2; bempty++){
// 					int X = 4 + 2 * xsz;
// 					int Y = 4 + 2 * ysz;
// 					int n = 2 * X * Y;
// 					vector<int> exists(n, 1);
// 					if(tempty) exists[0] = exists[1] = 0;
// 					if(bempty) exists[n-1] = exists[n-2] = 0;
// 					vector<vector<int> > edges(n);
// 					for(int i = 0; i < X; i++){
// 						for(int j = 0; j < Y; j++){
// 							int r = Y * i + j;
// 							if(!exists[2*r]) continue;
// 							edges[2*r].push_back(2*r+1);
// 							edges[2*r+1].push_back(2*r);
// 						}
// 					}
// 					for(int i = 0; i < X; i++){
// 						for(int j = 0; j < Y; j++){
// 							int r = Y * i + j;
// 							for(auto [a, b] : moves){
// 								a += i;
// 								b += j;
// 								if(a < 0 || b < 0 || a >= X || b >= Y) continue;
// 								int s = Y * a + b;
// 								if(exists[2*r] && exists[2*s]){
// 									edges[2*r].push_back(2*s);
// 									edges[2*r+1].push_back(2*s+1);
// 								}
// 							}
// 						}
// 					}
// 					nverts = n - tempty * 2 - bempty * 2;
// 					vector<int> vis(n, 0);
// 					int st = 2;
// 					found = {};
// 					z = {};
// 					// cerr << "? " << X << ' ' << Y << ' ' << tempty << ' ' << bempty << '\n';
// 					// cerr << nverts << '\n';
// 					dfs(st, st, edges, vis);
// 					if(!found.empty()){
// 						block_paths[xsz][ysz][tempty][bempty] = found;
// 						cerr << "block_paths[" << xsz << "][" << ysz << "][" << tempty << "][" << bempty << "] = {";
// 						for(int q : found){
// 							cerr << q << ',';
// 						}
// 						cerr << "};";
// 						cerr << '\n';
// 					}
// 				}
// 			}
// 		}
// 	}
// }

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	gen();
	int n;
	cin >> n;
	int B = n / 4;
	vector<int> s(B, 0);
	if(n % 4 == 2) s.back() = 1;
	vector<tuple<int,int,int> > cycle;
	for(int i = 0; i < B; i++){
		for(int j = 0; j < B; j++){
			int xsz = s[i];
			int ysz = s[j];
			int X = 4 + 2 * xsz;
			int Y = 4 + 2 * ysz;
			int tempty = (i == 0 && j == 0);
			int bempty = (i == B-1 && j == B-1);
			vector<int> path = block_paths[xsz][ysz][tempty][bempty];
			vector<tuple<int,int,int> > real_path;
			for(int a : path){
				int d = a % 2;
				a /= 2;
				int y = a % Y;
				a /= Y;
				int x = a;
				a /= X;
				assert(a == 0);
				int realx = x + 4*i;
				int realy = y + 4*j;
				real_path.push_back({realx, realy, d});
			}
			// for(auto [x, y, d] : real_path){
			// 	cerr << (x+1) << ' ' << (y+1) << ' ' << d << '\n';
			// }
			if(i == 0 && j == 0){
				cycle = real_path;
				// cerr << '\n';
			} else {
				for(int a = 0; a + 1 < (int)cycle.size(); a++){
					auto [ax, ay, ad] = cycle[a];
					auto [bx, by, bd] = cycle[a+1];
					if(ax != bx || ay != by) continue;
					assert((ad ^ bd) == 1);
					for(int c = 0; c + 1 < (int)real_path.size(); c++){
						auto [cx, cy, cd] = real_path[c];
						auto [dx, dy, dd] = real_path[c+1];
						if(cx != dx || cy != dy) continue;
						assert((cd ^ dd) == 1);
						int dist = abs(ax - cx) * abs(ay - cy);
						if(dist != 2) continue;
						vector<tuple<int,int,int> > shifted_path;
						for(int q = 0; q < (int)real_path.size(); q++){
							shifted_path.push_back(real_path[(c+1+q) % real_path.size()]);
						}
						if(ad != dd){
							reverse(shifted_path.begin(), shifted_path.end());
						}
						cycle.insert(cycle.begin() + a+1, shifted_path.begin(), shifted_path.end());
						goto glued;
					}
				}
				cerr << "failed to glue" << '\n';
				exit(0);
				glued:;
			}
		}
	}
	for(auto [x, y, d] : cycle){
		cout << (x+1) << ' ' << (y+1) << ' ' << d << '\n';
	}
}