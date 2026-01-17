# Impact-Bastion

Gra na zaliczenie z Podstaw Informatyki



Jak uruchomić grę?:

<strong>WINDOWS:</strong>

&nbsp;	1.Jeśli nie posiadasz zainstalowanego pakietu redystrybucyjnego Visual C++, to go zainstaluj.

&nbsp;	Możesz pobrać go z tej strony: https://learn.microsoft.com/pl-pl/cpp/windows/latest-supported-vc-redist?	view=msvc-170

&nbsp;	2.Przejdź do folderu x64/Release i uruchom plik impact\_bastion.exe

<strong>LINUX:</strong>

&nbsp; 1. Przejdź do katalogu gry /impact_bastion/ i otwórz terminal w tym miejscu

&nbsp; 2. Zastosuj polecenie: 

g++ *.cpp -o impact-bastion -std=c++17   -I../SFML-3.0.2/include   -L../SFML-3.0.2/lib   -Wl,-rpath,../SFML-3.0.2/lib   -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

&nbsp; Ewentualnie możesz uruchomić skrypt linux_compile.sh który zrobi to za ciebie

&nbsp; 3. Uruchom program impact-bastion w eksploratorze albo poleceniem:

./impact-bastion
