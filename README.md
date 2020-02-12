# Barnsley fern
Program generujący fraktal - paproć barnsleya.

//TODO Poprawić kod! Błędy: program dla wysokości fraktalu, która nie jest taka sama co szerokośc - wyciek pamięci. Dodatkowo odczyt obrazu przez programy zewnętrzne jest dziwny... pomomo że w notepad++ jest ok.

# Opis programu

Program generuje fraktal "Paproć Barnsleya". Użytkownik może wybrać rozmiar fraktalu z zakresu 0 do 2048 pikseli (wysokość i szerokość). Plik wyjściowy jest zapisywany w formacie PGM. Uruchomienie odbywa się z linii poleceń po wpisaniu odpowiednich przełączników, które mogą być podawane w dowolnej kolejności.

-o [nazwa dla fraktalu (nazwa pliku)]
-h [wysokość generowanego fraktalu]
-w [szerokośc generowanego fraktalu]

Program wyświetla którką instrukcję użytkownikowi po wpisaniu parametru -h.
