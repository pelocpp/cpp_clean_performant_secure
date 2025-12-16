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

Bemerkung:<br />
Hiermit sind **nicht** die so benannten Datenstrukturen wie etwa der Container `std::stack` und die Funktion `std::make_heap` aus der STL gemeint.

  * Stack und Heap sind zwei getrennte Speicherbereiche eines Programms während der Ausführung.

  * Die Zuweisung und Freigabe von Heap-Speicher durch den Programmierer / das Programm zur Laufzeit des Programms.

Im Vergleich dazu:

  * Die Zuweisung / Freigabe von Stack-Speicher erfolgt durch beim Laden eines Programms (*Loader*)
auf der Basis von Informationen, die der *Linker* in den Metadaten des Programms abgelegt hat.


---

## Weitere Charakteristika des Heap-Speichers <a name="link2"></a>

Mit der dynamischen Speicherverwaltung verwaltet der Programmierer den Heap.

  * Im Gegensatz zum linearen Stack kann der Heap als eine nicht-lineare, baumbasierte Datenstruktur angesehen werden.
  * Der Heap ist fragmentierbar (explizite Zuweisungen und Freigabe von Speicherbereichen).
  * Heap-Variablen können in der Größe verändert werden (dies erfordert in der Regel eine Kombination von Freigabe und erneuter Allokation).
    Ein intuitives Verlängern am Ende eines bereits zugewiesenen Speicherbereichs ist in der Regel nicht möglich, da dieser Speicher anderweitig vergeben sein kann.
  * Speicherbereiche auf dem Heap obliegen keiner Reihenfolge, ihre Zuweisung kann in beliebiger Reihenfolge erfolgen.
  * Für jeden allokierten Speicherbereich ist eine Deallokation notwendig.
    Andernfalls entstehen auf dem Heap so genannte &bdquo;*Memory Leaks*&rdquo;.

---

## Vorteile der dynamischen Speicherverwaltung <a name="link3"></a>

Welche Vorteile bietet die dynamischen Speicherverwaltung?

> Sie ermöglicht die Verarbeitung von Datenmengen, deren Umfang erst zur Laufzeit bekannt ist.

---

## Unterschiedliche Vorteile der dynamischen Speicherverwaltung <a name="link3"></a>

In C++ gibt es `new` und `delete` in verschiedenen Formen, die jeweils einem anderen Zweck dienen.

Im Folgenden finden Sie eine kategorisierte Übersicht der gebräuchlichsten (und einiger fortgeschrittener) Speicherbelegungs- und -freigabeformen
mit anschaulichen Beispielen vor:

### Zuweisung und Freigabe einer einzelnen Variable oder eines einzelnen Objekts

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

### Zuweisung und Freigabe von Arrays









---

[Zurück](Readme_MemoryManagement.md)

---
