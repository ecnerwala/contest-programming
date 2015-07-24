// Biblioteka message sluzy do przekazywania wiadomosci pomiedzy instancjami
// programu. Wiadomosc moze skladac sie z dowolnej liczby znakow (char) oraz
// liczb typu int i long long. Maksymalna liczba wiadmosci, jakie moze
// wyslac pojedyncza instancja, to 1000, a ich laczny rozmiar nie moze
// przekraczac 8 MB.

#ifndef MESSAGE_H_
#define MESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

// Zwraca liczbe instancji.
// Pascal: function NumberOfNodes:longint;

int NumberOfNodes();

// Zwraca ID biezacej instancji, z przedzialu {0, ..., NumberOfNodes()-1}.
// Pascal: function MyNodeId:longint;
int MyNodeId();

// Kolejkuje `value` do wyslania do instancji `target`.
// Pascal: procedure PutChar(target:longint; value:shortint);
void PutChar(int target, char value);

// Kolejkuje `value` do wyslania do instancji `target`.
// Pascal: procedure PutInt(target:longint; value:longint);
void PutInt(int target, int value);

// Kolejkuje `value` do wyslania do instancji `target`.
// Pascal: procedure PutLL(target:longint; value:int64);
void PutLL(int target, long long value);

// Wysyla zakolejkowana wiadomosc do instancji `target`. Powrot z funkcji
// nastepuje natychmiast, nie czekajac na odbior wiadomosci.
// Pascal: procedure Send(target:longint);
void Send(int target);

// Odbiera wiadomosc od instancji `source` (lub dowolnej, gdy `source` == -1).
// Zwraca numer instancji, od ktorej wiadomosc odebral. Receive wymaga, aby w
// momencie jego wywolania poprzednio odebrana wiadomosc od instancji `source`
// (lub wszystkie poprzednio odebrane wiadomosci, gdy `source` == -1) byla
// w calosci przeczytana.
// Pascal: function Receive(source:longint):longint;
int Receive(int source);

// Po odebraniu wiadomosci jej zawartosc nalezy czytac w takiej kolejnosci,
// w jakiej byla kolejkowana do wyslania. Np. proba odczytania int-a, podczas
// gdy pierwsza zakolejkowana wartosc byl long long, skonczy sie bledem
// wykonania (jesli program byl kompilowany z opcja --debug) lub spowoduje
// niezdefiniowane zachowanie (bez --debug).

// Po odebraniu wiadomosci jej zawartosc nalezy czytac w takiej kolejnosci,
// w jakiej byla kolejkowana do wyslania. Na przyklad, gdy nadawca jako pierwsza
// wartosc zakolejkowal long longa, a odbiorca sprobuje przeczytac chara,
// program zakonczy sie bledem wykonania (jesli byl kompilowany z opcja --debug)
// lub zachowa sie w niezdefiniowany sposob (bez --debug).

// Czyta char z odebranej wiadomosci od instancji `source`. Numer instancji
// musi byc liczba z przedzialu {0, ..., NumberOfNodes()-1}. W szczegolnosci
// nie moze byc rowny -1.
// Pascal: function GetChar(source:longint):shortint;
char GetChar(int source);

// Czyta int z odebranej wiadomosci od instancji `source`.
// Pascal: function GetInt(source:longint):longint;
int GetInt(int source);

// Czyta long long z odebranej wiadomosci od instancji `source`.
// Pascal: function GetLL(source:longint):int64;
long long GetLL(int source);

#ifdef __cplusplus
}
#endif

#endif  // MESSAGE_H_
