# Memory Management

[Zurück](../Performance_Optimization/Readme_Performance_Optimization.md)

---

## Inhalt

### [CPU-Cache-Speicher](Readme_Cache.md)

### [Der Stack (Stapel)](Readme_MemoryManagement_Stack.md)

### [Der Heap (Halde)](Readme_MemoryManagement_Heap.md)

### [Alignment und Padding (Ausrichtung und Abstand)](Readme_MemoryManagement_Alignment_Padding.md)

### [Placement New](Readme_MemoryManagement_Placement_New.md)

### [Literatur](XXX.md)

---



## Inhalt

  * [Allgemeines](#link1)

  * [Literatur](#link16)

---

#### Quellcode

[*MemoryManagement.cpp*](MemoryManagement.cpp)<br />

[*Arena.h*](Arena.h).<br />
[*Arena.cpp*](Arena.cpp).<br />


---

### TO BE DONE:

// Beachte:
// https://medium.com/@simontoth/daily-bit-e-of-c-std-make-unique-for-overwrite-std-make-shared-for-overwrite-and-34875c161fbd
// Simon Toth


// Beachte:
// https://stackoverflow.com/questions/21377360/proper-way-to-create-unique-ptr-that-holds-an-allocated-array



## Literatur <a name="link6"></a>

#### CPU-Cache-Speicher

Ein interessanter Artikel zum Thema *Demystifying CPU Caches with Examples*
findet sich [hier](https://mecha-mind.medium.com/demystifying-cpu-caches-with-examples-810534628d71).

Eine *Gallery of Processor Cache Effects* beschreibt *Igor Ostrovsky*
in seinem [Blog](https://igoro.com/archive/gallery-of-processor-cache-effects/).

Die Anregungen zur Berechnung der L1 Cache Größe finden Sie unter dem Github Gist<br />
[Get L1 data cache size on most operating systems](https://gist.github.com/kimwalisch/16c34ae16447b245464a)


---


## Literatur <a name="link16"></a>

### Memory-Windows

Hinweise auf das Vorhandensein von Extra-Speicher im Debug-Modus zu Kontrollzwecken kann man hier nachlesen:

[Why does VS Debug build allocates variables so far apart?](https://stackoverflow.com/questions/60419126/why-does-vs-debug-build-allocates-variables-so-far-apart)

### Arena

Die Anregungen zum Beispiel einer Arena finden Sie in einem Artikel von *Howard Hinnant* unter

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html)

###  *Low-level* Speicherverwaltungsfunktionen

Gute Beispiele und Beschreibungen der neuen *Low-level* Speicherverwaltungsfunktionen findet man unter

[Daily bit(e) of C++ | `std::uninitialized_copy`, `std::uninitialized_fill`, `std::uninitialized_move`, etc.](https://medium.com/@simontoth/daily-bit-e-of-c-std-uninitialized-copy-std-uninitialized-fill-std-uninitialized-move-f4a6b5afb7ba),<br />
verfasst von *Simon Toth*, also auch in einem Artikel von *Sandor Dargo*:

[The big STL Algorithms tutorial: the memory header](https://www.sandordargo.com/blog/2022/02/02/stl-alogorithms-tutorial-part-30-memory-header), verfasst von Simon Toth,

---

[Zurück](../Performance_Optimization/Readme_Performance_Optimization.md)

---
