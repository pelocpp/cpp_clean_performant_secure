# `std::ranges` Bibliothek

[Zurück](../../Readme.md)

---

## Inhalt

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

  * Views sind so genannte *lightweight Ranges* (*leichtgewichtige Bereiche*)

  * Views haben *non-owning* (nicht besitzende) Referenzen der Daten.

  * Views können über die Elemente eines Bereichs iterieren
   und dabei die Daten transformieren oder filtern.

  * Views benötigen eine konstante Zeitkomplexität zum Kopieren, Verschieben oder Zuweisen von Ansichten.


*Syntax*:

```cpp
std::ranges::operation_view { range, arguments... }
```


*Beispiel*:

```cpp
01: std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
02: 
03: auto view{ std::ranges::take_view{ numbers, 3 } };
04: 
05: std::ranges::for_each(
06:     view,
07:     [] (auto n) { std::print("{} ", n); }
08: );
```

*Ausgabe*:

```
1 2 3
```

Normalerweise werden Ansichten nicht mit ihren Konstruktoren,
sondern mit *Range Adaptors* (Bereichsadapter) erstellt.

---

## *Range Adaptors* (Bereichsadapter) <a name="link2"></a>

  * Bereichsadapter sind Hilfsfunktionen, die Views erstellen,
  wie beispielsweise `views::filter` , `views::transform` 
  und `views::take`.

  * Es ist vorzuziehen, Adapter zum Erstellen von Ansichten zu verwenden, da sie Optimierungen ermöglichen:
```
range | std::ranges::views::operation(arguments...)
```

  * Bereichsadapter können zusammengesetzt werden (*Composition*),
  um komplexe *Data Processing Pipelines* (Datenverarbeitungspipelines) effizient aufzubauen.


*Beispiel*:

```cpp
01: std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
02: 
03: auto result = numbers 
04:     | std::views::filter([](auto n) { return n % 2 == 0; })
05:     | std::views::transform([](auto n) { return n * 2; });
06: 
07: for (auto n : result) {
08:     std::print("{} ", n);
09: }
```

*Ausgabe*:

```
4 8 12
```

---

## *Function Composition, Pipelines* (Komposition von Funktionen) <a name="link2"></a>

Da es sich bei einer View (Ansicht) um einen Range (Bereich) handelt,
kann man eine Ansicht als Argument für eine andere Ansicht verwenden,
um eine Verkettung zu ermöglichen:

*Beispiel*:

```cpp
01: std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: 
03: auto result {
04:     std::views::reverse(
05:         std::views::take(
06:             std::views::transform(
07:                 std::views::filter(numbers, [](auto n) { return n % 2 == 0; }),
08:                 [](auto n) { return n * n; }
09:             ),
10:             4
11:         )
12:     ) 
13: };
14: 
15: std::ranges::for_each(
16:     result,
17:     [](auto n) { std::print("{} ", n); }
18: );
```

*Ausgabe*:

```
64 36 16 4
```

Übermäßige Verschachtelung kann zu Lesbarkeits- und Wartungsproblemen führen.

Es gibt eine weitere Möglichkeit, Bereiche und Ansichten zu kombinieren,
indem Sie den Pipe-Operator `|` verwenden:

*Beispiel*:

```cpp
01: std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: 
03: auto result = numbers | std::views::filter([](auto n) { return n % 2 == 0; })
04:     | std::views::transform([](auto n) { return n * n; })
05:     | std::views::take(4)
06:     | std::views::reverse;
07: 
08: for (auto n : result) {
09:     std::print("{} ", n);
10: }
```

---

## *Lazy Evaluation* (*Lazy* Auswertung) <a name="link2"></a>

  * Eine Ansicht ist lediglich die Beschreibung einer Verarbeitung.

  * Die Verarbeitung findet nicht statt, wenn man die Ansicht definiert.

  * Die eigentliche Verarbeitung wird Element für Element ausgeführt, wenn der nächste Wert *angefordert* wird.

*Beispiel*:

```cpp
01: auto lazySquares = 
02:     std::views::iota(1, 10) | std::views::transform([](auto i) { return i * i; });
03: 
04: for (auto n : lazySquares) {
05:     std::print("{} ", n);             // squares are calculated here
06: }
```


*Ausgabe*:

```
1 4 9 16 25 36 49 64 81
```

---

