from random import *
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import collections as col

global macierz_sasiedztwa
global tabela_x, tabela_y
global graf2


def generator():
    global tabela_x, tabela_y
    global graf
    graf = nx.Graph()
    global macierz_sasiedztwa
    print("Podaj liczbę wierzchołków")
    wierzcholeczki = int(input())
    tabela_x = []
    tabela_y = []
    for i in range(wierzcholeczki):
        templist = []
        losowa = randint(1, int(wierzcholeczki / 2))
        print(f"Losowa: {losowa}")
        for j in range(losowa):
            tabela_x.append(i + 1)
            temp = randint(1, int(wierzcholeczki / 2) + 1)
            print(f"Temp 1: {temp}")

            if temp in templist or i + 1 == temp:
                while temp in templist or i + 1 == temp:
                    temp = randint(1, int(wierzcholeczki / 2) + 1)
                    print(f"Temp 2: {temp}")
            tabela_y.append(temp)
            templist.append(temp)

    for i in range(len(tabela_x)):
        graf.add_edge(tabela_x[i], tabela_y[i])

    macierz_sasiedztwa = [[0 for i in range(wierzcholeczki)] for k in range(wierzcholeczki)]

    for i in range(len(tabela_x)):
        u = tabela_x[i]
        v = tabela_y[i]
        macierz_sasiedztwa[u - 1][v - 1] = 1

    macierz_sasiedztwa_numpy = np.array(macierz_sasiedztwa)
    print(macierz_sasiedztwa_numpy)
    global graf2
    graf2 = nx.from_numpy_matrix(macierz_sasiedztwa_numpy)
   # nx.draw(graf2, with_labels=True)
   # plt.draw()
   # plt.show()
    global liczba_wierzcholkow
    liczba_wierzcholkow = wierzcholeczki


def wczytaj_z_pliku():
    global tablica_x, tablica_y
    s=input()
    plik_wejscie = open(s, "r")
    przejscie1 = True
    global graf
    graf = nx.Graph()
    global macierz_sasiedztwa
    tablica_x = []
    tablica_y = []

    for i in plik_wejscie:
        if przejscie1:
            global liczba_wierzcholkow
            liczba_wierzcholkow = int(i)
            przejscie1 = False
        else:
            para = i.split()
            for k in range(0, len(para)):
                para[k] = int(para[k])
            tablica_x.append(para[0])
            tablica_y.append(para[1])

            graf.add_edge(para[0], para[1])

    macierz_sasiedztwa = [[0 for i in range(liczba_wierzcholkow)] for k in range(liczba_wierzcholkow)]
    print(tablica_x, tablica_y)

    for i in range(len(tablica_x)):
        u = tablica_x[i]
        v = tablica_y[i]
        macierz_sasiedztwa[u - 1][v - 1] = 1

    macierz_sasiedztwa_numpy = np.array(macierz_sasiedztwa)
    print(macierz_sasiedztwa_numpy)
    global graf2
    graf2 = nx.from_numpy_matrix(macierz_sasiedztwa_numpy)
    nx.draw(graf2, with_labels=True)
    plt.draw()
    plt.show()


def zachlanne(graf):
    wynik = {}
    for n in sorted(graf, key=lambda x: len(graf[x]), reverse=True):
        sasiedzi = set(wynik.get(sas) for sas in graf[n])
        wynik[n] = next(kolor for kolor in range(len(graf)) if kolor not in sasiedzi)
    return wynik



def tabu_search(graf, liczba_kolorow, wielkosc_tabu=7, powtorzenia=100, max_iteracji=10000):
    kolory = list(range(liczba_kolorow))
    iteracje = 0

    tabu = col.deque()

    rozwiazanie = dict()
    for i in range(len(graf)):
        rozwiazanie[i] = kolory[randrange(0, len(kolory))]

    poziom_aspiracji = dict()

    while iteracje < max_iteracji:

        przemieszczenie = set()
        liczba_konfliktow = 0
        for i in range(len(graf)):
            for j in range(i + 1, len(graf)):
                if graf[i][j] > 0:
                    if rozwiazanie[i] == rozwiazanie[j]:
                        przemieszczenie.add(i)
                        przemieszczenie.add(j)
                        liczba_konfliktow += 1
        przemieszczenie = list(przemieszczenie)

        if liczba_konfliktow == 0:
            break

        nowe_rozwiazanie = None
        for r in range(powtorzenia):
            wierzcholek = przemieszczenie[randrange(0, len(przemieszczenie))]

            nowy_kolor = kolory[randrange(0, len(kolory) - 1)]
            if rozwiazanie[wierzcholek] == nowy_kolor:
                nowy_kolor = kolory[-1]

            nowe_rozwiazanie = rozwiazanie.copy()
            nowe_rozwiazanie[wierzcholek] = nowy_kolor

            nowe_konflikty = 0
            for i in range(len(graf)):
                for j in range(i + 1, len(graf)):
                    if graf[i][j] > 0 and nowe_rozwiazanie[i] == nowe_rozwiazanie[j]:
                        nowe_konflikty += 1
            if nowe_konflikty < liczba_konfliktow:
                if nowe_konflikty <= poziom_aspiracji.setdefault(liczba_konfliktow, liczba_konfliktow - 1):
                    poziom_aspiracji[liczba_konfliktow] = nowe_konflikty - 1

                    if (wierzcholek, nowy_kolor) in tabu:
                        tabu.remove((wierzcholek, nowy_kolor))
                else:
                    if (wierzcholek, nowy_kolor) in tabu:
                        continue
                break

        tabu.append((wierzcholek, rozwiazanie[wierzcholek]))
        if len(tabu) > wielkosc_tabu:
            tabu.popleft()

        rozwiazanie = nowe_rozwiazanie
        iteracje += 1

    if liczba_konfliktow != 0:
        print("Nie można pokolorować grafu używając {} kolorów".format(liczba_kolorow))
        return 0
    else:
        print("Znaleziono kolorowanie:\n", rozwiazanie)
        return rozwiazanie


print("Wybierz co chcesz zrobić")
print("1. Wczytać z pliku")
print("2. Wygenerować losowy graf")
wybor1 = int(input())
if wybor1 == 1:
    wczytaj_z_pliku()
    print(liczba_wierzcholkow)
elif wybor1 == 2:
    generator()
print(liczba_wierzcholkow)
print("Wybierz co zrobić dalej")
print("1. Tabu search")
print("2. Zachłanne")
wybor2 = int(input())
if wybor2 == 1:
    for i in range(liczba_wierzcholkow, 0, -1):
        print(i)
        x = tabu_search(macierz_sasiedztwa, i)
        if x == 0:
            break
elif wybor2 == 2:
    print(zachlanne(graf2))
