# Ein Laufzeitvergleich: Vektoren mit Zeichenketten

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Ein Performanzvergleich](#link2)

---

#### Quellcode

[*PMR_08.cpp*](PMR_08.cpp)<br />

---

### Allgemeines <a name="link1"></a>

Wir wollen an einem einfachen Vergleich die Laufzeiten zweier Programme mit und ohne
polymorphe Speicherverwaltung vergleichen.

Betrachtet werden die zwei STL Container `std::vector<std::string>` bzw. `std::pmr::vector<std::pmr::string>`.

Sie werden mit Zeichenketten zufällig berechneter Längen bestehend aus dem Zeichen `a` gefüllt.

Ein `TrackingResource`-Objekt stellt sicher, dass im Beispiel mit polymorpher Speicherverwaltung
die Daten auch tatsächlich aus einer `std::pmr::monotonic_buffer_resource`-Speicherresource entnommen werden.

Auf diese Weise vergleichen wir damit die klassischen Speicherverwaltungsmethoden `new` und `delete`
mit denen eines `std::pmr::monotonic_buffer_resource`-Objekts: `allocate` und `deallocate`.
Da hier speziell die `deallocate`-Methode ein No-Op ist, sollten wir in der Laufzeit einen Unterschied beobachten.

## Ein Performanzvergleich <a name="link2"></a>

Die beiden Methoden, deren Laufzeit wir messen, sehen so aus:

```cpp
01: void benchmark_std()
02: {
03:     for (std::size_t n{}; n != Iterations; ++n)
04:     {
05:         std::vector<std::string> vec;
06:         vec.reserve(VectorCapacity);
07: 
08:         for (std::size_t i{}; i != VectorCapacity; ++i)
09:         {
10:             std::size_t len{ random_length() };
11: 
12:             std::string& s{ vec.emplace_back() };
13: 
14:             s.reserve(len);
15: 
16:             for (std::size_t j{}; j != len; ++j) {
17:                 s.push_back('a');
18:             }
19:         }
20: 
21:         vec.clear();
22:     }
23: }
```

und

```cpp
01: void benchmark_pmr()
02: {
03:     // track heap usage
04:     TrackingResource tracker{ std::pmr::new_delete_resource() };
05: 
06:     for (std::size_t n{}; n != Iterations; ++n)
07:     {
08:         std::pmr::monotonic_buffer_resource pool{ g_memory.data(), g_memory.size(), &tracker };
09: 
10:         std::pmr::vector<std::pmr::string> vec{ &pool };
11: 
12:         vec.reserve(VectorCapacity);
13: 
14:         for (std::size_t i{}; i != VectorCapacity; ++i)
15:         {
16:             std::size_t len{ random_length() };
17: 
18:             std::pmr::string& s{ vec.emplace_back() };
19: 
20:             s.reserve(len);
21: 
22:             for (std::size_t j{}; j != len; ++j) {
23:                 s.push_back('a');
24:             }
25:         }
26: 
27:         vec.clear();
28:     }
29: 
30:     std::println("Heap Allocations:     {}", tracker.getAllocations());
31:     std::println("Heap Deallocations:   {}", tracker.getDeallocations());
32:     std::println("Heap Bytes allocated: {}", tracker.getBytesAllocated());
33: }
```


Das `TrackingResource`-Objekte würde Aufrufe an die `allocate`- und `deallocate`-Methoden mitzählen,
sofern diese Methoden überhaupt aufgerufen würden.

*Ausgabe*:

```
Start
Elapsed time: 1141 milliseconds.
Heap Allocations:     0
Heap Deallocations:   0
Heap Bytes allocated: 0
Elapsed time: 902 milliseconds.
Done.
```

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---
