//
//
// Compilable on C++11 or higher
//
// This implementation prints the correct answer within finite time, but will get "Time Limit Exceeded" verdict upon submission.
//
//
#include <iostream>
#include <cassert>
#include <array>
#include <vector>

// Empty sum returns the additive identity.
int add(int M){ return 0; }
// Returns x + "sum of ys" mod M.
template<class... Ts>
int add(int M, int x, Ts... ys){
  x += add(M, ys...);
  if(x >= M) x -= M;
  return x;
}
// Returns x - y mod M.
int sub(int M, int x, int y){
  x -= y;
  if(x < 0) x += M;
  return x;
}
// Empty product returns the multiplicative identity.
int mul(int M){ return 1; }
// Returns x * "product of ys" mod M.
template<class... Ts>
int mul(int M, int x, Ts... ys){ return 1LL * x * mul(M, ys...) % M; }
// Returns the multiplicative inverse of x mod M
int inv(int M, int x){
  int y = 1;
  for(auto e = M - 2; e; e >>= 1){
    if(e & 1) y = mul(M, y, x);
    x = mul(M, x, x);
  }
  return y;
}

// Check if _X = C(L, A, I) within F(M, E, V) is valid.
bool valid(int M, int E, int V, std::array<int, 3> _X){
  int L = _X[0], A = _X[1], I = _X[2];
  if(L < 0 || A < 0 || I < 0 || L >= M || A >= M || I >= M){
    return false;
  }
  if(L == 0 && (A != (M - I) % M || A == I)){
    return false;
  }
  const int minus = M - 1; // -1 mod M
  return add(M,
    mul(M, A, A, A),
    mul(M, minus, A, A, L),
    mul(M, 3, A, A, I),
    mul(M, E, A, L, L),
    mul(M, L, L, L, V),
    mul(M, 2, A, L, I),
    mul(M, E, L, L, I),
    mul(M, 3, A, I, I),
    mul(M, minus, L, I, I),
    mul(M, I, I, I)
  ) == 0;
}

// Returns {L2, A2, I2} where
// C(L_2, A_2, I_2) = Combine(_X = C(L_0, A_0, I_0), _Y = C(L_1, A_1, I_1))
// within F(M, E, V).
std::array<int, 3> combine(int M, int E, int V, std::array<int, 3> _X, std::array<int, 3> _Y){
  assert(valid(M, E, V, _X) && valid(M, E, V, _Y));
  int L0 = _X[0], A0 = _X[1], I0 = _X[2];
  int L1 = _Y[0], A1 = _Y[1], I1 = _Y[2];
  if(L1 == 0) return {L0, A0, I0};
  if(L0 == 0) return {L1, I1, A1};
  int B0 = mul(M, add(M, A0, I0), L1), B1 = mul(M, add(M, I1, A1), L0);
  int C0 = mul(M, sub(M, A0, I0), L1), C1 = mul(M, sub(M, I1, A1), L0);
  if(B0 == B1){
    if(add(M, C0, C1) == 0) return {0, 3, M - 3};
    int Sum = add(M, A0, I0), Dif = sub(M, A0, I0);
    int B = add(M, mul(M, 3, Sum, Sum), mul(M, E, L0, L0));
    int C = mul(M, 2, Dif, L0), D = mul(M, 2, C, Sum, Dif);
    int E = sub(M, mul(M, B, B), mul(M, 2, D));
    int X = mul(M, C, E);
    int Y = sub(M, mul(M, B, sub(M, D, E)), mul(M, 2, C, C, Dif, Dif));
    return {mul(M, 2, C, C, C), add(M, X, Y), sub(M, X, Y)};
  }
  else{
    int B = sub(M, B0, B1), B2 = mul(M, B, B), B3 = mul(M, B, B, B);
    int C = sub(M, C0, C1), D = mul(M, L0, L1);
    int E = sub(M, mul(M, C, C, D), mul(M, B2, add(M, B0, B1)));
    int X = mul(M, B, E);
    int Y = sub(M, mul(M, C, sub(M, mul(M, B0, B2), E)), mul(M, C0, B3));
    return {mul(M, 2, B3, D), add(M, X, Y), sub(M, X, Y)};
  }
}

int main(){
  std::cin.tie(0)->sync_with_stdio(0); // Untie std::cin from std::cout
  std::cin.exceptions(std::ios::badbit | std::ios::failbit); // Raise exception for invalid inputs
  int M, E, V, N, Q;
  std::cin >> M >> E >> V >> N;
  std::vector<std::array<int, 3>> C(N); // Initial array
  for(auto i = 0; i < N; ++ i){
    for(auto j = 0; j < 3; ++ j) std::cin >> C[i][j];
    assert(valid(M, E, V, C[i])); // Initial cells are valid
  }
  std::cin >> Q;
  for(auto qi = 0; qi < Q; ++ qi){
    int type;
    std::cin >> type;
    if(type == 1){
      int i, L, A, I;
      std::cin >> i >> L >> A >> I;
      std::array<int, 3> X{L, A, I};
      assert(valid(M, E, V, X)); // Updated cells are valid
      C[i] = X;
    }
    else{
      int l, r;
      std::cin >> l >> r;
      std::array<int, 3> res = C[l];
      for(auto i = l + 1; i < r; ++ i){
        res = combine(M, E, V, res, C[i]);
        assert(valid(M, E, V, res)); // It can be proved that the result is valid
      }
      if(res[0] == 0) std::cout << "-1\n"; // Null State
      else std::cout << mul(M, res[1], res[2], inv(M, mul(M, res[0], res[0]))) << "\n"; // Positive State
    }
  }
  return 0;
}