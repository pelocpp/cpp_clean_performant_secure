# STL-Container mit polymorphen Allokatoren

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

#### Quellcode

[*PMR_04.cpp*](PMR_04.cpp)<br />

---

### Allgemeines <a name="link1"></a>

Wir betrachten in diesem Abschnitt STL-Container mit polymorphen Allokatoren.

## Ein erstes Beispiel: Ein `std::vector`-Objekt mit einer `std::pmr::monotonic_buffer_resource`-Speicherresource

> Datei PMR_Allocator_10.cpp

In diesem Beispiel demonstrieren wir den Einsatz einer `std::pmr::monotonic_buffer_resource`-Speicherressource
in Verbindung mit einem `std::vector`-Objekt:

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 32;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
08: 
09:     std::pmr::monotonic_buffer_resource pool{
10:         g_memory.data(),
11:         g_memory.size(),
12:         std::pmr::null_memory_resource()
13:     };
14: 
15:     std::pmr::vector<std::uint8_t> vec{ &pool };
16: 
17:     for (std::int32_t i{}; i != 5; ++i) {
18:         vec.push_back(1 + i);
19:         dumpBuffer(std::span{ g_memory });
20:     }
21: }
```


*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X7FF6CE5DC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 40 f5 3f f9 86 00 00 00  00 00 00 00 00 00 00 00 | @.?..... ........
0010 | 01 ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF6CE5DC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 40 f5 3f f9 86 00 00 00  00 00 00 00 00 00 00 00 | @.?..... ........
0010 | 01 01 02 ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF6CE5DC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 40 f5 3f f9 86 00 00 00  00 00 00 00 00 00 00 00 | @.?..... ........
0010 | 01 01 02 01 02 03 ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF6CE5DC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 40 f5 3f f9 86 00 00 00  00 00 00 00 00 00 00 00 | @.?..... ........
0010 | 01 01 02 01 02 03 01 02  03 04 ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF6CE5DC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 40 f5 3f f9 86 00 00 00  00 00 00 00 00 00 00 00 | @.?..... ........
0010 | 01 01 02 01 02 03 01 02  03 04 01 02 03 04 05 ff | ........ ........
---------------------------------------------------------------------------
```

Die Ausgabe sollten wir nun etwas genauer betrachten: Wir erkennen von `push_back`- zu `push_back`-Aufruf
folgende Veränderungen in der Speicherressource:

```
0010 | 01 ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
```

gefolgt von


```
0010 | 01 01 02 ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
```

gefolgt von

```
0010 | 01 01 02 01 02 03 ff ff  ff ff ff ff ff ff ff ff | ........ ........
```

gefolgt von

```
0010 | 01 01 02 01 02 03 01 02  03 04 ff ff ff ff ff ff | ........ ........
```

und schließlich gefolgt von

```
0010 | 01 01 02 01 02 03 01 02  03 04 01 02 03 04 05 ff | ........ ........
```

Die Werte, die die `push_back`-Aufrufe im Speicher ablegen, kann man identifizieren.
Ferner kann man erkennen, dass ein anfänglich leeres `std::pmr::vector<std::uint8_t>`-Objekt
keinen Speicher für seine Daten vorab reserviert hat.
Dadurch lassen sich die permanenten Reallokationen und Umkopieroperationen erklären.

## Eine Korrektur des letzten Beispiels: Einsatz der Methode `reserve`

Die Reservierung des Speichers ist im letzten Beispiel etwas &bdquo;großzügig&rdquo; ausgefallen.
Mit einem Aufruf der `reserve`-Methode am `std::pmr::vector<T>`-Objekt kann man hier Abhilfe schaffen:


*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 32;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
08: 
09:     std::pmr::monotonic_buffer_resource pool{
10:         g_memory.data(),
11:         g_memory.size(),
12:         std::pmr::null_memory_resource()
13:     };
14: 
15:     std::pmr::vector<std::uint8_t> vec{ &pool };
16: 
17:     vec.reserve(5);
18: 
19:     for (std::int32_t i{}; i != 5; ++i) {
20:         vec.push_back(1 + i);
21:         dumpBuffer(std::span{ g_memory });
22:     }
23: }
```


*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X7FF614FCC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 90 f1 7a 4c 68 00 00 00  00 00 00 00 00 00 00 00 | ..zLh... ........
0010 | 01 ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF614FCC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 90 f1 7a 4c 68 00 00 00  00 00 00 00 00 00 00 00 | ..zLh... ........
0010 | 01 02 ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF614FCC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 90 f1 7a 4c 68 00 00 00  00 00 00 00 00 00 00 00 | ..zLh... ........
0010 | 01 02 03 ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF614FCC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 90 f1 7a 4c 68 00 00 00  00 00 00 00 00 00 00 00 | ..zLh... ........
0010 | 01 02 03 04 ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF614FCC260
Bytes:  32
---------------------------------------------------------------------------
0000 | 90 f1 7a 4c 68 00 00 00  00 00 00 00 00 00 00 00 | ..zLh... ........
0010 | 01 02 03 04 05 ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
```

Es ist klar erkennbar, dass die konsekutiven `push_back`-Aufrufe nicht zu Reallokationen im Speicher führen.

