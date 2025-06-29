# *Low-level* Speicherverwaltungsfunktionen

[Zurück](Readme_MemoryManagement.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Ein Beispiel: `std::uninitialized_copy`](#link2) 
  * [Literatur](#link3) 

---

#### Quellcode

[*MemoryManagement_LowLevelSTLAlgorithms.cpp*](MemoryManagement_LowLevelSTLAlgorithms.cpp)<br />

---

## Allgemeines <a name="link1"></a>

Die manuelle Lebensdauerverwaltung und das Erstellen von Objekten in nicht initialisierten Speicherblöcken
ist ein spezielles Thema.

Es gibt Situationen, in denen die Klasse `std::vector` nicht die Performanz bietet, die man benötigt
oder die vielleicht in C machbar wäre. Ein Umstieg von C auf C++ sollte generell nicht angestrebt werden,
aber eine Erweiterung der STL um neue, performantere Funktionen könnte Abhilfe schaffen.

Die C++-Standardbibliothek STL bietet eine Reihe neuer, *low-level* Algorithmen,
die Standard-, Kopier-, Verschiebe- und Wertkonstruktion sowie deren Freigabe
auf der Grundlage nicht initialisierten Speichers ermöglichen.

### Ein Beispiel: `std::uninitialized_copy` <a name="link2"></a>

Wir betrachten eine häufig in der Praxis auftretende Situation:

Es liegen Daten in einem Container vor. Diese sollen &ndash; nach einer entsprechenden Verarbeitung &ndash;
in einen zweiten Container umkopiert werden. `std::copy` oder `std::transform` wären die naheliegenden Algorithmen aus der STL,
um dies durchzuführen:

*Beispiel* 1:

```cpp
01: class Integer
02: {
03: public:
04:     Integer() : m_value{} {}
05:     Integer(int value) : m_value{ value } {}
06:     int get() const{ return m_value; }
07: 
08: private:
09:     int m_value;
10: };
11: 
12: void test() {
13: 
14:     const size_t Size = 10;
15: 
16:     std::array<int, Size> numbers{};
17:     std::iota(numbers.begin(), numbers.end(), 1);
18: 
19:     std::vector<Integer> vec;
20:     vec.resize(Size);
21: 
22:     auto last = std::copy(
23:         numbers.begin(),
24:         numbers.end(),
25:         vec.begin()
26:     );
27: 
28:     for (auto it{ vec.begin()}; it != vec.end(); ++it) {
29:         std::print("{} ", (*it).get());
30:     }
31:     std::println();
32: }
```

Was ist in puncto Performanz kritisch an diesem Beispiel?

Der Ziel-Container &ndash; in diesem Beispiel ein `std::vector`-Container mit `Integer`-Objekten &ndash; 
wird zunächst mit &bdquo;leeren&rdquo; `Integer`-Objekten angelegt.
Der Speicher des `std::vector`-Containers wird natürlich benötigt,
aber die Vorbelegung (Initialisierung) der entsprechenden Anzahl von `Integer`-Objekten wird umsonst durchgeführt,
da ja in der unmittelbar folgenden Anweisung ein Kopier-Vorgang mit dem `std::vector`-Container als Ziel eingeleitet wird.

Was man bräuchte, wäre eine Möglichkeit, einen `std::vector`-Container mit `Integer`-Objekten anlegen zu können,
aber eben ohne den gesamten Speicherbereich zu initialisieren.

Dazu gibt es nun den Algorithmus `std::uninitialized_copy`.

Der Wehrmutstropfen bei diesem Ansatz ist, dass wir Speicher benötigen, der nicht initialisiert wird.
Das geht, aber nicht mit der Klasse `std::vector`. Wir müssen auf Funktionen wie beispielsweise

  * `std:malloc` / `std::free`
  * `std::aligned_alloc` / `std::free` (`std::aligned_alloc` wird von Visual C++ nicht unterstützt)
  * `_aligned_malloc` / `_aligned_free` (Alternative Funktionen, von Visual C++ bereitgestellt)

zurückgreifen.

*Bemerkung*:

Auch wenn es auch der Definition der Schnittstelle von `std:malloc` nicht ersichtlich ist:
`std:malloc` liefert einen Zeiger auf das niedrigste (erste) Byte eines zugewiesenen Speicherblocks zurück,
der für jeden Skalartyp geeignet ausgerichtet (*Alignment*) ist &ndash; mindestens so streng wie `std::max_align_t`.

*Beispiel* 2:

```cpp
01: void test() {
02: 
03:     const size_t Size = 10;
04: 
05:     std::array<int, Size> numbers{};
06:     std::iota(numbers.begin(), numbers.end(), 1);
07: 
08:     void* buffer = std::malloc(Size * sizeof(Integer));
09:     if (buffer == nullptr) {
10:         return;
11:     }
12: 
13:     auto first = static_cast<Integer*>(buffer);
14: 
15:     Integer* last = std::uninitialized_copy(
16:         numbers.begin(),
17:         numbers.end(),
18:         first
19:     );
20: 
21:     for (auto it{ first }; it != last; ++it) {
22:         std::print("{} ", (*it).get());
23:     }
24:     std::println();
25: 
26:     std::free(buffer);
27: }
```

Wenn wir uns auf diese Ebene begeben, kommt neben `std::uninitialized_copy` noch eine zweite Funktion
ins Spiel:

  * `std::destroy`

Im letzten Beispiel hatten wir einen Speicherbereich mit `Integer`-Objekten ausgebreitet.
Derartige Objekt haben keinen Destruktor, es ist bzgl. der Freigabe des Speichers der benutzen Objekte also nichts zu tun.
Anders sieht das aus, wenn Objekte mit dynamischen Daten umkopiert werden.
Diese haben einen Destruktor &ndash; und dieser muss aufgerufen werden.

Hierzu folgen nun zwei weitere Beispiele, wir wechseln von der Klasse `Integer` zur Klasse `std::string`:

*Beispiel* 3:

```cpp
01: void test() {
02: 
03:     const size_t Size = 6;
04:     std::array<std::string, Size> strings {"This", "is", "an", "array", "of", "strings"};
05: 
06:     std::vector<std::string> vec;
07:     vec.resize(Size);
08: 
09:     auto last = std::copy(
10:         strings.begin(),
11:         strings.end(),
12:         vec.begin()
13:     );
14: 
15:     for (auto it{ vec.begin() }; it != vec.end(); ++it) {
16:         std::print("{} ", *it);
17:     }
18:     std::println();
19: }
```

*Beispiel* 3:

```cpp
01: void test() {
02: 
03:     const size_t Size = 6;
04:     std::array<std::string, Size> strings{ "This", "is", "an", "array", "of", "strings" };
05: 
06:     void* buffer = std::malloc(Size * sizeof(std::string));
07:     if (buffer == nullptr) {
08:         return;
09:     }
10: 
11:     auto first = static_cast<std::string*>(buffer);
12: 
13:     std::string* last = std::uninitialized_copy(      // <=====
14:         strings.begin(),
15:         strings.end(),
16:         first
17:     );
18: 
19:     for (auto it{ first }; it != last; ++it) {
20:         std::print("{} ", *it);
21:     }
22:     std::println();
23: 
24:     std::destroy(first, last);                        // <=====
25: 
26:     std::free(buffer);
27: }
```

---

## Literatur <a name="link3"></a>

Gute Beispiele und Beschreibungen der neuen *Low-level* Speicherverwaltungsfunktionen findet man unter

[Daily bit(e) of C++ | `std::uninitialized_copy`, `std::uninitialized_fill`, `std::uninitialized_move`, etc.](https://medium.com/@simontoth/daily-bit-e-of-c-std-uninitialized-copy-std-uninitialized-fill-std-uninitialized-move-f4a6b5afb7ba),<br />
verfasst von *Simon Toth*.

oder in einem Artikel von *Sandor Dargo*:

[The big STL Algorithms tutorial: the memory header](https://www.sandordargo.com/blog/2022/02/02/stl-alogorithms-tutorial-part-30-memory-header).

---

[Zurück](Readme_MemoryManagement.md)

---
