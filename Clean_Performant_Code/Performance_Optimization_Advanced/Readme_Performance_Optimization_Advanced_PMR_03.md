# Polymorphe Allokatoren

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Ein Beispiel zur Klasse `std::pmr::polymorphic_allocator`](#link2)
  * [Zweites Beispiel zur Klasse `std::pmr::polymorphic_allocator`](#link3)

---

#### Quellcode

[*PMR_03.cpp*](PMR_03.cpp)<br />

---

### Allgemeines <a name="link1"></a>

Die Klasse `std::pmr::polymorphic_allocator` dient als Standardschnittstelle,
um die Lücke zwischen den statisch typisierten Containern der STL und den dynamischen Speicherressourcen zu schließen.
Die STL-Containerklassen erwarten einen Allokatortyp, dies kann die Klasse `std::pmr::polymorphic_allocator` sein.

`std::pmr::polymorphic_allocator`-Objekte

  * halten einen Zeiger auf ein `std::pmr::memory_resource`-Objekt.
  * leiten `allocate()`- und `deallocate()`-Aufrufe an das unterlagerte Speicherressourcenobjekt weiter.
 

Die konzeptionelle Rolle lässt sich folgendermaßen darstellen:

STL Container<br />&nbsp;&nbsp;&nbsp;&Rightarrow;
polymorpher Allokator (`std::pmr::polymorphic_allocator`)<br />&nbsp;&nbsp;&nbsp;&Rightarrow;
Speicherressource (`std::pmr::memory_resource`)<br />&nbsp;&nbsp;&nbsp;&Rightarrow;
eigentliche Speicherquelle.

## Ein Beispiel zur Klasse `std::pmr::polymorphic_allocator` <a name="link2"></a>

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 16;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
08: 
09:     std::pmr::monotonic_buffer_resource pool{ g_memory.data(), g_memory.size() };
10: 
11:     std::pmr::polymorphic_allocator<std::int32_t> allocator(&pool);
12: 
13:     std::int32_t* ptr{ nullptr };
14: 
15:     ptr = allocator.allocate(1);
16:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
17:     *ptr = 'A';
18: 
19:     ptr = allocator.allocate(1);
20:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
21:     *ptr = 'B';
22: 
23:     ptr = allocator.allocate(1);
24:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
25:     *ptr = 'C';
26: 
27:     std::span span{ g_memory };
28:     dumpBuffer(span);
29: }
```

Man beachte, wie sich bei Gebrauch eines `std::pmr::polymorphic_allocator`-Objekts die `allocate`-Aufrufe
von denen eines `std::pmr::memory_resource`-Objekts unterscheiden: In unserem Beispiel
ist die  `std::pmr::polymorphic_allocator<T>`-Klasse mit dem Datentyp `T` gleich `std::int32_t` instanziiert.
Damit bedeutet eine `1` als Parameter beim Aufruf von `allocate`, dass Speicher für eine Variable vom Typ
`std::int32_t` zu reservieren ist. Eine Umrechnung des Datentyps in die Anzahl der zu reservierenden Bytes
als auch die Betrachtung des Alignments sind hier nicht mehr vorhanden.

*Ausgabe*:

```
Pointer to allocated storage: 0X7FF6A37123E0
Pointer to allocated storage: 0X7FF6A37123E4
Pointer to allocated storage: 0X7FF6A37123E8
===========================================================================
Memory Dump
Buffer: 0X7FF6A37123E0
Bytes:  16
---------------------------------------------------------------------------
0000 | 41 00 00 00 42 00 00 00  43 00 00 00 ff ff ff ff | A...B... C.......
---------------------------------------------------------------------------
```

## Zweites Beispiel zur Klasse `std::pmr::polymorphic_allocator` <a name="link3"></a>

Wir wiederholen im Prinzip das Beispiel aus dem letzten Abschnitt noch einmal.
Nur dieses Mal instanziieren wir die `std::pmr::polymorphic_allocator`-Klasse mit dem Datentyp  `T` gleich `std::uint8_t`:

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 16;
02: 
03: std::array<std::uint8_t, NumBytes> g_smallBuffer{};
04: 
05: void test_pmr_10_02()
06: {
07:     // needing buffer size 16
08: 
09:     std::fill(
10:         g_smallBuffer.begin(), 
11:         g_smallBuffer.end(), 
12:         std::numeric_limits<std::uint8_t>::max()
13:     );
14: 
15:     std::pmr::monotonic_buffer_resource pool{
16:         g_smallBuffer.data(), 
17:         g_smallBuffer.size(),
18:         std::pmr::null_memory_resource()
19:     };
20: 
21:     std::pmr::polymorphic_allocator<std::uint8_t> allocator(&pool);
22: 
23:     std::uint8_t* ptr{ nullptr };
24: 
25:     ptr = allocator.allocate(1);
26:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
27:     *ptr = 'A';
28: 
29:     ptr = allocator.allocate(1);
30:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
31:     *ptr = 'B';
32: 
33:     ptr = allocator.allocate(1);
34:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
35:     *ptr = 'C';
36: 
37:     dumpBuffer(std::span{ g_smallBuffer });
38: }
```

*Ausgabe*:

```
Pointer to allocated storage: 0X7FF6CD8F23E0
Pointer to allocated storage: 0X7FF6CD8F23E1
Pointer to allocated storage: 0X7FF6CD8F23E2
===========================================================================
Memory Dump
Buffer: 0X7FF6CD8F23E0
Bytes:  16
---------------------------------------------------------------------------
0000 | 41 42 43 ff ff ff ff ff  ff ff ff ff ff ff ff ff | ABC..... ........
---------------------------------------------------------------------------
```

An der Ausgabe lässt sich nun erkennen, wie die Daten (`std::uint8_t`-Variablen)
im Speicher dicht aneinander liegen.

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---