## Verschiedene STL Container &ndash; eine Speicherressource

Unterschiedliche Containertypen können dieselbe Speicherressource gemeinsam nutzen.
Dies demonstrieren wir an folgendem Beispiel, in dem

  * ein `std::pmr::vector<std::uint8_t>`-Objekt,
  * ein `std::pmr::list<std::uint8_t>`-Objekt und
  * ein `std::pmr::set<std::uint8_t>`-Objekt

dieselbe unterlagerte `std::pmr::monotonic_buffer_resource`-Speicherressource verwenden:

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 176;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
08: 
09:     std::pmr::monotonic_buffer_resource pool{
10:         g_memory.data(),
11:         g_memory.size(),
12:         std::pmr::null_memory_resource()
13:     };
14: 
15:     std::pmr::vector<std::uint8_t> vec{ &pool };
16:     std::pmr::list<std::uint8_t> list{ &pool };
17:     std::pmr::set<std::uint8_t> set{ &pool };
18: 
19:     dumpBuffer(std::span{ g_memory });
20: 
21:     vec.push_back('A');
22:     dumpBuffer(std::span{ g_memory });
23: 
24:     list.push_back('B');
25:     dumpBuffer(std::span{ g_memory });
26: 
27:     set.insert('C');
28:     dumpBuffer(std::span{ g_memory });
29: }
```


*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X7FF7AEA1C280
Bytes:  176
---------------------------------------------------------------------------
0000 | 90 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0010 | d0 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0020 | a0 c2 a1 ae f7 7f 00 00  a0 c2 a1 ae f7 7f 00 00 | ........ ........
0030 | ff ff ff ff ff ff ff ff  10 f2 8f 04 0f 00 00 00 | ........ ........
0040 | 00 00 00 00 00 00 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0050 | c8 c2 a1 ae f7 7f 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0060 | 01 01 ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0070 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0080 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0090 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
00a0 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF7AEA1C280
Bytes:  176
---------------------------------------------------------------------------
0000 | 90 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0010 | d0 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0020 | a0 c2 a1 ae f7 7f 00 00  a0 c2 a1 ae f7 7f 00 00 | ........ ........
0030 | ff ff ff ff ff ff ff ff  10 f2 8f 04 0f 00 00 00 | ........ ........
0040 | 00 00 00 00 00 00 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0050 | c8 c2 a1 ae f7 7f 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0060 | 01 01 ff ff ff ff ff ff  41 ff ff ff ff ff ff ff | ........ A.......
0070 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0080 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0090 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
00a0 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF7AEA1C280
Bytes:  176
---------------------------------------------------------------------------
0000 | 90 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0010 | d0 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0020 | f0 c2 a1 ae f7 7f 00 00  f0 c2 a1 ae f7 7f 00 00 | ........ ........
0030 | ff ff ff ff ff ff ff ff  10 f2 8f 04 0f 00 00 00 | ........ ........
0040 | 00 00 00 00 00 00 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0050 | c8 c2 a1 ae f7 7f 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
0060 | 01 01 ff ff ff ff ff ff  41 ff ff ff ff ff ff ff | ........ A.......
0070 | a0 c2 a1 ae f7 7f 00 00  a0 c2 a1 ae f7 7f 00 00 | ........ ........
0080 | 42 ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | B....... ........
0090 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
00a0 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
===========================================================================
Memory Dump
Buffer: 0X7FF7AEA1C280
Bytes:  176
---------------------------------------------------------------------------
0000 | 90 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0010 | d0 f1 8f 04 0f 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0020 | f0 c2 a1 ae f7 7f 00 00  f0 c2 a1 ae f7 7f 00 00 | ........ ........
0030 | ff ff ff ff ff ff ff ff  10 f2 8f 04 0f 00 00 00 | ........ ........
0040 | 00 00 00 00 00 00 00 00  08 c3 a1 ae f7 7f 00 00 | ........ ........
0050 | 08 c3 a1 ae f7 7f 00 00  08 c3 a1 ae f7 7f 00 00 | ........ ........
0060 | 01 01 ff ff ff ff ff ff  41 ff ff ff ff ff ff ff | ........ A.......
0070 | a0 c2 a1 ae f7 7f 00 00  a0 c2 a1 ae f7 7f 00 00 | ........ ........
0080 | 42 ff ff ff ff ff ff ff  c8 c2 a1 ae f7 7f 00 00 | B....... ........
0090 | c8 c2 a1 ae f7 7f 00 00  c8 c2 a1 ae f7 7f 00 00 | ........ ........
00a0 | 01 00 43 ff ff ff ff ff  ff ff ff ff ff ff ff ff | ..C..... ........
---------------------------------------------------------------------------
```

Die Ausgabe ist dieses Mal leider etwas umfangreicher. Vor allem die beiden STL-Container
`std::list` und `std::set` benötigen intern zusätzlichen Speicher für etwaige &bdquo;Knoten&rdquo;-Strukturen (*Nodes*),
mit Hilfe derer die jeweilige Datenstruktur konstruiert wird.

Beim zweiten, dritten und vierten Dump kann man aber erkennen, dass der jeweilige Wert (`A`, `B` bzw. `C`) im Speicher vorhanden ist.

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---
