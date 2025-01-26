# C++ Performance Optimierung: Best Practices

[Zur�ck](../../Readme.md)

---

## Inhalt

  * [Vermeiden Sie unn�tiges Kopieren](#link1)
  * [Bevorzuge Stack Allocation](#link2)
  * [Schleifen optimieren](#link3)
  * [Reduzieren Sie Funktionsaufrufe](#link4)
  * [XXXX](#link)
  * [XXXX](#link)
  * [XXXX](#link)
  * [Literatur](#link7)

---

#### Quellcode

[*BestPractices.cpp*](BestPractices.cpp).<br />

---

## Vermeiden Sie unn�tiges Kopieren <a name="link1"></a>

Das Kopieren von Objekten kann teuer sein.

Verwenden Sie beim �bergeben und Zur�ckgeben von Objekten Referenzen oder Verschiebesemantik (`std::move`),
um unn�tiges Kopieren zu minimieren.

Hat man bei Parametern die Wahl zwischen `const std::string&` und `std::string_view`,
so sollte man `std::string_view` verwenden. Nicht immer,
aber h�ufig kann in diesem Fall die Performance besser sein.

---

## Bevorzuge Stack Allocation <a name="link2"></a>

Legen Sie Objekte nach M�glichkeit auf dem Stapel an,
da die Stack Allocation schneller ist als eine dynamische
Speicherplatzreservierung auf der Halde (*Heap*).

Verwenden Sie die dynamische Speicherplatzreservierung (`new` and `delete`) nur dann,
wenn die Lebensdauer des Objekts �ber den aktuellen Scope hinausgehen soll.

Es ist jedoch wichtig zu beachten, dass die Stack Allocation Einschr�nkungen hat:

  * Feste Gr��e:<br />
    Die Gr��e des Stapelspeichers ist fest und begrenzt.
    Das bedeutet, dass man keine sehr gro�en Objekte oder eine dynamische Anzahl von Objekten auf dem Stapel anlegen kann.

  * Das Risiko eines *Stack Overflow*:<br />
    �berm��iger Stapelspeicherverbrauch kann zu einem Stapel�berlauf (*Stack Overflow*) f�hren,
    wenn der verf�gbare Stapelspeicherplatz ersch�pft ist.
    Der Heap-Speicher kennt diese Einschr�nkung nicht.

---

## Schleifen optimieren <a name="link3"></a>

Schleifen bilden oft den Kern von Algorithmen.

Optimieren Sie Schleifen, indem Sie den Overheader von Schleifen,
unn�tige Berechnungen reduzieren und die richtigen Schleifenkonstrukte verwenden
(z. B. bereichsbasierte Schleifen / *range-based for loop*).

---

## Reduzieren Sie Funktionsaufrufe <a name="link4"></a>

Minimieren Sie Funktionsaufrufe innerhalb enger Schleifen.

Durch *Inline*-Funktionen (z. B. mithilfe von `inline`- oder Compileroptimierungen)
kann der Funktionsaufruf-Overhead eliminiert werden.

---

## XXXX <a name="link"></a>


---

## Literatur <a name="link6"></a>

---

[Zur�ck](../../Readme.md)

Die Anregungen zum konzeptionellen Beispiel finden Sie unter

[https://hackernoon.com/c-performance-optimization-best-practices](C++ Performance Optimization: Best Practices)

---
