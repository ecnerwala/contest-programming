unit pkollib;

interface
	function karta : Longint;
	procedure odpowiedz(wynik : Longint);

implementation

var
  N : Longint;
  indeks : Longint;
  licznik_zapytan : Longint;
  bufor : array of Longint;

procedure blad (wiadomosc : string);
begin
  Writeln('BLAD! ', wiadomosc);
  Halt(0);
end;

procedure wczytaj_talie;
var
  i : LongInt;
begin
  Read(N);
  if N <= 0 then begin
    blad('Liczba kart w talii powinna byc dodatnia.');
  end;
  SetLength(bufor, N);
  for i := 0 to (N - 1) do begin
    Read(bufor[i]);
    if (bufor[i] <= 0) or (bufor[i] > 2000 * 1000 * 1000) then begin
      blad('Numer bajtemona spoza dozwolonego przedzialu.');
    end;
  end;
end;

function karta : Longint;
begin
  Inc(licznik_zapytan);
  if licznik_zapytan = 2 * N + 3 then begin
    blad('Przekroczaono limit wywolan funkcji karta().');
  end;
  if indeks = N then begin
    indeks := 0;
    karta := 0;
  end else begin
    karta := bufor[indeks];
    Inc(indeks)
  end;
end;

procedure odpowiedz (wynik : Longint);
var
  przebiegow : Longint;
begin
  przebiegow := (licznik_zapytan + N) div (N + 1);
  WriteLn('Udzielona odpowiedz: ', wynik);
  WriteLn('Liczba wywolan funkcji karta: ', licznik_zapytan);
  WriteLn('Liczba rozpoczetych przebiegow: ', przebiegow);
  Halt(0);
end;

begin
  wczytaj_talie;
  indeks := 0;
  licznik_zapytan := 0;
end.
