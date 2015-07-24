#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

using namespace std;

const int MAXN = 100000, MAXK = 20;

vector<int> edges[MAXN];
int parK[MAXK][MAXN];
int depth[MAXN];

int V, Q;

void dfs(int u, int prev){
  if(parK[u][0] != 0) return;
  parK[u][0] = prev;
  depth[u] = depth[prev]+1;
  for(int v = 0; v < edges[u].size(); v++){
    dfs(v, u);
  }
}

int lca(int u, int v){
  int k = MAXK-1;
  while(k>=0){
    int d = 1<<k;
    if(depth[u]>=depth[v]+d) u=parK[k][u];
    if(depth[v]>=depth[u]+d) v=parK[k][v];
    k--;
  }
  k = MAXK-1;
  while(k>=0){
    if(parK[k][u] != parK[k][v]){
      u = parK[k][u];
      v = parK[k][v];
    }
    k--;
  }
  if(u != v){
    u = parK[0][u];
    v = parK[0][v];
  }
  return u;
}

int main(){
  int u, v, root;
  scanf("%d%d%d", &V, &Q, &root);
  for(int i = 0; i < V; i++){
    scanf("%d%d", &u, &v);
    edges[u].push_back(v);
    edges[v].push_back(u);
  }
  dfs(root, -1);
  for(int k = 1; k < MAXK; k++){
    for(int i = 0; i < V; i++){
      parK[i][k] = parK[parK[i][k-1]][k-1];
    }
  }
  for(int i = 0; i < Q; i++){
    scanf("%d%d", &u, &v);
    printf("%d\n", lca(u,v));
  }
  return 0;
}
