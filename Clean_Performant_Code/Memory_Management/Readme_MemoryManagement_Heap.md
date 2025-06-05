# Der Heap (Halde)

[Zur�ck](Readme_MemoryManagement.md)

---

## Inhalt
  
  * [Allgemeines](#link1)


  * [Der Heap (Halde)](#link12)
    * [*Low-level* Speicherverwaltungsfunktionen](#link13)
    * [*Placement new*](#link14)
    * [&bdquo;*Zeigerw�sche*&rdquo;: `std::launder`](#link15)

---

#### Quellcode

[*STL_Algorithms.cpp*](STL_Algorithms.cpp)<br />

---

## Allgemeines <a name="link1"></a>

---


### *Low-level* Speicherverwaltungsfunktionen <a name="link13"></a>

Die manuelle Lebensdauerverwaltung und das Erstellen von Objekten in nicht initialisierten Speicherbl�cken
ist ein spezielleres Thema.

Es gibt Situationen, in denen `std::vector` nicht die Performanz bietet, die man ben�tigt
oder die vielleicht in C machbar w�re. Ein Umstieg von C auf C++ sollte generell nicht angestrebt werden,
aber eine Erweiterung der STL um neue, performantere Funktionen k�nnte Abhilfe schaffen.

Die C++-Standardbibliothek STL bietet eine Reihe neuer, *low-level* Algorithmen,
die Standard-, Kopier-, Verschiebe- und Wertkonstruktion sowie deren Freigabe
auf der Grundlage nicht initialisierten Speichers erm�glichen.

### Ein Beispiel: `std::uninitialized_copy`

Wir betrachten eine h�ufig in der Praxis auftretende Situation:

Es liegen Daten in einem Container vor. Diese sollen &ndash; nach einer entsprechenden Verarbeitung &ndash;
in einen zweiten Container umkopiert werden. `std::copy` oder `std::transform` w�ren die naheliegenden Algorithmen aus der STL,
um dies durchzuf�hren:

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
wird zun�chste mit &bdquo;leeren&rdquo; `Integer`-Objekten angelegt.
Der Speicher des `std::vector`-Containers wird nat�rlich ben�tigt,
aber die Vorbelegung (Initialisierung) der entsprechenden Anzahl von `Integer`-Objekten wird umsonst durchgef�hrt,
da ja in der unmittelbar folgenden Anweisung ein Kopier-Vorgang mit dem `std::vector`-Container als Ziel eingeleitet wird.

Was man br�uchte, w�re eine M�glichkeit, einen `std::vector`-Container mit `Integer`-Objekten anlegen zu k�nnen,
aber eben ohne den gesamten Speicherbereich zu initialisieren.

Dazu gibt es nun den Algorithmus `std::uninitialized_copy`.

Der Wehrmutstropfen bei diesem Ansatz ist, dass wir Speicher ben�tigen, der nicht initialisiert wird.
Das geht, aber nicht mit der Klasse `std::vector`. Wir m�ssen auf Funktionen wie beispielsweise

  * `std:malloc` / `std::free`
  * `std::aligned_alloc` / `std::free` (`std::aligned_alloc` wird von Visual C++ nicht unterst�tzt)
  * `_aligned_malloc` / `_aligned_free` (Alternative Funktionen, von Visual C++ bereitgestellt)

zur�ckgreifen.

*Beispiel* 2:

```cpp
01: void static test_uninitialized_copy_03_01() {
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

### *Placement new* <a name="link14"></a>

C++ erm�glicht es uns, die Bereitstellung von Speicher (Speicherallokation)
von der Objekterstellung zu trennen.

Wir k�nnten beispielsweise mit `malloc()` ein Byte-Array reservieren
und in diesem Speicherbereich ein neues benutzerdefiniertes Objekt erstellen.

*Beispiel*:

Die m�glicherweise ungewohnte Syntax, die `::new (memory)` verwendet, hei�t *Placement new*.

Es handelt sich um eine nicht allokierende Form von `new`, die nur ein Objekt konstruiert.

Der doppelte Doppelpunkt (`::`) vor `new` stellt sicher,
dass die Aufl�sung aus dem globalen Namensraum erfolgt, um zu vermeiden,
dass eine �berladene Version des Operators `new` verwendet wird.

*Beispiel*:

```cpp
01:  void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = ::new (memory) Person{ "Sepp", "Mueller", (size_t)30 };
05: 
06:     person->~Person();
07:     std::free(memory);
08: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
```

Es gibt kein *Placement delete*.
Um das Objekt zu zerst�ren und den Speicher freizugeben, m�ssen wir also den Destruktor explizit aufrufen
und dann den Speicher separat freigeben.

C++17 f�hrt eine Reihe von Hilfsfunktionen in der STL (Include-Datei `<memory>`) ein,
um Objekte zu konstruieren und zu zerst�ren, ohne Speicher zuzuweisen oder freizugeben.

Anstatt *Placement new* zu verwenden, ist es jetzt also m�glich,
einige der Funktionen aus der STL zu verwenden,
deren Namen mit `std::uninitialized_` beginnen, um Objekte in einen nicht initialisierten Speicherbereich
zu konstruieren, zu kopieren und zu verschieben.

Und anstatt den Destruktor explizit aufzurufen, k�nnen wir jetzt `std::destroy_at()` verwenden,
um ein Objekt an einer bestimmten Speicheradresse zu zerst�ren, ohne den Speicher freizugeben.

Das vorherige Beispiel k�nnte mit diesen neuen Funktionen so geschrieben werden:

*Beispiel*:

```cpp
01: void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = reinterpret_cast<Person*>(memory);
05: 
06:     std::uninitialized_fill_n(person, 1, Person{ "Sepp", "Mueller", (size_t) 30 });
07:         
08:     std::destroy_at(person);
09:     std::free(memory);
10: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
d'tor Person
```


Der Aufruf der `std::uninitialized_fill_n`-Funktion kann ab C++ 20 auch
durch einen  Aufruf der `std::construct_at`-Funktion ersetzt werden.
Diese Funktion ist bzgl. ihres Aufrufs etwas einfacher in der Handhabung:


*Beispiel*:

```cpp
01: void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = reinterpret_cast<Person*>(memory);
05: 
06:     std::construct_at(person, Person{ "Sepp", "Mueller", (size_t)30 }); // C++20
07: 
08:     std::destroy_at(person);
09:     std::free(memory);
10: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
d'tor Person
```


Bitte beachte, dass wir diese *Low-Level*-Speicherfunktionen betrachten,
um ein besseres Verst�ndnis der Speicherverwaltung in C++ zu vermitteln.

Die Verwendung von `reinterpret_cast` und die hier gezeigten Speicherverwaltungsfunktionen
sollte in einer C++-Codebasis auf ein absolutes Minimum beschr�nkt werden.


### Ein zweites Beispiel

Wir betrachten in diesem Beispiel eine Klasse `BigData`:
Das Klassentemplate soll f�r Daten unterschiedlichen Typs `T`
eine Art Container sein.

#### Erster Ansatz

Eine erste einfache Realisierung, die nur die Deklaration der Instanzvariablen
und einen benutzerdefinierten Konstruktur zeigt, k�nnte so aussehen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T*          m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s) / d'tor
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:         : m_elems{ new T[size] }, m_size{ size }
14:     {
15:         std::fill(m_elems, m_elems + m_size, init);
16:     }
17: 
18:     ~BigData()
19:     {
20:         delete[] m_elems;
21:     }
22: 
23:     // ...
24: };
```

Was ist schlecht in puncto &bdquo;Performanz&rdquo; an dieser Implementierung?

Okay, es h�ngt ein wenig von der konkreten Wahl des Datentyps `T` ab.
Stellen wir uns vor, `T` repr�sentiert *keinen* elementaren Datentyp,
also zum Beispiel die Klasse `std::string` oder einen anderen benutzerdefinierten Typ.

Der Konstruktor reserviert zun�chst auf der Halde dynamisch Speicher f�r `size` Elemente des Typs `T` (mittels `new T[size]`).
Das bedeutet insbesondere, dass `size` Mal der Standardkonstruktor des Typs `T` ausgef�hrt wird.

Nun gibt es aber einen Vorbelegungswert `init`, mit dem alle Objekte im Speicherbereich `m_elems` vorbelegt werden sollen.
Es kommt also noch zus�tzlich `size` Mal der Kopierkonstruktor der Klasse `T` mit `init` als Vorlage zur Ausf�hrung.

Im Prinzip belegen wir den Speicherbereich `m_elems` zweimal vor:
Zum Ersten mit dem Standardkonstruktor des Typs `T` und zum Zweiten mit seinem Kopierkonstruktor.
Wie k�nnte man dies vermeiden?

#### Zweiter Ansatz

Mit Hilfe von *Placement new* k�nnen wir eine Vereinfachung erreichen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T* m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s)
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:     {
14:         m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
15:         m_size = size;
16: 
17:         for (auto pBegin = m_elems; pBegin != m_elems + m_size; ++pBegin) {
18:             ::new (static_cast<void*>(pBegin)) T{ init };
19:         }
20:     }
21: 
22:     ~BigData()
23:     {
24:         std::for_each(
25:             m_elems,
26:             m_elems + m_size,
27:             [](const T& obj) { obj.~T(); }
28:         );
29: 
30:         std::free(m_elems);
31:     }
32: 
33:     // ...
34: };
```

In Zeile 14 reservieren wir den Speicher mit `std::malloc`. Dies erspart uns die unn�tze Ausf�hrung des Standardkonstruktors
f�r alle `T`-Objekte.

Um die `T`-Objekte wie gefordert vorzubelegen (mit dem Wert `init`),
wenden wir den *Placement new*&ndash;Mechanismus an (Zeile 18).

Wir d�rfen in dieser Variante nicht �bersehen, dass wir den Destruktor der Klasse `T` explizit aufrufen m�ssen.
Dies erfolgt in Zeile 27.

#### Dritter Ansatz

F�r die *Placement new*&ndash;Techniken aus dem letzten Beispiel gibt es Low-Level-Funktionen, 
die deren Schreibweise vereinfachen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T*          m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s)
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:     {
14:         m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
15:         m_size = size;
16:         std::uninitialized_fill(m_elems, m_elems + m_size, init);
17:     }
18: 
19:     ~BigData()
20:     {
21:         std::destroy(m_elems, m_elems + m_size);
22:         std::free(m_elems);
23:     }
24: 
25:     // ...
26: };
```

*Hinweis*:<br />
Zwischen `std::fill` und `std::uninitialized_fill` gibt es folgenden Unterschied in der Ausf�hrung:


  * `std::fill`:<br />
  Es wird ein g�ltig vorbelegter Speicherbereich erwartet. Das letzte Argument von `std::fill` wird mit dem `=`-Operator
  an alle vorhandenen Elemente im Speicherbereich zugewiesen. Folglich erwartet der `operator=` einen Speicherbereich,
  der g�ltige Werte enth�lt.

  * `std::uninitialized_fill`:<br />
  Der zu belegende Speicherbereich kann beliebige Werte enthalten.
  Es wird der gesamte Speicherbereich im Stile des *Placement new*&ndash;Mechanismus mit dem letzten Argument von `std::uninitialized_fill` vorbelegt.

Die Ausf�hrungszeiten erf�llen auf meinem Rechner nicht alle Erwartungen:
Jeweils drei aufeinanderfolgende Zeilen betrachten ein `BigData`-Objekt
mit `int`, `std::string` oder `Person`-Objekten:

```
[1]:    Elapsed time: 4 [milliseconds]
[1]:    Elapsed time: 4602 [milliseconds]
[1]:    Elapsed time: 7700 [milliseconds]

[1]:    Elapsed time: 3 [milliseconds]
[1]:    Elapsed time: 3315 [milliseconds]
[1]:    Elapsed time: 7693 [milliseconds]

[1]:    Elapsed time: 4 [milliseconds]
[1]:    Elapsed time: 3371 [milliseconds]
[1]:    Elapsed time: 7717 [milliseconds]
```

---

### &bdquo;*Zeigerw�sche*&rdquo;: `std::launder` <a name="link15"></a>

Die STL-Funktion ist etwas kurios, sie f�hrt eine &bdquo;*Zeigerw�sche*&rdquo; durch.

`std::launder` gibt einen Zeiger auf denselben Speicher zur�ck, auf den das Argument von `std::launder` zeigt.
Es wird jedoch angenommen,
dass das Referenzobjekt eine andere Lebensdauer und einen anderen dynamischen Typ hat als der &bdquo;neue&rdquo; Zeiger,
den `std::launder` zur�ckliefert.

Ein Aufruf von `std::launder` hat keinen Einfluss auf das Argument.
Der R�ckgabewert muss verwendet werden, um auf das Objekt in einer &bdquo;*neuen Sichtweise*&rdquo; zuzugreifen.
Es ergibt also keinen Sinn, `std::launder` aufzurufen und den R�ckgabewert zu verwerfen.

*Beispiel*:

```cpp
01: static void test_std_launder()
02: {
03:     constexpr int CountPersons{ 5 };
04: 
05:     alignas(class Person) unsigned char buffer[sizeof(class Person) * CountPersons];
06: 
07:     for (size_t i{}; i != CountPersons; ++i) {
08: 
09:         std::string first{ "first_name_" };
10:         std::string last{ "last_name_" };
11: 
12:         // manually construct objects using placement new
13:         new(buffer + sizeof(Person) * i) Person{ first + std::to_string(i), last + std::to_string(i), i};
14:     }
15: 
16:     auto ptr{ std::launder(reinterpret_cast<Person*>(buffer)) };
17: 
18:     for (size_t i{}; i != CountPersons; ++i) {
19: 
20:         std::destroy_at(&ptr[i]);
21:     }
22: }
```

*Ausf�hrung*:

```
c'tor Person
c'tor Person
c'tor Person
c'tor Person
c'tor Person
d'tor Person
d'tor Person
d'tor Person
d'tor Person
d'tor Person
```

*Anwendungsfall*:<br />

Definieren eines Zeigers
mit Hilfe eines anderen Zeigers, der auf einen Speicherbereich verweist,
in dem ein mittels *Placement new* erzeugtes Objekt residiert.
Der Zeiger, der f�r *Placement new* den Speicher bereitstellt,
ist meist von einem skalaren Typ wie `std::byte`, `unsigned char` oder �hnliches.

Einige erg�nzende Erl�uterungen dazu:

  * Bei `std::launder` handelt es sich um eine Funktion f�r den Compiler im Umfeld der Low-Level-Speicherverwaltung und der
  Betrachtung von Lebensdauererwartungen bestimmter Objekte,
  um die Nachverfolgung und Optimierungen zur Kompilierzeit zu deaktivieren, die m�glicherweise nicht korrekt sind.

  * Wird normalerweise nur verwendet, wenn Sie die Lebensdauer eines Objekts �ber oder innerhalb eines anderen starten.

  * Wenn Sie m�chten, dass der Compiler dumm ist, waschen Sie im Wesentlichen den Zeiger,
  damit der Compiler die komplexe Zustandsnachverfolgung zur Kompilierzeit, die Compiler durchf�hren, absichtlich vergisst und so tut,
  als w�re der Zeiger tats�chlich ein brandneues Objekt, von dem er nichts wusste.

  * Es liegen gewisse �hnlichkeiten zum Schl�sselwort `volatile` vor:
  `volatile` dient zum Deaktivieren von Compiler-Annahmen dar�ber, was ein Wert zur Laufzeit sein k�nnte.
  Wenn Sie beispielsweise zweimal hintereinander von einer regul�ren `int`-Variablen lesen, ohne dazwischen zu schreiben,
  w�rde der Compiler wahrscheinlich den zweiten Lesevorgang entfernen und den ersten Wert wiederverwenden (bessere Codegenerierung),
  da er wei�, dass der Wert nicht ge�ndert wurde.

  * Mit dem Schl�sselwort `volatile` teilen Sie dem Compiler mit, dass er nicht alle �nderungen an der Variable zuverl�ssig beobachten kann,
  sodass jeder Lesevorgang ausgef�hrt werden muss (dasselbe gilt auch f�r Schreibvorg�nge).

  * `std::launder` und `volatile` sind sich insofern �hnlich, als sie existieren, um dem Compiler mitzuteilen,
  dass er keine Annahmen �ber die Werte/Objekte auf Grund m�glicher Beobachtungen treffen soll.

---

---

[Zur�ck](Readme_MemoryManagement.md)

---