## Begrenzte vs. unbegrenzte (unendliche) Ranges <a name="link2"></a>

  * `std::views::iota` kann als eine *Range-Factory* angesehen werden,
  die eine Sequenz von Elementen generiert, indem sie einen Anfangswert wiederholt inkrementiert.

  * Die Sequenz kann entweder begrenzt oder unbegrenzt (unendlich) sein.

  * `std::views::iota(10)` ist eine *lazy* View. Sie erzeugt nur dann einen neuen Wert,
  wenn dieser angefordert wird.


*Beispiel*:

```cpp
01: // use bounded std::views::iota with begin and end value
02: for (int n : std::views::iota(0, 10)) {
03:     std::print("{} ", n);
04: }
05: std::println();
06:         
07: // use infinite std::views::iota in combination with std::views::take
08: for (int n : std::views::iota(0) | std::views::take(10)) { 
09:     std::print("{} ", n);
10: }
11: std::println();
12:         
13: // use infinite std::views::iota in combination with std::views::take_while
14: for (int n : std::views::iota(0) | std::views::take_while([] (int y) { return y < 10; })) {
15:     std::print("{} ", n);
16: }
17: std::println();
```

*Ausgabe*:

```
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
```

---

## *Lazy* Auswertung, Beispiel für Primzahlen <a name="link2"></a>

Wir betrachten ein Beispiel zur Berechnung von Primzahlen:

*Beispiel*:

```cpp
01: bool isPrime(int num)
02: {
03:     if (num <= 1) return false;
04:     if (num % 2 == 0) return false;
05: 
06:     for (int i = 2; i * i <= num; ++i) {
07:         if (num % i == 0) return false;
08:     }
09: 
10:     return true;
11: }
12: 
13: void test()
14: {
15:     // generate an infinite range starting from 2
16:     auto numbers = std::ranges::iota_view{ 2 };
17: 
18:     // filter the numbers to keep only prime numbers and take the first 20 ones
19:     auto primeNumbers = numbers
20:         | std::views::filter(isPrime)
21:         | std::views::take(20);
22: 
23:     // print the first 20 prime numbers
24:     for (int prime : primeNumbers) {
25:         std::cout << prime << " ";
26:     }
27: }
```

*Ausgabe*:

```
3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73
```

---

## Projektionen (*Projections*) <a name="link2"></a>

Viele bereichsbasierte Algorithmen haben einen so genannten *Projektionsparameter*,
einen Rückruf, der jedes Element vor der Verarbeitung transformiert.

