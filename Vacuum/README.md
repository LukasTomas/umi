## Zadání

Uvažujte svět vysavače, tj. pravidelnou mřížku čtvercovou mřížku, kde každé políčko je
buď prázdné, nebo obsahuje smetí. Dále se v prostředí pohybuje automatický vysavač, který
se vždy nachází na některém políčku mřížky. Vysavač se může pohybovat na sousední políčko
nebo může vysávat, čehož výsledkem je prázdné políčko tam, kde se vysavač vyskytuje.
Ilustrace problému na mřížce obsahující dvě políčka následuje:

1) Definuje stavový prostor, využijte funkci následníka.
2) Navrhněte algoritmus pro nalezení plánu, který vyčistí celou mřížku.
3) Navrhněte algoritmus pro nalezení nejkratšího plánu (co do počtu akcí), který vyčistí
celou mřížku.
4) Implementujte navržené algoritmy.

## Řešení

1) **Vytvoření úplného grafu mezi důležitými políčky**: Pomocí BFS (Breadth-First Search) algoritmu najdu nejkratší cesty mezi všemi důležitými políčky (startovací pole vysavače a všechna políčka s nepořádkem). Na základě těchto cest vytvořím úplný graf, který obsahuje pouze tato důležitá políčka. Tento graf poté poslouží jako základ pro hledání nejlepšího průchodu vrcholy.
2) **Náhodný výběr variace vrcholů**: Náhodně zvolím jednu variaci (permutaci) vrcholů, která představuje možné pořadí, ve kterém vysavač navštíví všechna důležitá políčka.
3) **Vyhodnocení všech variací brute-forcem**: Následně pomocí brute-force algoritmu zkontroluji všechny možné variace vrcholů, abych našel tu, která představuje zaručeně nejkratší cestu a která zahrnuje všechny důležité políčka (včetně políček s nepořádkem).
4) **Implementace**: Tento postup je již implementován.

## Návod

1) **Vytvoření spustitelného souboru** - ```make```
2) **Pro nalezení nějaké cesty** - ```./vacuum```
3) **Pro nalezení nejkratší cesty** - ```./vacuum find-shortest```

### Grafické znázornění stavového prostoru

Stavový prostor je znázorněn:
- *0* - počáteční pole vysavače
- *čislo > 0* - pole nepořádku
- *#* - překážka
- *.* - aktuální poloha vysavače

```
----------------------------------------------------------------
0 |   | # | 1 |   | 2 | # |   |   | # |   | 3 |   |   | 4 |   |
----------------------------------------------------------------
# |   |   |   | # |   |   |   |   | # |   |   |   |   |   |   |
----------------------------------------------------------------
  | # | . | # |   |   | # |   |   |   |   | # |   | # |   |   |
----------------------------------------------------------------
  |   | 5 |   |   | # |   |   |   |   |   | # |   |   | # |   |
----------------------------------------------------------------
# |   |   | # | 6 |   |   |   |   | # |   |   |   |   |   |   |
----------------------------------------------------------------
  | # |   | # |   | 7 |   | # |   |   | # |   |   |   |   |   |
----------------------------------------------------------------
  |   |   |   |   |   | # |   |   |   | 8 |   |   |   |   | # |
----------------------------------------------------------------
# |   |   |   | # |   |   |   |   |   |   | # |   |   | 9 |   |
----------------------------------------------------------------
  |   | # |   |   |   |   |   | # |   |   |   |   |   |   |   |
----------------------------------------------------------------
  | # |   | 10 |   | # |   | # |   |   |   |   |   |   | 11 |   |
----------------------------------------------------------------
```