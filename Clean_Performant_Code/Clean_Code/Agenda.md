# Agenda Seminar &bdquo;Modern C++&rdquo;

[Zurück](./Readme_Clean_Code.md)

---

## Prinzipientreue

  * KISS (*Keep it simple and stupid*)
  * YAGNI (*You Aren't Gonna Need It*)
  * DRY (*Don't Repeat Yourself*)
  * POLS (*Principle Of Least Surprise*)
  * Datenkapselung (*Information Hiding*)
  * Starker Zusammenhalt (*Strong Cohesion*)
  * Lose Kopplung (*Lose Coupling*)

## Einfache Konzepte von Clean C++

  * Namensgebung für Bezeichner
  * Kommentare
  * Funktionen
  * Old C-Style in einem C++ Projekt


## Fortgeschrittene Konzepte von Clean C++

  * Ressourcenverwaltung mit RAII
  * Nutzung von Smart Pointer Klassen (`std::unique_ptr`, `std::shared_ptr` und `std::weak_ptr`)
  * Verschieben ist besser als Kopieren (`std::move`)
  * Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`)
  * Vorteile von `auto` (*Automatic Type Deduction*)
  * Vermeide UB (*Undefined Behaviour*)
  * Typreiche Programmierung (Type-Rich Programming, Type Traits)
  * Statischer vs. dynamischer Polymorphismus
  * Verwenden Sie Bibliotheken (STL)
  * Richtige Ausnahme- und Fehlerbehandlung
  * Design von Methoden-/Funktionsschnittstellen (Attribute)
  * Verwendung von Konzepten (`concept`) und Anforderungen (`requires`)


## Einige Design Patterns (Entwurfsmuster) und Idiome
  * Das SOLID Idiom
    * Single Responsibility Prinzip (SRP)
    * Open-Closed Prinzip (OCP)
    * Liskovsches Substitutionsprinzip (LSP)
    * Interface Segregationsprinzip (ISP)
    * Dependency Inversion Prinzip (DIP)
  * Einige Muster und wann sie zu verwenden sind
    * Adapter
    * Strategy
    * Virtueller Konstruktor
    * Observer
    * Factories
    * Visitor

## Modularisierung
  * Objektorientierung
    * Objektorientiertes Denken
    * Prinzipien für gutes Klassendesign
  * Module
    * Die Nachteile von #include 
    * Module als Rettung
    * Unter der Haube

## Funktionale Programmierung

  * Was ist funktionale Programmierung?
  * Funktionale Programmierung in modernem C++
    * Funktionale Programmierung mit C++-Templates
    * Funktionsähnliche Objekte (Funktoren)
    * Lambda-Ausdrücke
    * Generische Lambda-Ausdrücke
    * Lambda-Vorlagen
  * Funktionen höherer Ordnung
    * Map, Filter und Reduce
  * Pipelining mit Bereichs-Adaptern
  * Sauberer Code in der funktionalen Programmierung


## Notwendigkeit des Testens

  * Unit-Tests
    * Regeln für gute Unit-Tests
    * Test Codequalität
    * Unit-Test-Benennung
    * Unit-Test-Unabhängigkeit
  * Beispiel: Die Bibliothek Catch2


---

[Zurück](./Readme_Clean_Code.md)

---
