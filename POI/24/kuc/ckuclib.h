int dajn();
int daje();

int jedno(int k);
int dwa(int k);
int polowa(int k);

void odpowiedz(int wynik);

namespace ecnerwala {
bool initialized = false;
int N, E;
std::vector<int> VA;
std::vector<int> VB;
std::vector<int> VC;
void init() {
    if (initialized) return;
    initialized = true;
    std::cin >> N >> E;
    VA.resize(N); for (auto& v : VA) std::cin >> v;
    VB.resize(N); for (auto& v : VB) std::cin >> v;
    VC.resize(N); for (auto& v : VC) std::cin >> v;
}
}

int dajn() {
    ecnerwala::init();
    return ecnerwala::N;
}
int daje() {
    ecnerwala::init();
    return ecnerwala::E;
}
int jedno(int k) {
    ecnerwala::init();
    return ecnerwala::VA[k-1];
}
int dwa(int k) {
    ecnerwala::init();
    return ecnerwala::VB[k-1];
}
int polowa(int k) {
    ecnerwala::init();
    return ecnerwala::VC[k-1];
}
void odpowiedz(int wynik) {
    std::cout << wynik << '\n';
    exit(0);
}
