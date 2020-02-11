// Paproc.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>

void help();
int toInt(std::string w);
void save(const std::string& NAME, char** space, int w, int h);
void barnsley(double parameters[4][6], char** space, int probability[4], int w, int h);
void initialization(int w, int h, char** space);

/*
Glowna funkcja programu. Przechowuje funkcje i parametry odpowiedzialne za jego dzialanie oraz obsluguje parametry wprowadzone z linii polecen.

parametry funkcji:
__________________
argc - liczba calkowita informujaca o ilosci wprowadzonych argumentow z linii polecen
*argv[] - tablica przechowujaca znaki, zawiera sciezke wywolania i podane parametry

wartosc zwracana:
_________________
0 - po wykonaniu wszystkich zadan funkcja 'int main' zakancza dzialanie programu zwracajac calkowita wartosc

uwagi:
_____
w = 0, h = 0 - kolejno szerokosc i wysokosc generowanej paproci, na samym poczatku nadane wartosci wynosza 0
H = "-h" - zmienna przechowujaca tekst, w zaleznosci od tego czy uzytkownik wprowadzil sam parametr '-h' jej rola zmienia sie do parametru odpowiedzialnego za pomoc lub rozmiar
WIDTH = "-w" - zmienna przechowujaca tekst w postaci szerokosci paproci
FILE = "-o" - zmienna przechowujaca tekst w postaci nazwy pliku do ktorego ma byc zapisany fraktal
NAME = "" - zmienna przechowujaca tekst, pomomaga przy okresleniu czy uzytkownik wprowadzil nazwe
parameters[4][6] - tablica przechowujaca wartosci parametrow funkcji odpowiedzialnych za tworzenie paproci barnsleya
probability[4] - tablica przechowujaca liczby odpowiadajace za dane prawdopodobienstwo wybierania funkcji iterowanych
S - zmienna calkowita przechowywujaca wysokosc i szerokosc tablicy
space[S][S] - tablica statyczna dwuwymiarowa, wyznacza obszar na ktorym bedzie rysowany fraktal

autor: Pawel Salicki
2017-11-30
*/

int main(int argc, char* argv[])
{
	int w = 0, h = 0;
	const std::string H = "-h";
	const std::string WIDTH = "-w";
	const std::string FILE = "-o";
	std::string NAME = "";

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i] == H && i == argc)
			{
				help();
			}
			else
			{
				if (argv[i] == FILE)
				{
					NAME = argv[++i];
				}
				else if (argv[i] == H)
				{
					h = toInt(argv[++i]);
				}
				else if (argv[i] == WIDTH)
				{
					w = toInt(argv[++i]);
				}
			}
		}
	}


	if (NAME == "")
	{
		help();
		return 0;
	}

	if (w <= 0 || h <= 0)
	{
		help();
		return 0;
	}

	if (w > 2048) 
	{
		w = 2048;
	}

	if (h > 2048)
	{
		h = 2048;
	}

	double parameters[4][6] = {
		{ 0.85, 0.04, 0.0, -0.04, 0.85, 1.6 },
		{ -0.15, 0.28, 0.0, 0.26, 0.24, 0.44 },
		{ 0.1, -0.26, 0.0, 0.23, 0.22, 1.6 },
		{ 0.0, 0.0, 0.0, 0.0, 0.16, 0.0 }
	};

	char** space = new char* [h];
	for (int i = 0; i < h; ++i) {
		space[i] = new char[w];
	}

	int probability[4] = { 78, 20, 21, 5 };

	initialization(w, h, space);
	barnsley(parameters, space, probability, w, h);
	save(NAME, space, w, h);

	for (int i = 0; i < h; ++i) {
		delete[] space[i];
	}
	delete[] space;

	return 0;
}
/*
Funkcja odpowiedzialna za wypisywanie tekstu pomocy po wykryciu bledu podczas typowania parametrow, badz wywolana parametrem "-h".
Zawiera w sobie tylko tekst, ktory jest wyswietlany uzytkownikowi.

autor: Pawel Salicki
2017-11-30
*/

void help()
{
	std::cout << "Program generujacy fraktal Barnsleya." <<
		std::endl << "By wygenerowac fraktal wpisz:" <<
		std::endl << "-o [Wpisz tutaj nazwe pliku wyjsciowego]" <<
		std::endl << "-w [Wpisz tutaj szerokosc generowanego fraktala. Max 2048.]" <<
		std::endl << "-h [Wpisz tutaj wysokosc generowanego fraktala. Max 2048.]" <<
		std::endl;
}

/*
Funkcja konwertujaca wprowadzone parametry (rozmiary fraktala) na liczby.
Ponadto sprawdza zakres wielkosci i szerokosci.

parametry funkcji:
_________________
w		-rozmiar fraktala, dotyczy jego wysokosci i szerokosci

wartosc zwracana:
________________
keep	- podano poprawne parametry, przechowuje w sobie rozmiar wprowadzonej wysokosci i szerokosci paproci
0	- przekroczono dozwolony zakres rozmiaru paproci;

uwagi:
______
max_size			-przechwuje dozwolona maksymalna wielkosc generowanego fraktalu
keep				-zmienna przechowywujaca rozmiar
istringstream ss(w)		-strumien napisowy, konwertuje wrowadzony rozmiar w postaci tekstu na licbe

autor: Pawel Salicki
2017-11-30
*/

int toInt(std::string w)
{
	const int maxSize = 2048;
	int keep;
	std::istringstream ss(w);

	ss >> keep;

	if (keep <= maxSize)
		return keep;

	return 0;
}

