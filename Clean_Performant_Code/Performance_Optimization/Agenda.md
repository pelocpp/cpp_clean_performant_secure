# Agenda Seminar &bdquo;Performanz und Optimierungstechniken für C++ Entwickler&rdquo;

[Zurück](./Readme_Performance_Optimization.md)

---

## Einführung in die Performance-Optimierung

  * Bedeutung der Performance-Optimierung
  * Übersetzungszeit (*Compile-Time*) versus Laufzeit (*Run-Time*)


## Speicherverwaltung (Memory Management)

  * Speicherbereiche eine C/C++&ndash;Programms
  * Globales Datensegment, Stack und Heap
  * CPU Cache-Speicher
  * Ausrichtung (*Alignment*) und *Padding* 
  * *Placement New*: Trennung von Speicherallokation und Objektkonstruktion


## Einfache Code-Optimierungstechniken

  * *Call*-*by*-*Value* versus *Call*-*by*-*Reference* bei elementaren Datentypen
  * *Call*-*by*-*Value* versus *Call*-*by*-*Reference* bei nicht-elementaren Datentypen
  * *getter*-Methoden von Klassen: Rückgabetyp *By*-*Value* oder *By*-*Reference*
  * Klassen (Strukturen) und die Ausrichtung der Daten: Je kleiner, desto besser 
  * Klasse `std::vector`: Bedeutung der `reserve`-Methode
  * Wozu Klasse `std::initializer_list` verwenden?
  * Klasse `std::string`: SSO (*Small String Optimization*)
  * Wozu Klasse `std::string_view` verwenden?
  * `constexpr` &ndash; *Compile-Time*&ndash;Programmierung
  * IIFE &ndash; *Immediately Invoked Functional Expression*
  * *Copy*/*Move* Elision
  * Die Technik der `emplace`-Methoden


## Fortgeschrittene Code-Optimierungstechniken

  * &bdquo;*Folding*&rdquo;-Ausdrücke
  * Verschieben ist besser als Kopieren (`std::move`)
  * Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`)
  * CRTP &ndash; Curiously Recurring Template Pattern
  * Statischer versus dynamischer Polymorphismus
  * `std::variant`, `std::visit` und statische Methodenaufrufe


## Optimierung von Algorithmen und Datenstrukturen

  * *Big-O* Notation
  * Verwenden Sie Container und Algorithmen der C++&ndash;Bibliothek STL
  * Die richtige Auswahl des Containers
  * Die richtige Auswahl eines Algorithmus
  * STL-Algorithmen und generische Lambda-Objekte
  * STL-Algorithmen versus klassische Wiederholunganweisungen


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
