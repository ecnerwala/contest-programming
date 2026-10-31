program kol;

uses pkollib;

var
  wynik : LongInt;
  k : LongInt;

begin
	while true do begin
    k := karta;
    if k = 0 then begin
      Break;
    end;
    wynik := k;
  end;
  odpowiedz(wynik);
end.
