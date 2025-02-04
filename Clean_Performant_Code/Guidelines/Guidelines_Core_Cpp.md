# Guidelines Core C++

[Zur�ck](Guidelines.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link19)

---

## Allgemeines <a name="link1"></a>

---

## Klasse (`class`) oder Struktur (`struct`)

#### Klasse (`class`) oder Struktur (`struct`)?

Eine Richtlinie stammt von Bjarne Stroustrup *himself*:

  * Verwenden Sie Klasse (`class`), wenn es *Invarianten* gibt.
  * Verwenden Sie Strukturen (`struct`), wenn alle Datenelemente sich unabh�ngig voneinander ver�ndern k�nnen.

Es bleibt nur noch zu kl�ren, was man unter einer *Invarianten* versteht?

#### Klasse (`class`) oder Struktur (`struct`)?

Wenn ein (oder mehrere Elemente) mit `private` zu kennzeichnen sind, sollte man `class` verwenden.

#### Interfaces (Schnittstellen) wozu?

*Interfaces* stellen nicht nur eine Beschreibung von Methoden dar, sie bewirken damit
auch eine *Hervorhebung*.

Zur Implementierung in einer Klasse wird ein Bezug zur Schnittstelle gegeben.

In einer Klasse lassen sich Daten kapseln, die relevanten Methoden der Schnittstelle
sind hervorgehoben.

#### Minimieren Sie die Sichtbarkeit von Mitgliedern einer Klasse

Ohne Worte :)



---

[Zur�ck](Guidelines.md)

---

*Beispiel*:

```cpp
```


*Ausgabe*:

```
```
