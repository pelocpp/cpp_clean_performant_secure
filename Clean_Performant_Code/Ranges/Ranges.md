# std::ranges` Bibliothek

[Zurück](../../Readme.md)

---

## Inhalt

  * [XXXX](#link)
  * [Vermeiden Sie unnötiges Kopieren](#link1)
  * [Bevorzuge Stack Allocation](#link2)
  * [Schleifen optimieren](#link3)
  * [Reduzieren Sie Funktionsaufrufe](#link4)
  * [Vermeiden Sie *Raw*-Schleifen](#link5)
  * [XXXX](#link)
  * [XXXX](#link)
  * [XXXX](#link)
  * [Literatur](#link7)

---

#### Quellcode

[*BestPractices.cpp*](BestPractices.cpp).<br />

---

## Allgemeines <a name="link1"></a>

Wir geben in diesem Tutorial keinen vollständigen Überblick über dir `std::ranges` Bibliothek,
sondern vielmehr einige erste Beispiele für Neueinsteiger in modernes C++20 liefern.

An Hand einer Reihe von Beispielen zeigen wir auf, welche Rolle der Einsatz dieser Bibliothek
*Modern C++* spielt.

Erfahren Sie, wie Sie `std::ranges` für besser lesbaren und effizienteren Code nutzen,
also für *Clean Code*.

---

## Was ist ein *Range* <a name="link2"></a>

Ein *Range* (*Bereich*) ist ein Konzept, das die Anforderungen an einen Typ definiert,
der eine Iteration über seine Elemente zulässt.

Ein gültiger Bereich sollte zwei Methoden `begin()` und `end()` bereitstellen.

Im Gegensatz zur Definition des Iteratoren-Konzepts in C++ müssen die Rückgabetypen
dieser beiden Methoden nicht identisch sein, dazu aber später mehr.

*Beispiele*:<br />
`std::array`, `std::vector`, `std::string_view`, `std::span`, Array im C-Stil usw.

*Vorraussetzungen*:<br />

```cpp
#include <ranges>
```

Die Datentypen der `std::ranges` Bibliothek liegen im Namensraum `std::ranges`.

---

## *Ranges* versus traditionelle Iteratoren im Vergleich <a name="link2"></a>

Traditionelle STL-Algorithmen verwenden Iteratorpaare zur Bezeichnung von Bereichen.

Dies kann fehleranfällig sein und ist darüber hinaus etwas &bdquo;*verbose*&rdquo;.


*Beispiel*:

```cpp
01: std::vector<int> numbers{ 1, 4, 2, 7, 9, 3, 5 };
02: 
03: std::sort(
04:     numbers.begin(), 
05:     numbers.end()
06: );
```

*Beispiel*:

```cpp
std::vector<int> numbers{ 1, 4, 2, 7, 9, 3, 5 };

std::ranges::sort(numbers);
```


---

## Ranges und Concepts (Konzepte) <a name="link2"></a>

Der Gebrauch von Range Concepts trägt dazu bei, bessere und verständlichere Fehlermeldungen zu generieren.
Die Fehlermeldungen, die bei klassischem C++ und der STL erscheinen,
sind manchmal absolut unverständlich und sie treten vor allem in Dateien in Erscheinung,
die korrekt sind. Der Fehler ist im Regelfall in der Nähe der Template Instanziierung,
was für einen C++ Compiler zunächst einmal nicht die Fehlerursache ist.
Das reagierde Verhalten im Falle von Fehler tritt an anderen Stellen zu Tage.

Bei Gebrauch von Range Concepts haben wir es mit einem proaktiven Verhalten zu tun,
es wird die Stelle der Template Instanziierung in Übereinstimmung mit dem Concept analyisiert,
im Fehlerfall erfolgt die Fehlermeldung nun an dieser Stelle.

*Beispiel*:

```cpp
std::list<int> numbers{ 5, 6, 1, 3, 7 };

std::sort(
    numbers.begin(),
    numbers.end()
);
```

Die Fehlermeldung lautet

```
binary '-':
'const std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<_Ty>>>'
does not define this operator or a conversion to a type acceptable to the predefined operator
```

und sie wird aus der Datei *algorithm.h* in Zeile 8244 gemeldet!

*Beispiel*:

```cpp
std::list<int> numbers{ 5, 6, 1, 3, 7 };

std::ranges::sort(numbers);
```

Die Fehlermeldung lautet nun

```
   20 |         std::ranges::sort(numbers);
      |         ~~~~~~~~~~~~~~~~~^~~~~~~~~
      |     note:   'std::random_access_iterator_tag' is not a base of 'std::bidirectional_iterator_tag'
      |     concept derived_from = __is_base_of(_Base, _Derived)
      | 
```

Dieses Mal finden wir die Zeilennummer 20 und die korrekte Zuordnung zur Quelldatei vor.

---

## *Views* (Ansichten) <a name="link2"></a>

  * Ansichten sind so genannten *lightweight Ranges* (*leichtgewichtige Bereiche*)

  * Ansichten haben * non-owning * (nicht besitzende) Referenzen der Daten.

  * Ansichten können über die Elemente eines Bereichs iterieren
und dabei die Daten transformieren oder filtern.

  * Ansichten benötigen eine konstante Zeitkomplexität zum Kopieren, Verschieben oder Zuweisen von Ansichten.


*Syntax*:

```cpp
std::ranges::operation_view { range, arguments... }
```




  
---

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


---

## Literatur <a name="link6"></a>

---

[Zurück](../../Readme.md)

Die Anregungen zum konzeptionellen Beispiel finden Sie unter

[https://hackernoon.com/c-performance-optimization-best-practices](C++ Performance Optimization: Best Practices)

---



*Beispiel*:

```cpp
```

*Beispiel*:

```cpp
```


