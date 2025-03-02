# Memory Management

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Der Stack (Stapel)](#link2)
  * [Gr��e des Stapels bestimmen](#link3)
  * [Speicher Ausrichtung (Memory Alignment)](#link4)
  * [Der Heap (Halde)](#link5)
  * [Placement *new*](#link6)
  * [&bdquo;*Zeigerw�sche*&rdquo;: `std::launder`](#link7)
  * [Literatur](#link8)

---

## Placement new <a name="link6"></a>

---

#### Quellcode

[*MemoryManagement.cpp*](MemoryManagement.cpp)<br />

[*Arena.h*](Arena.h).<br />
[*Arena.cpp*](Arena.cpp).<br />

---

## Allgemeines <a name="link1"></a>

---

## Der Stack (Stapel) <a name="link2"></a>

Wir analysieren den Stack an einem kleinen Beispiel, um herausfinden,
in welche Richtung er w�chst. Dazu vergleichen wir die Adressen von Variablen, die sich auf dem Stapel befinden.

Das folgende Beispiel zeigt, auf welche Weise der Stapel beim Betreten und Verlassen von Funktionen w�chst und schrumpft:

```cpp
01: static void func1()
02: {
03:     auto i{ 1 };
04:     std::cout << "func1(): " << std::addressof(i) << " - i: " << i << '\n';
05: }
06: 
07: static void func2()
08: {
09:     auto i{ 2 };
10:     std::cout << "func2(): " << std::addressof(i) << " - i: " << i << '\n';
11:     func1();
12: }
13: 
14: static void test_examine_stack() {
15: 
16:     auto i{ 0 };
17:     std::cout << "main():  " << std::addressof(i) << " - i: " << i << '\n';
18:     func2();
19:     func1();
20: }
```

*Bemerkung*:<br />
Die Ausgaben wurden mit dem GCC-Compiler erzeugt.

*Ausgabe*:

```
main():  0x7ffd0e679874 - i: 0
func2(): 0x7ffd0e679854 - i: 2
func1(): 0x7ffd0e679834 - i: 1
func1(): 0x7ffd0e679854 - i: 1
```

Indem wir die Adressen der auf dem Stapel abgelegten `int`-Variablen als ganze Zahl ausgeben,
k�nnen wir analysieren, wie stark und in welche Richtung der Stapel auf einem Rechner w�chst.

Wir beobachten jedes Mal 32 Bytes, wenn wir entweder `func1()` oder `func2()` aufrufen.
Die `int`-Variable `i`, die auf Stapel liegt, ist allerdings nur 4 Bytes (auf meinem Rechner) gro�.

Die verbleibenden 28 Bytes enthalten Daten, die ben�tigt werden, wenn die Funktion endet, wie etwa die R�cksprungadresse und m�glicherweise etwas *Padding*-Bytes zur Ausrichtung.

<img src="Cpp_Examine_Stack.svg" width="700">

*Abbildung* 1: Der Stapel w�chst und schrumpft, wenn Funktionen aufgerufen und verlassen werden.

---

## Die Gr��e des Stapels bestimmen <a name="link3"></a>

Wir versuchen, ein Programm zu schreiben, das die Gr��e des Stacks auf einem Rechner bestimmt.
Dies l�sst sich allerdings nur im Rahmen einer Sch�tzung durchf�hren.

Wir schreiben zu diesem Zweck eine Funktion `func`, an die wir beim Aufruf die Adresse einer Variablen �bergeben,
die auf dem Stack abgelegt ist.
Die Funktion `func` selbst tut zwei Dinge: Zum einen allokiert sie pro Aufruf 1024 Bytes auf dem Stack,
zum anderen berechnet sie &ndash; pro rekursivem Aufruf &ndash; den Abstand dieses Felds zur urspr�nglichen Variablen,
deren Adresse wir uns beim ersten Aufruf gemerkt haben und die wir an jeden Aufruf von `func` weiterreichen.

Auf diese Weise n�hern wir uns der tats�chlichen Gr��e des Stapels, bis es auf Grund eines Stack�berlaufs
zu einem Absturz des Programms kommt. Dies entspricht nicht ganz der feinen englischen Art,
ist aber eine unkonverntionelle Vorgehensweise, um die ungef�hre Gr��e des Stapels zu berechnen.


```cpp
01: void func (const std::byte* stack_bottom_addr) {
02:     std::byte data[1024] = {};
03:     std::cout << stack_bottom_addr - data << '\n';
04: 
05:     func(stack_bottom_addr);
06: }
07: 
08: void test() {
09: 
10:     std::byte b{};
11:     func(&b);
12: }
```

Auf einem Windows Rechner erhalten wir folgende Ausgaben &ndash; im Mittelteil der Ausf�hrung verk�rzt dargestellt:


```
1684
3380
5076
6772
8468
10164
11860
13556
15252
16948
18644
20340
...
993844
995540
997236
998932
1000628
1002324
1004020
1005716
1007412
1009108
1010804
1012500
1014196
1015892
1017588
1019284
1020980
```

Unter Windows ist die Standardgr��e des Stacks normalerweise auf 1 MB eingestellt.
Die Ausgaben des Programms best�tigen dies in etwa &ndash; der Wert 1,020,980 ist
nicht weit von 1.048.576 (1.024 * 1.024) entfernt.


---

## Speicher Ausrichtung (Memory Alignment) <a name="link4"></a>

Vorab einige Begrifflichkeiten:

Jedem Objekt im Speicher besitzt eine so genannte *Ausrichtung* (*Alignment*),
die ihm durch Anforderungen des entsprechenden Typs auferlegt werden.

Die Ausrichtung ist immer eine Potenz von 2 und Objekte mit einer entsprechenden Ausrichtung
k�nnen immer nur an Speicheradressen platziert werden,
die ein Vielfaches dieser Ausrichtung sind.

### `std::alignment_of<T>::value` / `alignof()`

Liefert die Ausrichtung des Typs `T` bzw. des Arguments (Datentyp) `alignof` zur�ck.

*Beispiel*:

```cpp
01: void test() {
02: 
03:     size_t align_of_char      { alignof(char)      };
04:     size_t align_of_short     { alignof(short)     };
05:     size_t align_of_int       { alignof(int)       };
06:     size_t align_of_long      { alignof(long)      };
07:     size_t align_of_long_long { alignof(long long) };
08:     size_t align_of_float     { alignof(float)     };
09:     size_t align_of_double    { alignof(double)    };
10: 
11:     std::println("alignof (char)      {}", align_of_char);
12:     std::println("alignof (short)     {}", align_of_short);
13:     std::println("alignof (int)       {}", align_of_int);
14:     std::println("alignof (long)      {}", align_of_long);
15:     std::println("alignof (long long) {}", align_of_long_long);
16:     std::println("alignof (float)     {}", align_of_float);
17:     std::println("alignof (double)    {}", align_of_double);
18: }
```

*Ausf�hrung*:

```
alignof (char)      1
alignof (short)     2
alignof (int)       4
alignof (long)      4
alignof (long long) 8
alignof (float)     4
alignof (double)    8
```

### `alignas`

Legt die Anforderung an die Ausrichtung eines Typs oder Objekts im Speicher fest.

`alignas` ist ein *Spezifizierer*, d. h. er findet Anwendung zur �bersetzungszeit.


*Beispiel*:

```cpp
01: void test()
02: {
03:     alignas(16) int a {};
04:     alignas(1024) int b {};
05: 
06:     std::cout << &a << std::endl;
07:     std::cout << &b << std::endl;
08: 
09:     printBinary("a", &a);
10:     printBinary("b", &b);
11: }
```

*Ausf�hrung*:

```
00000092387BE810
00000092387BEC00
a: 0000000000000000000000001001001000111000011110111110100000010000
b: 0000000000000000000000001001001000111000011110111110110000000000
```

Man beachte bei der Ausgabe die letzten Stellen der Adressen in bin�rer Darstellung:
Bei einer Ausrichtungsanforderung von 16 (entspricht 2<sup>4</sup>) finden wir 4 Nullen am Ende vor,
bei der Ausrichtungsanforderung von 1024 (entspricht 2<sup>10</sup>) entsprechend 10 Nullen.

### `std::align`

Legt die Anforderung an die Ausrichtung eines Typs oder Objekts im Speicher fest.

`std::align` ist eine STL-Bibliotheksfunktion, sie berechnet ausgerichtete Adressen
in vorgegebenen Pufferbereichen zur Laufzeit.

```cpp
01: static void test_examine_alignment_07()
02: {
03:     std::byte buffer[32] = {};
04: 
05:     std::cout << "Buffer Address:                 " << buffer << std::endl;
06:     std::cout << "Buffer Size:                    " << sizeof(buffer) << std::endl;
07: 
08:     size_t alignof_int64_t{ alignof(int64_t) };     // requested alignment
09:     size_t sizeof_int64_t{ sizeof(int64_t) };       // requested storage size
10: 
11:     void* ptr{ buffer + 2 };     // not properly aligned for int64_t
12:     size_t size{ 32 - 2 };       // remaining space in place3
13: 
14:     std::cout << "Modified Buffer Address:        " << ptr << std::endl;
15: 
16:     size_t oldSize{ size };      // want to compare old size with new calculated size
17:     void* oldPtr{ ptr };         // want to compare old ptr with new calculated pointer
18: 
19:     void* p = std::align(
20:         alignof_int64_t,         // desired alignment
21:         sizeof_int64_t,          // size of the storage to be aligned
22:         ptr,                     // pointer to contiguous storage(a buffer) in which to operate
23:         size                     // buffer size
24:     );
25: 
26:     std::cout << "Aligned Buffer Address:         " << ptr << std::endl;
27:     std::cout << "Size of new Buffer :            " << size << std::endl;
28: 
29:     assert(p == ptr);
30: 
31:     auto diff = reinterpret_cast<uint8_t*>(ptr) - reinterpret_cast<uint8_t*>(oldPtr);
32: 
33:     std::cout << "Number of skipped Bytes:        " << diff << std::endl;
34: }
```

*Ausf�hrung*:

```
Buffer Address:                 000000A73F38F548
Buffer Size:                    32
Modified Buffer Address:        000000A73F38F54A
Aligned Buffer Address:         000000A73F38F550
Size of new Buffer :            24
Number of skipped Bytes:        6
```


### `std::max_align_t`


`std::max_align_t` beschreibt eine Ausrichtungsanforderung,
die mindestens so streng ist wie die von skalaren Typen.

Auf meinem Rechner ist `std::max_align_t` wie folgt definiert:

```cpp
using max_align_t = double; // most aligned type
```

Der typische Anwendungsfall im Gebrauch von `std::max_align_t`
besteht beim Deklarieren einer Variablen mit dem Spezifizierer `alignas`.
Hier kann man nun eine benutzerdefinierte Ausrichtungsanforderung angeben,
die strenger ist als die Standardausrichtung.

*Beispiel*:

```cpp
01: void test()
02: {
03:     alignas(std::max_align_t) char buffer[512] = {};  // typical use
04:     printBinary("a", buffer);
05: }
```

*Ausf�hrung*:

```
a: 0000000000000000000000001100101010001101000110111111001000111000
```

Man beachte wiederum die Anzahl der Nullen am Ende der Bin�rdarstellung.

---

## Der Heap (Halde) <a name="link5"></a>

---

## Placement new <a name="link6"></a>

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
und dann den separat Speicher freigeben.

C++17 f�hrt eine Reihe von Hilfsfunktionen in der STL (<memory>) ein,
um Objekte zu konstruieren und zu zerst�ren, ohne Speicher zuzuweisen oder freizugeben.

Anstatt *Placement new* zu verwenden, ist es jetzt also m�glich,
einige der Funktionen aus der STL (<memory>) zu verwenden,
deren Namen mit `std::uninitialized_` beginnen, um Objekte in einen nicht initialisierten Speicherbereich
zu konstruieren, zu kopieren und zu verschieben.

Und anstatt den Destruktor explizit aufzurufen, k�nnen wir jetzt `std::destroy_at()` verwenden,
um ein Objekt an einer bestimmten Speicheradresse zu zerst�ren, ohne den Speicher freizugeben.

Das vorherige Beispiel k�nnte mit diesen neuen Funktionen neu geschrieben werden:

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

Die Verwendung von `reinterpret_cast` und den hier gezeigten Speicherverwaltungsfunktionen
sollte in einer C++-Codebasis auf ein absolutes Minimum beschr�nkt werden.


---

## &bdquo;*Zeigerw�sche*&rdquo;: `std::launder` <a name="link7"></a>

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

  * Es �liegen gewisse �hnlichkeiten zum Schl�sselwort `volatile` vor:
  `volatile` dient zum Deaktivieren von Compiler-Annahmen dar�ber, was ein Wert zur Laufzeit sein k�nnte.
  Wenn Sie beispielsweise zweimal hintereinander von einer regul�ren `int`-Variablen lesen, ohne dazwischen zu schreiben,
  w�rde der Compiler wahrscheinlich den zweiten Lesevorgang entfernen und den ersten Wert wiederverwenden (bessere Codegenerierung),
  da er wei�, dass der Wert nicht ge�ndert wurde.

  * Mit dem Schl�sselwort `volatile` teilen Sie dem Compiler mit, dass er nicht alle �nderungen an der Variable zuverl�ssig beobachten kann,
  sodass jeder Lesevorgang ausgef�hrt werden muss (dasselbe gilt auch f�r Schreibvorg�nge).

  * `std::launder` und `volatile` sind sich insofern �hnlich, als sie existieren, um dem Compiler mitzuteilen,
  dass er keine Annahmen �ber die Werte/Objekte auf Grund m�glicher Beobachtungen treffen soll.

---

## Literatur <a name="link8"></a>

Die Anregungen zum Beispiel eines Arena finden Sie in einem Artikel von *Howard Hinnant* unter

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

---

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---
