# *Placement New*: Trennung von Speicherallokation und Objektkonstruktion

[Zur�ck](Readme_MemoryManagement.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Erstes Beispiel](#link2)
  * [Zweites Beispiel](#link3)

---

#### Quellcode

[*MemoryManagement_Placement_New.cpp*](MemoryManagement_Placement_New.cpp)<br />

---

### Allgemeines <a name="link1"></a>

C++ erm�glicht es uns, die Bereitstellung von Speicher (Speicherallokation)
von der Objekterstellung zu trennen.

---

### Erstes Beispiel <a name="link2"></a>

Wir k�nnen beispielsweise mit `malloc()` ein Byte-Array reservieren
und in diesem Speicherbereich ein neues, benutzerdefiniertes Objekt erstellen.

Die m�glicherweise ungewohnte Syntax, die `::new (memory)` verwendet, hei�t *Placement new*.

Es handelt sich um eine nicht allokierende Form von `new`, die nur ein Objekt konstruiert.

Der doppelte Doppelpunkt (`::`) vor `new` stellt sicher,
dass die Aufl�sung aus dem globalen Namensraum erfolgt, um zu vermeiden,
dass eine �berladene Version des Operators `new` verwendet wird.

*Beispiel*:

```cpp
01: void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = ::new (memory) Person{ "Sepp", "Mueller", static_cast<size_t>(30) };
05: 
06:     person->~Person();
07:     std::free(memory);
08: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
```

Es gibt kein *Placement delete*.
Um das Objekt zu zerst�ren und den Speicher freizugeben, m�ssen wir also zum einen den Destruktor explizit aufrufen
und dann zum zweiten den Speicher separat freigeben.

C++17 f�hrt eine Reihe von Hilfsfunktionen in der STL (Include-Datei `<memory>`) ein,
um Objekte zu konstruieren und zu zerst�ren, ohne Speicher zuzuweisen oder freizugeben.

Anstatt *Placement new* zu verwenden, ist es jetzt also m�glich,
einige der Funktionen aus der STL zu verwenden,
deren Namen mit `std::uninitialized_` beginnen, um Objekte in einem nicht initialisierten Speicherbereich
zu konstruieren, zu kopieren und zu verschieben.

Und anstatt den Destruktor explizit aufzurufen, k�nnen wir jetzt `std::destroy_at()` verwenden,
um ein Objekt an einer bestimmten Speicheradresse zu zerst�ren, ohne den Speicher freizugeben.

Das vorherige Beispiel k�nnte mit diesen neuen Funktionen so geschrieben werden:

*Beispiel*:

```cpp
01: void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = reinterpret_cast<Person*>(memory);
05: 
06:     std::uninitialized_fill_n(person, 1, Person{ "Sepp", "Mueller", (size_t) 30 });
07:         
08:     std::destroy_at(person);
09:     std::free(memory);
10: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
d'tor Person
```


Der Aufruf der `std::uninitialized_fill_n`-Funktion kann ab C++ 20 auch
durch einen  Aufruf der `std::construct_at`-Funktion ersetzt werden.
Diese Funktion ist bzgl. ihres Aufrufs etwas einfacher in der Handhabung:


*Beispiel*:

```cpp
01: void test() {
02: 
03:     auto* memory = std::malloc(sizeof(Person));
04:     auto* person = reinterpret_cast<Person*>(memory);
05: 
06:     std::construct_at(person, Person{ "Sepp", "Mueller", 30 }); // C++20
07: 
08:     std::destroy_at(person);
09:     std::free(memory);
10: }
```

*Ausf�hrung*:

```
c'tor Person
d'tor Person
d'tor Person
```

---

### Zweites Beispiel <a name="link3"></a>

Wir betrachten in diesem Beispiel eine Klasse `BigData`:
Das Klassentemplate soll f�r Daten unterschiedlichen Typs `T`
eine Art Container sein.

#### Erster Ansatz

Eine erste einfache Realisierung, die nur die Deklaration der Instanzvariablen
und einen benutzerdefinierten Konstruktur zeigt, k�nnte so aussehen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T*          m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s) / d'tor
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:         : m_elems{ new T[size] }, m_size{ size }
14:     {
15:         std::fill(m_elems, m_elems + m_size, init);
16:     }
17: 
18:     ~BigData()
19:     {
20:         delete[] m_elems;
21:     }
22: 
23:     // ...
24: };
```

Was ist schlecht in puncto &bdquo;Performanz&rdquo; an dieser Implementierung?

Okay, es h�ngt ein wenig von der konkreten Wahl des Datentyps `T` ab.
Stellen wir uns vor, `T` repr�sentiert *keinen* elementaren Datentyp,
also zum Beispiel die Klasse `std::string` oder einen anderen benutzerdefinierten Typ.

Der Konstruktor reserviert zun�chst auf der Halde dynamisch Speicher f�r `size` Elemente des Typs `T` (mittels `new T[size]`).
Das bedeutet insbesondere, dass `size` Mal der Standardkonstruktor des Typs `T` ausgef�hrt wird.

Nun gibt es aber einen Vorbelegungswert `init`, mit dem alle Objekte im Speicherbereich `m_elems` vorbelegt werden sollen.
Es kommt also noch zus�tzlich `size` Mal der Kopierkonstruktor der Klasse `T` mit `init` als Vorlage zur Ausf�hrung.

Im Prinzip belegen wir den Speicherbereich `m_elems` zweimal vor:
Zum Ersten mit dem Standardkonstruktor des Typs `T` und zum Zweiten mit seinem Kopierkonstruktor.
Wie k�nnte man dies vermeiden?

#### Zweiter Ansatz

Mit Hilfe von *Placement new* k�nnen wir eine Vereinfachung erreichen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T* m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s)
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:     {
14:         m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
15:         m_size = size;
16: 
17:         for (auto pBegin = m_elems; pBegin != m_elems + m_size; ++pBegin) {
18:             ::new (static_cast<void*>(pBegin)) T{ init };
19:         }
20:     }
21: 
22:     ~BigData()
23:     {
24:         std::for_each(
25:             m_elems,
26:             m_elems + m_size,
27:             [](const T& obj) { obj.~T(); }
28:         );
29: 
30:         std::free(m_elems);
31:     }
32: 
33:     // ...
34: };
```

