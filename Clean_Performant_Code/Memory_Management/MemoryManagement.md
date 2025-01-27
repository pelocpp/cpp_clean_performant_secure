# Memory Management

[Zur�ck](../../Readme.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Der Stack (Stapel)](#link2)
  * [Gr��e des Stapels bestimmen](#link3)
  * [Speicher Ausrichtung (Memory Alignment)](#link4)
  * [Literatur](#link5)

---

#### Quellcode

[*MemoryManagement.cpp*](MemoryManagement.cpp).<br />

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

## Literatur <a name="link5"></a>

Die Anregungen zum Beispiel eines Arena finden Sie in einem Artikel von *Howard Hinnant* unter

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

---

[Zur�ck](../../Readme.md)

---
