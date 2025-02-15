# Secure Programming Advices

[Zurück](../../Markdown/Readme_Performance_Optimization.md)

---

## Inhalt

  * [Don't use C &ndash; Use C++](#link)
  * [Do memory allocations with new/delete](#link)


  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [Literatur](#link6)

---

## Don't use C &ndash; Use C++

Sollte man die Wahl haben: Verwenden Sie an Stelle von C C++.

Warum: C++ bietet bessere Typprüfungsmöglichkeiten und attraktivere Möglichkeiten in der Formulierung von Quellcode.
Es bietet bessere Unterstützung für die Programmierung auf höherer Ebene
und generiert häufig schnelleren Code.

Diese Aussage findet sich auch in den [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
wieder: *Prefer C++ to C* (*CPL.1*).

Als Illustration vergleiche man das Konkatenieren von zwei Zeichenketten:
Einmal in C++ geschrieben und ein zweites Mal in C:

```c
01: std::string first{ "Hello " };
02: std::string second{ "World" };
03: std::string result{ first + second };
04: std::println("{}", result);
```

```cpp
01: char first[] = "Hello ";
02: char second[] = "World";
03: char result[20];
04: 
05: strcpy_s(result, 20, first);
06: strcat_s(result, 20, second);
07: std::printf("%s\n", result);
```

---

## Verwenden Sie STL-Algorithmen

Algorithmen der STL sind robuster im Gebrauch als CRT-Bibliotheksfunktionen oder selbst geschriebene Funktionen.

*Beispiel*:

```cpp
01: std::string str{ "Hello World" }; // use also "Hello:World"
02: std::println("{}", str);
03: 
04: auto isColon = [] (int ch) { return ch == ':'; };
05: 
06: auto pos = std::find_if(str.begin(), str.end(), isColon);
07: 
08: // if found - delete everything afterwards
09: if (pos != str.end()) {
10:     str.erase(pos, str.end());
11: }
12: 
13: std::println("{}", str);
```


*Beispiel*:

```c
01: char str[] = "Hello World";    // use also "Hello:World"
02: std::printf("%s\n", str);
03: 
04: const char* pos = strstr(str, ":");
05: 
06: if (pos != NULL) {
07: 
08:     // if found - poke '\0' into string - this deletes everything afterwards
09:     size_t ofs = pos - str;
10:     str[ofs] = '\0';
11: }
12: 
13: std::printf("%s\n", str);
```

*Hinweis*:
Vermeiden Sie generell *manuelle* Wiederholungsschleifen!

Verwenden Sie Algorithmen der STL oder die C++ *Range-based Loop*:
Beide kommen ohne Schleifen-Indices aus, die eine häufige Ursache für Fehlerquellen sind.


---

## Sicheres Downcasting

An Stelle der unsicheren C-Cast Operatoren sollten man die sicheren C++-Cast Operatoren verwenden.

Ein Beispiel ist der Gebrauch von `static_cast`:


*Beispiel*:

```cpp
01: class Spiderman {};
02: class Ironman {};
03: 
04: void test() {
05: 
06:     Spiderman* ptr = new Spiderman;
07:     Ironman* ptr2 = nullptr;
08: 
09:     // compile error: 'static_cast': cannot convert from 'Spiderman *' to 'Ironman *'
10:     ptr2 = static_cast<Ironman*>(ptr);
11: }
```

*Beispiel*:

```c
01: class Spiderman {};
02: class Ironman {};
03: 
04: void test() {
05: 
06:     Spiderman* ptr = new Spiderman;
07:     Ironman* ptr2 = NULL;
08: 
09:     // compiles (!)
10:     ptr = (Spiderman*) ptr;
11: }
```

---

## Verwende den `new`-Operator nicht explizit

Siehe hierzu das entsprechende 
[C++ Core Guideline](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
mit dem Titel *Avoid calling new and delete explicitly* (*R.11*).

<img src="cpp_security_advice_no_new.svg" width="400">

*Abbildung* 1: `std::unique_ptr`-Variablen haben Charme.


<img src="cpp_security_advice_no_new_gc.svg" width="200">

*Abbildung* 2: Stack-Variablen haben Charme.

---

## Elementare Datentypen haben keine Semantik, nur Wertebereiche


Ein Datentyp wie `int` hat keine Bedeutung (*Semantik*).

Man kann nicht sagen, dass man das Alter eines Angestellten in einer `Employee`-Klasse mit
dem Typ `int` definieren sollte. `int`-Werte können sehr groß sein als auch negativ.
 `size_t_`-Werte wiederum sind nicht negativ, aber können wieder extrem große Werte annehmen.

 Es gibt die eine oder andere Möglichkeit, Werte mit einer Semantik zu verbinden:


  * Aufzählungstypen
  * `std::string` Literals
  * Benutzerdefinierte Literale
  * `size_t`


Enum-Klassen sind typsichere Klassen.

  
---



[Zurück](../../Markdown/Readme_Performance_Optimization.md)

---

36:17
