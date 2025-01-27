# Memory Management

[Zurück](../../Readme.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Der Stack (Stapel)](#link2)
  * [Größe des Stapels bestimmen](#link3)
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
in welche Richtung er wächst, indem wir die Adresse der dem Stapel zugewiesenen Daten prüfen.

Das folgende Beispiel zeigt, auf welche Weise der Stapel beim Betreten und Verlassen von Funktionen wächst und schrumpft:

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

Ausgabe*:

```
main():  0x7ffd0e679874 - i: 0
func2(): 0x7ffd0e679854 - i: 2
func1(): 0x7ffd0e679834 - i: 1
func1(): 0x7ffd0e679854 - i: 1
```

Indem wir die Adresse der auf dem Stapel abgelegten `int`-Variable als ganze Zahl ausgeben,
können wir feststellen, wie stark und in welche Richtung der Stapel auf meinem Rechner wächst.

Wir beobachten jedes Mal 32 Bytes, wenn wir entweder `func1()` oder `func2()` aufrufen.
Die `int`-Variable `i`, die auf Stapel liegt, ist allerdings nur 4 Bytes (auf meinem Rechner) groß.

Die verbleibenden 28 Bytes enthalten Daten, die benötigt werden, wenn die Funktion endet, wie etwa die Rücksprungadresse und möglicherweise etwas *Padding*-Bytes zur Ausrichtung.

<img src="Cpp_Examine_Stack.svg" width="700">

*Abbildung* 1: Der Stapel wächst und schrumpft, wenn Funktionen aufgerufen und verlassen werden.

---

## Die Größe des Stapels bestimmen <a name="link3"></a>

Wir versuchen, ein Programm zu schreiben, das die Größe des Stacks auf meinem Rechner bestimmt.
Dies lässt sich im Rahmen einer Schätzung bewerkstelligen.

Wir schreiben zu diesem Zweck eine Funktion `func`, an die wir beim Aufruf die Adresse einer Variablen übergeben.
Die Funktion `func` selbst tut zwei Dinge: Zum einen allokiert sie pro Aufruf 1024 Bytes auf dem Stack,
zum anderen berechnet sie &ndash; pro rekursivem Aufruf &ndash; den Abstand dieses Felds zur ursprünglichen Variablen,
deren Adresse wir beim ersten Aufruf erhalten haben.

Auf diese Weise nähern wir uns der tatsächlichen Größe des Stapels, bis es auf Grund eines Stacküberlaufs
zu einem Absturz des Programms kommt:

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

Auf einem Windows Rechner erhalten wir folgende Ausgaben:


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

Unter Windows ist die Standardgröße des Stacks normalerweise auf 1 MB eingestellt.
Die Ausgaben des Programms bestätigen dies in etwa &ndash; der Wert 1,020,980 ist
nicht weit von 1,048,576 (1,024 * 1,024) entfernt.


---

## Speicher Ausrichtung (Memory Alignment) <a name="link4"></a>

Einige Begrifflichkeiten:

Jedem Objekt im Speicher besitzt eine so genannte *Ausrichtung* (Alignment),
die ihm durch Anforderungen des entsprechenden Typs auferlegt werden.

Die Ausrichtung ist immer eine Potenz von 2 und Objekte mit einer entsprechenden Ausrichtung
können immer nur an Speicheradressen platziert werden,
die ein Vielfaches dieser Ausrichtung sind.

### `std::alignment_of<T>::value` / `alignof()`

Liefern die Ausrichtung des Typs `T` bzw. des Typparameters von `alignof` zurück.

### `alignas`

Legt die Anforderung an die Ausrichtung eines Typs oder Objekts im Speicher fest.

 `alignas` ist ein *Spezifizierer*, d. h. er findet Anwendung zur Übersetzungszeit.

### `std::align`

Legt die Anforderung an die Ausrichtung eines Typs oder Objekts im Speicher fest.

`std::align` ist eine STL-Bibliotheksfunktion, sie berechnet ausgerichtete Adressen
in vorgegebenen Pufferbereichen zur Laufzeit.

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
alignas(std::max_align_t) char buffer[512] = {};
```


---

## Literatur <a name="link5"></a>

Die Anregungen zum Beispiel eines Arena finden Sie in einem Artikel von *Howard Hinnant* unter

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

---

[Zurück](../../Readme.md)

---
