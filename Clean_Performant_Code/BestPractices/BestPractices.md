# C++ Performance Optimierung: Best Practices

[Zurück](../../Readme.md)

---

## Inhalt

  * [Vermeiden Sie unnötiges Kopieren](#link1)
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

## Vermeiden Sie unnötiges Kopieren <a name="link1"></a>

Das Kopieren von Objekten kann teuer sein.

Verwenden Sie beim Übergeben und Zurückgeben von Objekten Referenzen oder Verschiebesemantik (`std::move`),
um unnötiges Kopieren zu minimieren.

Hat man bei Parametern die Wahl zwischen `const std::string&` und `std::string_view`,
so sollte man `std::string_view` verwenden. Nicht immer,
aber häufig kann in diesem Fall die Performance besser sein.

---

## Bevorzuge Stack Allocation <a name="link2"></a>

Legen Sie Objekte nach Möglichkeit auf dem Stapel an,
da die Stack Allocation schneller ist als eine dynamische
Speicherplatzreservierung auf der Halde (*Heap*).

Verwenden Sie die dynamische Speicherplatzreservierung (`new` and `delete`) nur dann,
wenn die Lebensdauer des Objekts über den aktuellen Scope hinausgehen soll.

Es ist jedoch wichtig zu beachten, dass die Stack Allocation Einschränkungen hat:

  * Feste Größe:<br />
    Die Größe des Stapelspeichers ist fest und begrenzt.
    Das bedeutet, dass man keine sehr großen Objekte oder eine dynamische Anzahl von Objekten auf dem Stapel anlegen kann.

  * Das Risiko eines *Stack Overflow*:<br />
    Übermäßiger Stapelspeicherverbrauch kann zu einem Stapelüberlauf (*Stack Overflow*) führen,
    wenn der verfügbare Stapelspeicherplatz erschöpft ist.
    Der Heap-Speicher kennt diese Einschränkung nicht.

---

## Schleifen optimieren <a name="link3"></a>

Schleifen bilden oft den Kern von Algorithmen.

Optimieren Sie Schleifen, indem Sie den Overheader von Schleifen,
unnötige Berechnungen reduzieren und die richtigen Schleifenkonstrukte verwenden
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

[Zurück](../../Readme.md)

Die Anregungen zum konzeptionellen Beispiel finden Sie unter

[https://hackernoon.com/c-performance-optimization-best-practices](C++ Performance Optimization: Best Practices)

---