In Zeile 14 reservieren wir den Speicher mit `std::malloc`. Dies erspart uns die unn�tze Ausf�hrung des Standardkonstruktors
f�r alle `T`-Objekte.

Um die `T`-Objekte wie gefordert vorzubelegen (mit dem Wert `init`),
wenden wir den *Placement new*&ndash;Mechanismus an (Zeile 18).

Wir d�rfen in dieser Variante nicht �bersehen, dass wir den Destruktor der Klasse `T` explizit aufrufen m�ssen.
Dies erfolgt in Zeile 27.

#### Dritter Ansatz

F�r die *Placement new*&ndash;Techniken aus dem letzten Beispiel gibt es Low-Level-Funktionen, 
die deren Schreibweise vereinfachen:

```cpp
01: template <typename T>
02: class BigData
03: {
04: private:
05:     T*          m_elems{};
06:     std::size_t m_size{};
07: 
08: public:
09:     // c'tor(s)
10:     BigData() = default;
11: 
12:     BigData(std::size_t size, const T& init)
13:     {
14:         m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
15:         m_size = size;
16:         std::uninitialized_fill(m_elems, m_elems + m_size, init);
17:     }
18: 
19:     ~BigData()
20:     {
21:         std::destroy(m_elems, m_elems + m_size);
22:         std::free(m_elems);
23:     }
24: 
25:     // ...
26: };
```

*Hinweis*:<br />
Zwischen `std::fill` und `std::uninitialized_fill` gibt es folgenden Unterschied in der Ausf�hrung:


  * `std::fill`:<br />
  Es wird ein g�ltig vorbelegter Speicherbereich erwartet. Das letzte Argument von `std::fill` wird mit dem `=`-Operator
  an alle vorhandenen Elemente im Speicherbereich zugewiesen. Folglich erwartet der `operator=` einen Speicherbereich,
  der g�ltige Werte enth�lt.

  * `std::uninitialized_fill`:<br />
  Der zu belegende Speicherbereich kann beliebige Werte enthalten.
  Es wird der gesamte Speicherbereich im Stile des *Placement new*&ndash;Mechanismus mit dem letzten Argument von `std::uninitialized_fill` vorbelegt.

Die Ausf�hrungszeiten sehen auf meinem Rechner wie folgt im *Release*-Modus aus:
Jeweils drei aufeinanderfolgende Zeilen betrachten ein `BigData`-Objekt
mit `int`, `std::string` oder `Person`-Objekten:

```
Elapsed time: 2 milliseconds.
Elapsed time: 2666 milliseconds.
Elapsed time: 4958 milliseconds.

Elapsed time: 2 milliseconds.
Elapsed time: 2035 milliseconds.
Elapsed time: 4503 milliseconds.

Elapsed time: 2 milliseconds.
Elapsed time: 2026 milliseconds.
Elapsed time: 4484 milliseconds.
```

---

[Zur�ck](Readme_MemoryManagement.md)

---