Beispiel: Projektionsparameter der Funktion `std::ranges::sort()`<br />
(entnommen aus [https://en.cppreference.com/w/cpp/algorithm/ranges/sort](https://en.cppreference.com/w/cpp/algorithm/ranges/sort)):

```cpp

template<ranges::random_access_range R,
         class Comp = ranges::less,
         class Proj = std::identity>
    requires std::sortable<ranges::iterator_t<R>, Comp, Proj>
constexpr ranges::borrowed_iterator_t<R>
sort( R&& r, Comp comp = {}, Proj proj = {} );
```


*Beispiel*:

```cpp
01: std::vector<int> numbers = { -8, 4, -6, -3, 2, -5, 10, 7, 1, -9 };
02: 
03: // using projection
04: std::ranges::sort(
05:     numbers,
06:     std::ranges::less{},                  // comparison
07:     [](auto n) { return std::abs(n); }    // identity
08: );
09: 
10: for (auto elem : numbers) {
11:     std::print("{} ", elem);
12: }
13: std::println();
```


*Ausgabe*:

```
1 2 -3 4 -5 -6 7 -8 -9 10
```


*Beispiel*:

```cpp
01: std::vector<int> numbers = { -8, 4, -6, -3, 2, -5, 10, 7, 1, -9 };
02: 
03: // using defaulted projection and original value
04: std::ranges::sort(
05:     numbers,
06:     {},
07:     {}
08: );
09: 
10: for (auto elem : numbers) {
11:     std::print("{} ", elem);
12: }
```

*Ausgabe*:

```
-9 -8 -6 -5 -3 1 2 4 7 10
```

---

## Sentinels <a name="link6"></a>

  * *Sentinels* stellen das Ende eines Bereichs dar:

  * Beispiele für Sentinels in der Programmierung:

    * EOF als Ende eines Dateistreams
    * Der Nullterminator `\0` als Ende einer Zeichenfolge im C-Stil
    * `nullptr` als Ende einer verknüpften Liste
    * `-1` als Ende einer Liste mit nicht-negativen Ganzzahlen
    * `\n` als Ende einer Zeile

  * Im Gegensatz zum traditionellen iteratorenbasierten STL-Ansatz,
  bei dem Anfang und Ende einer Sequenz (STL-Container) durch Iteratoren desselben Typs gekennzeichnet sind,
  können der Typ des Sentinels und der des Anfangsiterators unterschiedlich sein.

  * Dieses Feature kann nützlich sein,
  wenn das Ende der Sequenz ungewiss ist, bis es während der Iteration erreicht wird.


*Beispiel*:

```cpp
01: struct NegativeNumber
02: {
03:     bool operator== (std::input_iterator auto iter) const {
04:         return *iter < 0;
05:     }
06: };
07: 
08: static void ranges_sentinels_01()
09: {
10:     std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };
11: 
12:     std::ranges::transform(
13:         numbers.begin(),
14:         NegativeNumber{},   // <== sentinel
15:         numbers.begin(),
16:         [](const auto& n) { return n * n; }
17:     );
18: 
19:     for (auto elem : numbers) {
20:         std::print("{} ", elem);
21:     }
22: }
```


*Ausgabe*:

```
1 4 9 -4 5 6
```

*Beispiel*:

```cpp
01: std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };
02: 
03: std::ranges::for_each(
04:     numbers.begin(),
05:     NegativeNumber{},     // <== sentinel
06:     [] (auto n) { std::print("{} ", n); }
07: );
```


*Ausgabe*:

```
1 2 3
```


*Beispiel*:

```cpp
01: std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };
02: 
03: auto range{ 
04:     std::ranges::subrange{
05:         numbers.begin(),
06:         NegativeNumber{}    // <== sentinel
07:     }
08: };
09: 
10: for (auto elem : range) {
11:     std::print("{} ", elem);
12: }
```


*Ausgabe*:

```
1 2 3
```



*Beispiel*:

```cpp
01: const char* str = "Hello, World!";
02: 
03: std::ranges::for_each(
04:     str,
05:     TerminatingZero{},
06:     [] (char c) { std::print("{} ", c); }
07: );
```


*Ausgabe*:

```
H e l l o ,   W o r l d !
```


---

## *Dangling Iterators* / *Borrowed Iterators*  <a name="link2"></a>

  * Wenn man einen temporären Bereich an einen Algorithmus übergibt, der einen Iterator zurückgibt,
  ist der zurückgegebene Iterator am Ende der Anweisung ungültig, da der Bereich zerstört wird.

  * Dieses Problem würde nicht auftreten, wenn man einen Bereich mit zwei Argumenten definiert (Anfang und Ende).

  * Die *Ranges* Bibliothek führt zu diesem Zweck das Konzept so genannter *Borrowed Iterators* ein.

  * Wenn der an einen Algorithmus übergebene Range ein temporäres Objekt ist,
  dann ist der Rückgabewert ein Objekt vom Typ `std::ranges::dangling`.

*Beispiel*:

```cpp
01: std::vector<int> getData()
02: {
03:     return std::vector<int> { 1, 2, 3, 4, 5 };
04: }
05: 
06: void test()
07: {
08:     auto pos = std::ranges::find( getData(), 123); 
09: 
10:     std::println("{} ", *pos);
11: }
```

Dieses Beispiel ist *nicht* übersetzungsfähig! Die Fehlermeldung lautet in etwa

```
Error: You cannot dereference an operand of type 'std::ranges::dangling'
```

Der Typ `std::ranges::dangling` stellt keinerlei Operationen bereit,
man kann ihn in einem Programm nicht verwenden!
Er dient ausschließlich dem Zweck,
eine praktische Fehlermeldung zu erzeugen,
um darauf hinzuweisen, was schiefgelaufen ist.

Die Lösung des Problems besteht darin, dass man dem temporären Bereich
einen Namen geben muss:

*Beispiel*:

```cpp
01: const auto& values = getData();               // declare const lvalue reference
02: 
03: auto value{ 3 };
04: 
05: auto pos = std::ranges::find(values, value);
06: 
07: if (pos == values.end()) {
08:     std::println("Value {} not found!", value);
09: }
10: else {
11:     std::println("Value {} found!", *pos);
12: }
```

*Ausgabe*:

```
Value 3 found!
```


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


*Ausgabe*:

```
```

