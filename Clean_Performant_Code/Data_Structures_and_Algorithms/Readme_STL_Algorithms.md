# STL Container

[Zurück](Readme_Data_Structures_and_Algorithms.md)

---

## Inhalt

  * [Allgemeines](#link)

---

#### Quellcode

[*STL_Algorithms.cpp*](STL_Algorithms.cpp)<br />

---

## Allgemeines <a name="link1"></a>

Es werden folgenden STL-Algorithmen an Hand von einfachen Beispielen vorgestellt:

  * `std::fill`
  * `std::for_each`
  * `std::generate`
  * `std::iota`
  * `std::copy`
  * `std::transform`
  * `std::accumulate`
  * `std::sort`
  * `std::find`
  * `std::is_sorted`
  * `std::binary_search`
  * `std::none_of`
  * `std::all_of`
  * `std::any_of`
  * `std::count`
  * `std::equal_range`
  * `std::distance`
  * `std::min`
  * `std::max`
  * `std::clamp`
  * `std::minmax_element`

---

## Best Practice: Nicht-generische versus generische Funktion <a name="link1"></a>

```cpp
01: auto contains(const std::vector<int>& vec, int elem) {
02: 
03:     for (size_t i{}; i != vec.size(); ++i) {
04:         if (vec[i] == elem) {
05:             return true;
06:         }
07:     }
08:     return false;
09: }
```

Um das gesuchte Element zu finden, verlassen wir uns auf die Schnittstelle von `std::vector`,
die uns die Funktion `size()` und den Indexoperator (`operator[]()`) zur Verfügung stellt.

Allerdings stellen uns nicht alle Container diese Funktionen zur Verfügung,
damit ist eine derartige `for`-Wiederholungsschleife nicht sehr empfehlenswert!

Stattdessen können wir ein Funktionstemplate erstellen, das mit Iteratoren arbeitet.
Auf diese Weise erhalten wir eine `contains`-Funktion, die mit unterschiedlichen STL-Containertypen arbeiten kann:

```cpp
01: template <typename TIterator, typename T>
02: auto contains(TIterator begin, TIterator end, const T& x) {
03: 
04:     for (auto it{ begin }; it != end; ++it) {
05:         if (*it == x) {
06:             return true;
07:         }
08:     }
09:     return false;
10: }
```

*Anwendungsbeispiel*:

```cpp
01: void test()
02: {
03:     const auto values1{ std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
04:     const auto values2{ std::list{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
05:     std::list<std::string> values3{ "1.5", "2.5", "3.5", "4.5", "5.5" };
06: 
07:     bool found1{ contains(values1.begin(), values1.end(), 3) };
08:     bool found2{ contains(values2.begin(), values2.end(), 3) };
09:     bool found3{ contains(values3.begin(), values3.end(), "3.5")};
10: }
```

---

## Best Practice: Benutzerdefinierte Datentypen mit `begin()` und `end()` <a name="link1"></a>

Wir wir im letzten Abschnitt gesehen haben, können neue benutzerdefinierte Datentypen,
die wir erstellen, von den standardmäßigen generischen Algorithmen verwendet werden können,
solange sie die Iteratoren `begin()` und `end()` verfügbar machen.

Als einfaches Beispiel implementieren wir eine zweidimensionale `Grid`-Struktur,
bei der Zeilen als Iteratorenpaar verfügbar gemacht werden, wie folgt:

```cpp
01: class Grid
02: {
03: public:
04:     Grid() : Grid{ 0, 0} {}
05: 
06:     Grid(size_t width, size_t height) : m_width{ width }, m_height{ height }
07:     {
08:         m_data.resize(width * height);
09:     }
10: 
11:     auto getData() // returns whole grid as iterator pairs
12:     {
13:         auto left = m_data.begin();
14:         auto right = m_data.begin() + m_width * m_height;
15:         return std::make_pair(left, right);
16:     }
17: 
18:     auto getRow(size_t i) // returns iterator pair for corresponding row
19:     {
20:         auto left = m_data.begin() + m_width * i;
21:         auto right = left + m_width;
22:         return std::make_pair(left, right);
23:     }
24:         
25:     auto print() 
26:     {
27:         std::for_each(
28:             m_data.begin(),
29:             m_data.end(),
30:             [this, i=0, j=0] (auto elem) mutable {
31:                 
32:                 std::print("[{}:{}] {:2}  ", i, j, m_data[m_width * i + j]);
33: 
34:                 ++j;
35:                 if (j == m_width) {
36:                     ++i;
37:                     j = 0;
38:                     std::println();
39:                 }
40:             }
41:         );
42:     }
43: 
44:     auto getWidth() const { return m_width; }
45:     auto getHeight() const { return m_height; }
46: 
47: private:
48:     std::vector<int> m_data;
49:     size_t           m_width;
50:     size_t           m_height;
51: };
```

---

## Best Practice: Optimierungstechniken der STL <a name="link1"></a>

Die Realisierung der STL ist immer für Überraschungen gut:

Selbst scheinbar sehr einfache Algorithmen können Optimierungen enthalten,
die man normalerweise nicht in Betracht ziehen würde.

Wir betrachten als Beispiel den Algorithmus `std::find()`:

WEITER WEITER

---

[Zurück](Readme_Data_Structures_and_Algorithms.md)

---