/*
Funkcja zapisujaca wygenerowany fraktal do pliku. Po zapisie powiadamia uzytkownika o zakonczeniu programu.

parametry funkcji:
_________________
NAME - wprowadzona nazwa, ktora ma byc nazwany zapisany plik
space - tablica przechowywujaca przestrzen zainicjowana znakami '0'
w - szerokosc paproci
h - wysokosc paproci

wartosc zwracana:
________________
brak

uwagi:
_____
magic_number						- przechowuje format zapisu grafiki rastorowej
Ofstream						- klasa sluzaca do obslugi wyjscia plikowego
f							- zmienna plikowa przy pomocy ktorej dokonuje zmian na pliku
color							- zmienna calkowita przechowywujaca wartosc koloru paproci
f.close()						- zamyka plik i zwalnia bufor
f.write(reinterpret_cast<const char*> (tablica[i]), w) 	- zmienna plikowa zapisywana do pliku poprzez konwersje przez zmiane typow, ktore sa niepewne lub zalezne od implementacji

autor: Pawel Salicki
2017-11-30
*/

void save(const std::string& NAME, char **space, int w, int h)
{
	const std::string magic_number = "P5\n";
	int color = 255;
	
	std::string fileName = NAME + ".pgm";

	std::ofstream f(fileName.c_str(), std::ios_base::binary);
	f << magic_number << w << " " << h << "\n" << color << "\n";

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			f << space[i][j];
		}
		f << "\n";
	}

	f.close();

	std::cout << "Generowanie fraktalu Barnsleya zakonczone sukcesem." <<
		std::endl << "Zapisany plik znajduje sie w folderze z programem.";
}


/*
Funkcja jest odpowiedzialna za algorytm generowania paproci Barnsleya.
Zostaly w niej zaimplementowane funkcje odpowiedzialne za losowanie pseudolosowych liczb oraz przypisywanie do tablicy space 255, ktory jest odpowiedzialny za wyswietlanie punktu paproci.
Wylosowane wartosci zostaja przyrownane do wartosci znajdujacych sie w tablicy przychowywujacej prawdopodobienstwo.

parametry funkcji:
_________________
parameters - tablica dwuwymiarowa przechowywujaca parametry funkcji iterowanych odpowiedzialnych za fraktal
space - tablica dwuwymiarowa przechowywujaca wielkosc na ktorej bedzie generowany fraktal, jest wypelniona znakami '0'
probability - tablica jednowymiarowa przechowywujaca wartosci prawdopodobienstwa w danych proporcjach
w - zmienna przechowywujaca szerokosc fraktala
h - zmienna przechowywujaca wysokosc fraktala

wartosc zwracana:
________________
brak

uwagi:
_____
chance - zmienna calkowita pomocnicza, nadawana jej jest wartosc losowa oraz jest porownywana z wartosciami zawartymi w tablicy 'probability'
inxex - zmienna calkowita uzyta do przyporzadkowania indeksu tablicy odpowiedniego parametru
I - zmienna przechowywujaca liczbe iteracji w petli for
widthX - szerokosc fratalu
heightY - wysokosc fraktalu
space[(int)((x - minX) / ratioX)][(int)((y - minY) / ratioY)] = (char)255; - tablica przechowywujaca losowe punkty x i y, nadaje im znak 255 w tablicy ASCII, ktory jest widoczny w formacie pgm

autor: Pawel Salicki
2017-11-30
*/

void barnsley(double parameters[4][6], char** space, int probability[4], int w, int h)
{
	int index = 0;
	const int I = 200000;
	double x = 0, y = 0, x0 = 0, y0 = 0, chance = 0;
	double granica_left = 1000, granica_right = -1000, granica_up = -1000, granica_down = 1000;

	double* punktyX{ new double[I] {} };
	double* punktyY{ new double[I] {} };

	std::default_random_engine engine;
	std::uniform_real_distribution<double> distribution(0, 100);

	auto czas = std::chrono::system_clock::now();
	auto interwal = czas.time_since_epoch();
	engine.seed(interwal.count());

	for (int i = 0; i < I; i++)
	{
		chance = distribution(engine);
		if (chance <= probability[3])
			index = 3;
		else if (chance <= probability[2])
			index = 2;
		else if (chance <= (probability[2] + probability[3]))
			index = 1;
		else
			index = 0;

		x0 = parameters[index][0] * x + parameters[index][1] * y + parameters[index][2];
		y0 = parameters[index][3] * x + parameters[index][4] * y + parameters[index][5];

		x = x0;
		y = y0;

		punktyX[i] = x;
		punktyY[i] = y;

		if (x > granica_right)
			granica_right = x;
		if (x < granica_left)
			granica_left = x;
		if (y < granica_down)
			granica_down = y;
		if (y > granica_up)
			granica_up = y;
	}

	granica_right += 0.001;
	granica_left -= 0.001;
	granica_up += 0.001;
	granica_down -= 0.001;

	for (int i = 0; i < I; i++)
	{
		space[(int)((punktyX[i] - granica_left) / ((granica_right - granica_left) / (w * 1.0)))][(int)((punktyY[i] - granica_down) / (granica_up / (h * 1.0)))] = (char)255;
	}

	delete[] punktyX;
	delete[] punktyY;
}

/*
Funkcja odpowiedzialna za wypelnienie statycznej tablicy znakami '0'

parametry funkcji:
__________________
w - wprowadzona szerokosc fraktalu
h - wprowadzona wysokosc fraktalu
sppace - statyczna tablica dwuwymiarowa, przechowuje pole na ktorym bedzie generowany fraktal

wartosc zwracana:
________________
brak

uwagi:
_____
Zastosowano petle w petli by wypelnic tablice znakami '0'

autor: Pawel Salicki
2017-11-30
*/


void initialization(int w, int h, char** space)
{
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
			space[i][j] = '0';
	}
}