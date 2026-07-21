# Agenda Seminar &bdquo;Clean Code C++&rdquo;

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

## Richtlinien Clean Code &bdquo;Klassisches C++&rdquo;

  * Namensgebung für Bezeichner
  * Kommentare
  * Old C-Style in einem C++ Projekt
  * Organisation von Include-Files
  * Initialisierung von Strukturen und Objekten
  * Warum Schlüsselwort `explicit`?
  * Schlüsselwort `auto` verwenden oder nicht?
  * Schlüsselwort `auto`: Left-to-Right Initialisierungsstil
  * `if`- und `for`-Anweisungen mit Initialisierer

## Richtlinien Clean Code &bdquo;Modernes C++&rdquo;

  * Ressourcenverwaltung mit RAII
  * Nutzung von Smart Pointer Klassen (`std::unique_ptr`, `std::shared_ptr` und `std::weak_ptr`)
  * Verwenden Sie Container und Algorithmen der C++&ndash;Bibliothek STL
  * C-Style Arrays oder `std::array` zusammen mit `std::span`?
  * Wozu gibt es den &bdquo;*lightweight*&rdquo; Container `std::initializer_list`?
  * Welche Vorteile bietet die Klasse `std::string_view` gegenüber `std::string`?
  * Verschieben ist besser als Kopieren (`std::move`)
  * *Rule-of-Zero*
  * *Copy-Swap* Idiom
  * Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`)
  * Vorteile von `auto` (*Automatic Type Deduction*)
  * Bessere Lesbarkeit durch *Structured Binding*
  * Vorteile von Lambda-Objekten
  * Verwendung von Utility-Klassen (`std::variant`, `std::tuple`, `std::any`, etc.)
  * Monadische Schnittstellen zur Vereinfachung von Fehlerüberprüfungen (`std::optional`, `std::expected`)
  * Typreiche Programmierung (*Type-Rich Programming*, *Type Traits*)
  * *Strong Types*
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
    * Die Nachteile von `#include` 
    * Module als Rettung
    * Unter der Haube

## Funktionale Programmierung: Die `std::ranges` Bibliothek

  * Was ist funktionale Programmierung?
  * Funktionale Programmierung in modernem C++
    * Lambda-Ausdrücke
    * Generische Lambda-Ausdrücke
  * Funktionen höherer Ordnung
    * Map, Filter und Reduce
  * Pipelining mit Bereichs-Adaptern
  * Sauberer Code in der funktionalen Programmierung

## Notwendigkeit des Testens

  * Unit-Tests
    * Regeln für gute Unit-Tests
    * Test Codequalität
    * Unit-Test-Benennung
  * Beispiel: Die Bibliothek *Catch2*

---

[Zurück](./Readme_Clean_Code.md)

---
