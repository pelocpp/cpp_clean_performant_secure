# Die Halde (*Heap*)

[Zurück](Readme_MemoryManagement.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Weitere Charakteristika des Heap-Speichers](#link2)
  * [Vorteile der dynamischen Speicherverwaltung](#link3)

---

#### Quellcode

[*MemoryManagement_Stack.cpp*](MemoryManagement_Heap.cpp)<br />

---

## Allgemeines <a name="link1"></a>

Zwei wichtige Begriffe bei der statischen und dynamischen Speicherverwaltung sind *Stack* (*Stapel*) und *Heap* (*Halde*).

  * Stack und Heap sind zwei getrennte Speicherbereiche eines Programms während der Ausführung.

  * Die Zuweisung und Freigabe von Heap-Speicher durch den Programmierer / das Programm zur Laufzeit des Programms.

Im Vergleich dazu:

  * Die Zuweisung / Freigabe von Stack-Speicher erfolgt durch beim Laden eines Programms (*Loader*)
auf der Basis von Informationen, die der *Linker* in den Metadaten des Programms abgelegt hat.


---

## Weitere Charakteristika des Heap-Speichers <a name="link2"></a>

Mit der dynamischen Speicherverwaltung verwaltet der Programmierer den Heap.

  * Im Gegensatz zum linearen Stack kann der Heap als eine nicht-lineare, baumbasierte Datenstruktur angesehen werden.
  * Der Heap ist fragmentierbar (als Folge expliziter Zuweisungen und Freigabe von Speicherbereichen).
  * Heap-Variablen können in der Größe verändert werden (dies erfordert in der Regel eine Kombination von Freigabe und erneuter Allokation).
    Ein intuitives Verlängern am Ende eines bereits zugewiesenen Speicherbereichs ist in der Regel nicht möglich, da dieser Speicher anderweitig vergeben sein kann.
  * Speicherbereiche auf dem Heap obliegen keiner Reihenfolge, ihre Zuweisung kann in beliebiger Reihenfolge erfolgen.
  * Für jeden allokierten Speicherbereich ist eine Deallokation notwendig.
    Andernfalls würden auf dem Heap so genannte &bdquo;*Memory Leaks*&rdquo; entstehen.

---

## Vorteile der dynamischen Speicherverwaltung <a name="link3"></a>

Welche Vorteile bietet die dynamischen Speicherverwaltung?

> Sie ermöglicht die Verarbeitung von Datenmengen, deren Umfang erst zur Laufzeit bekannt ist.

---

## Unterschiedliche Vorteile der dynamischen Speicherverwaltung <a name="link3"></a>

In C++ gibt es `new` und `delete` in verschiedenen Formen, die jeweils einem anderen Zweck dienen.

Im Folgenden finden Sie eine kategorisierte Übersicht der gebräuchlichsten
(und einiger fortgeschrittener) Speicherbelegungs- und -freigabeformen vor:


## Stack versus Heap Speicher im Vergleich <a name="link3"></a>

| Beschreibung | Syntax |
|:-|:-|
| Allokation und Freigabe einer einzelnen Variable oder eines einzelnen Objekts | `new T` / `delete` |
| Allokation und Freigabe von Arrays | `new T[n]` / `delete[]` |
| *nothrow* `new` (Allokation, die keine Ausnahme wirft) | `new (std::nothrow) T` |
| *Placement New*: Trennung von Speicherallokation und Objektkonstruktion | `new (ptr) T` |
| Wert-Initialisierung vs. Default-Initialisierung | `new T()` bzw. `new T{}` |
| *Aligned* Allokation | `alignas(size)` |
| Klassenspezifische Allokation | `operator new` / `operator delete` |
| Globaler `operator new` / `operator delete` |  `operator new` / `operator delete`|

*Tabelle* 1: Unterschiedliche Anwendungsmöglichkeiten des `new`-Operators.

*Bemerkung*:<br />
Zum Thema &bdquo;*Placement New*: Trennung von Speicherallokation und Objektkonstruktion&rdquo;
gibt es [hier](Readme_MemoryManagement_Placement_New.md) einen eigenen Abschnitt.

Es folgen einige Beispiele zur Veranschaulichung der einzelnen Anwendungsmöglichkeiten des `new`-Operators
aus *Tabelle* 1:

### Allokation und Freigabe einer einzelnen Variable oder eines einzelnen Objekts

```cpp
01: class Foo
02: {
03: public:
04:     Foo(int x) {}
05: };
06: 
07: void test() {
08: 
09:     // allocating and deallocating a single variable or object
10: 
11:     int* p = new int(123);   // dynamically allocate one int
12:     delete p;                // deallocate it
13: 
14:     Foo* f = new Foo(123);
15:     delete f;
16: }
```

### Allokation und Freigabe von Arrays


```cpp
01: void test () {
02: 
03:     int* arr = new int[5];     // allocate array of 5 ints
04:     delete[] arr;              // must use delete[]
05: 
06:     // delete arr;             // WRONG — must use delete[] - UB (undefined behaviour)
07: 
08:     int* arr2 = new int[5] {}; // with initialization
09:     delete[] arr2;
10: 
11:     Foo* foos1 = new Foo[3];   // allocate an array of objects
12:     delete[] foos1;
13: 
14:     Foo* foos2 = new Foo[3]{ {1}, {2}, {3} };  // with initialization
15:     delete[] foos2;
16: }
```



### *nothrow* `new` (Allokation, die keine Ausnahme wirft)


```cpp
01: void test_03_nothrow_new_01() {
02: 
03:     int* p = new (std::nothrow) int;
04: 
05:     if (!p) {
06:         // allocation failed
07:     }
08: 
09:     delete p;
10: }
```

### Wert-Initialisierung vs. Default-Initialisierung (*Value-Initialization* vs. *Default-Initialization*)


```cpp
01: void test() {
02: 
03:     // default-Initialization
04:     int* p1 = new int;         // uninitialized value
05:     delete p1;
06: 
07:     // value-initialization
08:     int* p2 = new int{};       // initialized to 0
09:     delete p2;
10: 
11:     // default-Initialization
12:     Foo* f1 = new Foo;         // default constructor
13:     delete f1;
14: 
15:     // value-initialization
16:     Foo* f2 = new Foo{};       // also default constructor
17:     delete f2;
18: }
```

### *Aligned* Allokation


```cpp
01: struct alignas(64) Big {
02:     int x;
03: };
04: 
05: void test() {
06: 
07:     Big* ptr = new Big;
08:     std::println("ptr: {:#X}", reinterpret_cast<intptr_t>(ptr));
09: 
10:     // is the address really 64 bit aligned?
11:     // 64 bits == 8 bytes ==> last three bits of the address must be null
12:     if ((reinterpret_cast<uintptr_t>(ptr) & 0x7) != 0x0) {
13:         std::println("Address not 64 bit aligned !");
14:     }
15:     delete ptr;
16: }
```

### Klassenspezifische Allokation


```cpp
01: class AnotherFoo
02: {
03: public:
04:     static void* operator new(std::size_t size)
05:     {
06:         return std::malloc(size);
07:     }
08: 
09:     static void operator delete(void* ptr)
10:     {
11:         std::free(ptr);
12:     }
13: 
14:     static void* operator new[](std::size_t size)
15:     {
16:         return std::malloc(size);
17:     }
18: 
19:     static void operator delete[](void* ptr)
20:     {
21:         std::free(ptr);
22:     }
23: };
24: 
25: void test() {
26: 
27:     AnotherFoo* f = new AnotherFoo;
28:     delete f;
29: 
30:     AnotherFoo* foos = new AnotherFoo[3];
31:     delete[] foos;
32: }
```

### Globaler `operator new` / `operator delete`


```cpp
01: // ===================================================================
02: // Global operator new / operator delete
03: // Note: May onyl be declared within the global namespace
04: 
05: void* operator new(std::size_t size) {
06:     return std::malloc(size);
07: }
08: 
09: void operator delete(void* ptr) noexcept {
10:     std::free(ptr);
11: }
12: 
13: namespace MemoryManagement {
14: 
15:     void test() {
16: 
17:         int* p = new int(123);   // dynamically allocate one int
18:         delete p;                // deallocate it
19: 
20:         Foo* f = new Foo{ 123 }; // same for class type
21:         delete f;
22:     }
23: }
```

---

[Zurück](Readme_MemoryManagement.md)

---
