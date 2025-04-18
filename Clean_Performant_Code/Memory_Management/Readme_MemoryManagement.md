# Memory Management

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Der Stack (Stapel)](#link2)
    * [Unterschiede des Stapels im Debug- und Relase-Modus](#link3) 
    * [Ausdehnung des Stapels](#link4)
    * [Gr��e des Stapels bestimmen](#link5)
  * [Ausrichtung von Variablen im Speicher (Memory Alignment)](#link6)
    * [`alignof()`](#link7)
    * [`alignas`](#link8)
    * [`std::align`](#link9)
    * [`std::max_align_t` ](link10)
  * [Padding](#link11)
  * [Der Heap (Halde)](#link12)
  * [Placement *new*](#link13)
  * [&bdquo;*Zeigerw�sche*&rdquo;: `std::launder`](#link14)
  * [Literatur](#link15)

---

#### Quellcode

[*MemoryManagement.cpp*](MemoryManagement.cpp)<br />

[*Arena.h*](Arena.h).<br />
[*Arena.cpp*](Arena.cpp).<br />

---

## Allgemeines <a name="link1"></a>

---

## Der Stack (Stapel) <a name="link2"></a>

### Unterschiede des Stapels im Debug- und Relase-Modus <a name="link3"></a>

Bei der Suche nach Fehlern versorgt uns die Visual Stdio IDE mit einer Reihe von unterst�tzenden Tools.
Ein simples Tool sind *Memory Windows*:

```cpp
01: int a = 1;
02: int b = 2;
03: int c = 3;
04: int d = 3;
05: 
06: int* ap = &a;
07: int* bp = &b;
08: int* cp = &c;
09: int* dp = &d;
10: 
11: unsigned long long d1 = (unsigned long long)bp - (unsigned long long)ap;
12: unsigned long long d2 = (unsigned long long)cp - (unsigned long long)bp;
13: unsigned long long d3 = (unsigned long long)dp - (unsigned long long)cp;
14: 
15: std::cout << "&a:  " << &a << std::endl;
16: std::cout << "&b:  " << &b << std::endl;
17: std::cout << "&c:  " << &c << std::endl;
18: std::cout << "&d:  " << &d << std::endl;
19: 
20: std::println("Diff a <=> b: {}", d1);
21: std::println("Diff b <=> c: {}", d2);
22: std::println("Diff c <=> d: {}", d3);
```

Die Inhalte der Variablen `a`, `b`, `c`, und `d` kann man in einem solchen *Memory Window* ansehen:

<img src="MemoryWindow.png" width="900">

*Abbildung* 1: &bdquo;*Memory*&rdquo;-Fenster f�r lokale Variablen.

Hmmm, f�llt Ihnen an der Ausgabe im Fenster etwas auf?
Richtig gesehen, die vier `int`-Variablen liegen in einem Abstand von 32-Bytes voneinander entfernt im Speicher.

Die Ausgaben in der Konsole best�tigen uns in dieser Vermutung:

```
&a:  0000003A9CEFF7E4
&b:  0000003A9CEFF804
&c:  0000003A9CEFF824
&d:  0000003A9CEFF844
Diff a <=> b: 32
Diff b <=> c: 32
Diff c <=> d: 32
```

Im Release-Modus haben die Variablen den erwarteten Abstand von 8 Bytes.
Dies l�sst sich nicht ohne Weiteres durch die Ausgaben best�tigen, da der Compiler/Optimizer
offensichtlich an der Reihenfolge der Variablen bzgl. ihrer Ablage im Speicher �nderungen vornimmt.
Damit werden die Ausgaben sinnlos,
dies best�tigt auch ein Blick in das Fenster der lokalen Variablen:

<img src="Debug_vs_Release_Mode.png" width="400">

*Abbildung* 2: Nicht jede im Programm deklarierte Variable findet Einzug in das endg�ltige Programm (*Release*-Modus).

Aber bleiben wir beim Debug-Modus. Warum ist das so? Hierzu finden sich wenige Hinweise im Netz,
immerhin gibt es bei [SO](https://stackoverflow.com/questions/60419126/why-does-vs-debug-build-allocates-variables-so-far-apart) eine Erkl�rung hierzu:

&bdquo;Die Debug-Version allokiert den Speicher anders als die Release-Version.
Insbesondere allokiert die Debug-Version zus�tzlich Speicherplatz am Anfang und Ende jedes Speicherblocks,
den eine Variable ben�tigt, und f�llt diesen mit einem bestimmten Allokationsmuster,
dass man auf etwaige Besch�digungen hin �berpr�fen kann&rdquo;.

In *Abbildung* 1 kann man erkennen, dass im &bdquo;toten&rdquo; Bereich der Wert `0xCC` vorhanden ist.

Die Speicherallokation erfolgt in quantisierten Bl�cken, wobei die L�nge eines Quantums nicht dokumentiert ist,
sie k�nnte 16 oder 32 Bytes betragen.

Wenn Sie beispielsweise vier `int`-Variablen auf dem Stack allokieren
(Gr��e = 4 * `sizeof(int)` Bytes = 4 * 4 = 16 Bytes),
belegt der Allokator im Debug-Modus tats�chlich 128 Bytes (vier 32-Byte Quanten).

Wenn Sie nun �ber den zul�ssigen Speicherplatz hinausschreiben
(das ginge in diesem Beispiel nur mit Typwandlungsoperationen),
�berschreiben Sie einen Teil des &bdquo;toten Speicherplatzes&rdquo; und besch�digen das vorhandene Bitmuster. 

Beim Verlassen eines Unterprogramms im Debug-Modus k�nnen Testroutinen des Debuggers �berpr�fen,
ob Besch�digungen in den flankierenden Speicherbereichen vorhanden sind und, wenn ja,
Laufzeitfehlermeldungen generieren.


### Ausdehnung des Stapels <a name="link4"></a>

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

*Abbildung* 3: Der Stapel w�chst und schrumpft, wenn Funktionen aufgerufen und verlassen werden.

---

### Die Gr��e des Stapels bestimmen <a name="link5"></a>

Wir versuchen, ein Programm zu schreiben, das die Gr��e des Stacks auf einem Rechner bestimmt.
Dies l�sst sich allerdings nur im Rahmen einer Sch�tzung durchf�hren.

Wir schreiben zu diesem Zweck eine Funktion `func`, an die wir beim Aufruf die Adresse einer Variablen �bergeben,
die auf dem Stack abgelegt ist.
Die Funktion `func` selbst tut zwei Dinge: Zum einen allokiert sie pro Aufruf 1024 Bytes auf dem Stack,
zum anderen berechnet sie &ndash; pro rekursivem Aufruf &ndash; den Abstand dieses Felds zur urspr�nglichen Variablen,
deren Adresse wir uns beim ersten Aufruf gemerkt haben und die wir an jeden Aufruf von `func` weiterreichen.

Auf diese Weise n�hern wir uns der tats�chlichen Gr��e des Stapels, bis es auf Grund eines Stack�berlaufs
zu einem Absturz des Programms kommt. Dies entspricht nicht ganz der feinen englischen Art,
ist aber eine unkonventionelle Vorgehensweise, um die ungef�hre Gr��e des Stapels zu berechnen.


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
Die Ausgaben des Programms best�tigen dies in etwa &ndash; der Wert 1.020.980 ist
nicht weit von 1.048.576 (1.024 * 1.024) entfernt.

---

## Ausrichtung von Variablen im Speicher (Memory Alignment) <a name="link6"></a>

Vorab einige Begrifflichkeiten:

Jedes Objekt im Speicher besitzt eine so genannte *Ausrichtung* (*Alignment*),
die ihm durch Anforderungen des entsprechenden Typs auferlegt werden.

Die Ausrichtung ist immer eine Potenz von 2 und Objekte mit einer entsprechenden Ausrichtung
k�nnen immer nur an Speicheradressen platziert werden,
die ein Vielfaches dieser Ausrichtung sind.

### `alignof()` <a name="link7"></a>

Liefert die Ausrichtung des Arguments (Datentyp) `alignof()` im Speicher zur�ck.

F�r elementare Datentypen `T` kann man die Aussage treffen, dass stets `sizeof(T)` gleich `alignof(T)` gilt:

*Beispiel*:

```cpp
01: void test() {
02: 
03:     std::println("alignof (char)        {}", alignof (char));
04:     std::println("alignof (short)       {}", alignof (short));
05:     std::println("alignof (int)         {}", alignof (int));
06:     std::println("alignof (long)        {}", alignof (long));
07:     std::println("alignof (long long)   {}", alignof (long long));
08:     std::println("alignof (float)       {}", alignof (float));
09:     std::println("alignof (double)      {}", alignof (double));
10:     std::println("alignof (long double) {}", alignof (long double));
11: }
```

*Ausf�hrung*:

```
alignof (char)        1
alignof (short)       2
alignof (int)         4
alignof (long)        4
alignof (long long)   8
alignof (float)       4
alignof (double)      8
alignof (long double) 8
```

Etwas komplizierter wird es, wenn wir die Ausrichtung von Strukturen betrachten,
siehe hierzu weiter unten den Abschnitt zu [Padding](#link11).


*Bemerkung*:

In der &bdquo;*Type Traits*&rdquo;-Metaprogramming Bibliothek der STL gibt es ein Klassentemplate `std::alignment_of<T>`.
Dessen Arbeitsweise ist weitestgehend identisch mit dem des Operators `alignof()`, es wird die Ausrichtung des Typs `T` zur�ckgeliefert.
Von Ausnahmen abgesehen kann man also immer mit dem Operator `alignof()` arbeiten.

### `alignas` <a name="link8"></a>

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

### `std::align` <a name="link9"></a>

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


### `std::max_align_t` <a name="link10"></a>


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

## Padding <a name="link11"></a>

In benutzerdefinierten Typen (Klassen, Strukturen) muss der �bersetzer manchmal zus�tzliche Bytes (Padding) hinzuf�gen.
Zun�cht einmal muss der �bersetzer die Elemente in der im Quellcode definierten Reihenfolge platzieren.

Er muss jedoch auch sicherstellen, dass die Datenelemente innerhalb der Klasse (Struktur) korrekt ausgerichtet sind.
Daher f�gt er bei Bedarf *Padding* (F�llbytes) zwischen den Datenelementen hinzu.

*Beispiel*:

```cpp
01: class Document
02: {
03:     bool       m_isCached;
04:     double     m_rank;
05:     int        m_id;
06: };
```

Mit der Anweisung

```cpp
std::println("sizeof(Document): {}", sizeof(Document));
```

erhalten wir die Ausgabe:

```
sizeof(Document): 24
```

Der Grund f�r die m�gliche Ausgabe von 24 liegt darin, dass der Compiler nach `bool` und `int` F�llbytes einf�gt,
um die Alignment-Anforderungen der einzelnen Datenelemente und der gesamten Klasse zu erf�llen.

Der Compiler konvertiert die Klasse `Dokument` in etwa wie folgt:


```cpp
01: class Document_Behind_the_Scenes {
02:     bool       m_isCached;
03:     std::byte  m_padding1[7]; // invisible padding inserted by compiler
04:     double     m_rank_;
05:     int        m_id_;
06:     std::byte  m_padding2[4]; // invisible padding inserted by compiler
07: };
```

Im Visual Stdio gibt es f�r Klassen einen &bdquo;*Memory Layout Viewer*&rdquo;, der uns von der Original-Klasse folgende Darstellung liefert:

<img src="Memory_Padding_01.png" width="600">

*Abbildung* 4: &bdquo;*Memory Layout Viewer*&rdquo; auf Klasse `Document` angewendet.



  * Der erste Abstand zwischen `bool` und `double` betr�gt 7 Byte, da das `m_rank`-Datenelement
  des Typs `double` eine Ausrichtung von 8 Byte hat.
  * Der zweite Abstand nach `int` betr�gt 4 Byte.
  * Dies ist erforderlich, um die Ausrichtungsanforderungen der Klasse `Document` selbst zu erf�llen.
  * Das Element mit der gr��ten Ausrichtungsanforderung bestimmt auch die Ausrichtungsanforderung f�r die gesamte Datenstruktur.

In unserem Beispiel bedeutet dies, dass die Gesamtgr��e der Klasse `Document` ein Vielfaches von 8 sein muss,
da sie einen `double`-Wert mit 8-Byte-Ausrichtung enth�lt.

Nat�rlich k�nnen wir die Reihenfolge der Datenelemente in der Klasse `Document` auch �ndern,
um die vom Compiler eingef�gten F�llbytes zu minimieren.

Wir beginnen hierzu am besten mit den Datentypen, die die gr��ten Ausrichtungsanforderungen besitzen.
Wir erstellen eine neue Version der Klasse `Document`:

```cpp
01: class Document_V2
02: {
03:     double     m_rank;
04:     int        m_id;
05:     bool       m_isCached;
06: };
```

<img src="Memory_Padding_01.png" width="600">

*Abbildung* 5: Die Datenelemente der `Document`-Klasse neu arrangiert.

Die Gr��e der neuen `Document`-Klasse betr�gt nun nur noch 16 Bytes, verglichen mit 24 Bytes in der ersten Version.

**Generell** gilt:<br />Die gr��ten Datenelemente sollten am Anfang und die kleinsten am Ende platziert werden.
Auf diese Weise l�sst sich der durch das Padding verursachte Speicheraufwand minimieren.


---

## Der Heap (Halde) <a name="link12"></a>

---

## Placement new <a name="link13"></a>

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

## &bdquo;*Zeigerw�sche*&rdquo;: `std::launder` <a name="link14"></a>

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


## Literatur <a name="link15"></a>

### Memory-Windows

Hinweise auf das Vorhandensein von Extra-Speicher im Debug-Modus zu Kontrollzwecken kann man hier nachlesen:

[Why does VS Debug build allocates variables so far apart?](https://stackoverflow.com/questions/60419126/why-does-vs-debug-build-allocates-variables-so-far-apart)

### Arena

Die Anregungen zum Beispiel einer Arena finden Sie in einem Artikel von *Howard Hinnant* unter

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

---

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---
