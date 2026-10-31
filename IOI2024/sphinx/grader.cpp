#include "sphinx.h"
#include <cassert>
#include <cstdio>
#include <queue>
#include <string>
#include <vector>

namespace {

const int CALLS_CNT_LIMIT = 2750;

int calls_cnt;

int N, M;
std::vector<int> C;
std::vector<int> X, Y;
std::vector<std::vector<int>> adj;

void quit(const char* message) {
  printf("%s\n", message);
  exit(0);
}

int count_components(const std::vector<int>& S) {
  int components_cnt = 0;
  std::vector<bool> vis(N, false);
  for (int i = 0; i < N; i++) {
    if (vis[i])
      continue;
    components_cnt++;

    std::queue<int> q;
    vis[i] = true;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : adj[cur])
        if (!vis[nxt] && S[nxt] == S[cur]) {
          vis[nxt] = true;
          q.push(nxt);
        }
    }
  }
  return components_cnt;
}

} // namespace

int perform_experiment(std::vector<int> E) {
  calls_cnt++;
  if (calls_cnt > CALLS_CNT_LIMIT)
    quit("Too many calls");
  if ((int)E.size() != N)
    quit("Invalid argument");
  for (int i = 0; i < N; i++)
    if (!(-1 <= E[i] && E[i] <= N))
      quit("Invalid argument");

  std::vector<int> S(N);
  for (int i = 0; i < N; i++)
    S[i] = (E[i] == -1 ? C[i] : E[i]);

  return count_components(S);
}

int main() {
  assert(2 == scanf("%d%d", &N, &M));
  C.resize(N);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &C[i]));
  X.resize(M), Y.resize(M);
  for (int j = 0; j < M; j++)
    assert(2 == scanf("%d%d", &X[j], &Y[j]));
  fclose(stdin);

  adj.assign(N, std::vector<int>());
  for (int j = 0; j < M; j++) {
    adj[X[j]].push_back(Y[j]);
    adj[Y[j]].push_back(X[j]);
  }

  calls_cnt = 0;
  std::vector<int> G = find_colours(N, X, Y);

  int L = G.size();
  printf("%d %d\n", L, calls_cnt);
  for (int i = 0; i < L; i++)
    printf("%s%d", (i == 0 ? "" : " "), G[i]);
  printf("\n");
  fclose(stdout);

  return 0;
}
