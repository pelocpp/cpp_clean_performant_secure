# Agenda Seminar &bdquo;Performanz und Optimierungstechniken für C++ Entwickler&rdquo;

[Zurück](./Readme_Performance_Optimization.md)

---

## Einführung in die Performance-Optimierung

  * Bedeutung der Performance-Optimierung
  * Übersetzungszeit (*Compile-Time*) versus Laufzeit (*Run-Time*)


## Speicherverwaltung (Memory Management)

  * Speicherbereiche eine C/C++&ndash;Programms
  * Stack versus Heap
  * CPU-Cache-Speicher
  * Ausrichtung (*Alignment*) und *Padding* 
  * *Placement New*: Trennung von Speicherallokation und Objektkonstruktion


## Einfache Code-Optimierungstechniken

  * Klasse `std::vector`: Bedeutung der `reserve`-Methode
  * Wozu Klasse `std::initializer_list` verwenden?
  * SSO (*Small String Optimization*)
  * Wozu Klasse `std::string_view` verwenden?
  * `constexpr` &ndash; *Compile-Time*&ndash;Programmierung
  * IIFE &ndash; *Immediately Invoked Functional Expression*
  * *Copy*/*Move* Elision
  * Statischer versus dynamischer Polymorphismus


## Fortgeschrittene Code-Optimierungstechniken

  * &bdquo;*Folding*&rdquo;-Ausdrücke
  * Verschieben ist besser als Kopieren (`std::move`)
  * Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`)
  * CRTP &ndash; Curiously Recurring Template Pattern


## Optimierung von Algorithmen und Datenstrukturen

  * *Big-O* Notation
  * Verwenden Sie Container und Algorithmen der C++&ndash;Bibliothek STL
  * Die richtige Auswahl des Containers
  * Die richtige Auswahl eines Algorithmus


## Multithreading / Concurrency

  * Grundlagen des Multithreading
  * Verwenden von Threads und elementare Synchronisationsmechanismen
  * `std::async` und `std::future`
  * Parallelisierung von STL-Algorithmen (`std::execution::parallel_policy`)


## Tools zur Performance-Messung

  * Quick C++ Benchmark
  * Visual Studio Diagnostic Tools
  * Visual Studio Analysis Tools (CPU Usage, Memory Usage und weitere)

 
## Funktionale Programmierung: Die Bibliothek `std::ranges`

  * Was ist funktionale Programmierung?
  * Funktionale Programmierung in modernem C++
    * Lambda-Ausdrücke
    * Generische Lambda-Ausdrücke
  * Funktionen höherer Ordnung
    * *Map*, *Filter* und *Reduce*
  * Pipelining mit Bereichs-Adaptern


---

[Zurück](./Readme_Performance_Optimization.md)

---
