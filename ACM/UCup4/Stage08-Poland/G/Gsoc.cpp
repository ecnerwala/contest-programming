#include <bits/stdc++.h>

/* Użycie: 

./Gsoc NAZWA_PLIKU_Z_TESTEM

(input i output użytkownika powinien być podłączony na stdin/stdout, tak
 jak w run.sh).

Format pliku opisującego test (patrz też przykłady G0a.in, G0b.in i G0c.in):
- Pierwszy wiersz zawiera dwie liczby całkowite: N (liczba poziomów
  z których wychodzą rury) i K (liczba wersji każdego poziomu).
- Następne N x K linii zawiera opisy poszczególnych wersji. Pierwsza linia
  zawiera opis wersji 0 poziomu 0, druga opis wersji 1 poziomu 0, trzecia
  wersji 2 poziomu 0; i tak dalej (K+1-wsza linia zawiera opis wersji 0
  poziomu 1).
  Każdy opis składa się z czterech liczb: wersja poziomu i+1 do którego
  prowadzi lewa rura, liczba monet w lewej rurze, wersja poziomu i+1 do
  którego prowadzi prawa rura, liczba monet w prawej rurze.
- Następna linia zawiera Q, liczbę zapytań testowych.
- Nastepne Q linii zawiera opisy zapytań testowych. Każdy opis składa się
  z liczby całkowitej (wersji poziomu 0 od której zaczynamy) i ciągu
  długości N złożonego z liter L i P, opisujący ciąg rur, którym idzie Gary.
*/

#define DEBUG 1

// Liczba poziomów, wersji jednego poziomu, oraz zapytań.
int N, K, Q;
// Ile rozgrywek użył już użytkownik.
int T;
// A[n][k][d] to liczba monet w rurze z poziomu n w wersji k, w kierunku d,
// gdzie d == 0 to L, d == 1 to P.
std::vector<std::vector<std::vector<int>>> A;
// G[n][k][d] to wersja n+1-wszego poziomu do którego prowadzi rura z
// z poziomu n w wersji k, w kierunku d.
std::vector<std::vector<std::vector<int>>> G;
// Zapytania testowe - wersja pierwszego poziomu.
std::vector<int> S;
// Zapytania testowe - ciąg wyborów rur.
std::vector<std::string> R;

bool initialized = false;

void wrong_answer(std::string msg) {
  std::cerr << "WRONG\n";
  std::cerr << msg << "\n";
  exit(1);
}

void ok() {
  std::cerr << "OK\n";
  std::string msg = "Liczba zapytań = " + std::to_string(T) + 
      ", rozmiar planszy to " + std::to_string(N) + "x" + 
      std::to_string(K) + ".";
  std::cerr << msg << "\n";
  exit(0);
}

// Wczytuje z pliku wejściowego dane dotyczące testu.
void init(const char *infile) {
  if (initialized) return;
  T = 0;
  std::ifstream input(infile);
  input >> N >> K;
  assert(!input.fail()); //Błędny format pliku testowego
  A.resize(N);
  G.resize(N);
  for (int n = 0; n < N; ++n) {
    A[n].resize(K);
    G[n].resize(K);
    for (int k = 0; k < K; ++k) {
      A[n][k].resize(2);
      G[n][k].resize(2);
      for (int d = 0; d < 2; ++d) {
        input >> G[n][k][d] >> A[n][k][d];
        assert(!input.fail()); //Błędny format pliku testowego
      }
    }
  }
  input >> Q;
  assert(!input.fail()); //Błędny format pliku testowego
  S.resize(Q);
  R.resize(Q);
  for (int q = 0; q < Q; ++q) {
    input >> S[q] >> R[q];
    assert(!input.fail()); //Błędny format pliku testowego
  }
  initialized = true;
}

// Otrzymuje wersję poziomu startowego i ciąg kierunków, i zwraca liczbę
// monet. Jeśli "count", to ta rozgrywka wlicza się do limitu zapytań
// (i wywalimy się z błędem, jeśli przekroczy limit).
int rozgrywka(int k, const std::string &dirs, bool count) {
  if (count && (T == 300'000)) {
    wrong_answer("Przekroczony limit zapytań, zapytanie numer " + 
                 std::to_string(T+1) + " to " + std::to_string(k) + 
                 ", '" + dirs + "'.");
    return -1;
  }
  if (count) T += 1;
  if (dirs.size() != (unsigned) N) {
    wrong_answer("Niepoprawna długość ciągu kierunków: '" + dirs +
                 "' w zapytaniu " + std::to_string(T) + ".");
    return -1;
  }
  int ans = 0;
  for (int n = 0; n < N; ++n) {
    if (dirs[n] != 'L' && dirs[n] != 'P') {
      wrong_answer("Niepoprawny znak na pozycji " + std::to_string(n) +
                   " w zapytaniu " + std::to_string(T) + ": '" + dirs + 
                   "'.");
      return -1;
    }
    int d = (dirs[n] == 'P');
    ans += A[n][k][d];
    k = G[n][k][d];
  }
  return ans;
}

void interactor() {
  printf("%d %d\n", N, K); fflush(stdout);
  if (DEBUG) std::cerr << "< " << N << " " << K << std::endl;
  while (true) {
    char c;
    std::cin >> c;
    assert(!std::cin.fail()); //Błedny format wyjścia rozwiązania,
    if (DEBUG) std::cerr << "> " << c;
    if (c == '!') { if (DEBUG) { std::cerr << std::endl; } break; }
    assert(c == '?');
    int k;
    std::string query;
    std::cin >> k >> query;
    assert(!std::cin.fail()); //Błedny format wyjścia rozwiązania,
    if (DEBUG) std::cerr << " " << k << " " << query 
                         << "[" << T+1 << "]" << std::endl;
    printf("%d\n", rozgrywka(k, query, true)); fflush(stdout);
    if (DEBUG) std::cerr << "< " << rozgrywka(k, query, false) << std::endl;
  }
  printf("%d\n", Q); fflush(stdout);
  if (DEBUG) std::cerr << "< " << Q << std::endl;
  for (int q = 0; q < Q; ++q) {
    printf("%d %s\n", S[q], R[q].c_str()); fflush(stdout);
    if (DEBUG) std::cerr << "< " << S[q] << " " << R[q] << std::endl;
    int ans;
    std::cin >> ans;
    assert(!std::cin.fail()); //Błedny format wyjścia rozwiązania,
    if (DEBUG) std::cerr << "> " << ans << std::endl;
    if (ans != rozgrywka(S[q], R[q], false)) {
      wrong_answer("W zapytaniu " + std::to_string(q+1) + "(" + 
          std::to_string(S[q]) + " " + R[q] + ") oczekiwano " +
          std::to_string(rozgrywka(S[q], R[q], false)) + ", otrzymano " +
          std::to_string(ans) + ".");
    }
  }
  ok();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Użycie: " << argv[0] << " INPUT_FILENAME" << std::endl;
    exit(1);
  }
  init(argv[1]);
  interactor();
  return 0;
}
