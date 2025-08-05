# &bdquo;*Zeigerw�sche*&rdquo;: `std::launder`

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link2)

---

#### Quellcode

[*Laundry.cpp*](Laundry.cpp)<br />

---

### Allgemeines <a name="link1"></a>

Die STL-Funktion ist etwas kurios, sie f�hrt eine &bdquo;*Zeigerw�sche*&rdquo; durch.

`std::launder` gibt einen Zeiger auf denselben Speicher zur�ck, auf den das Argument von `std::launder` zeigt.
Es wird jedoch angenommen,
dass das Referenzobjekt eine andere Lebensdauer und einen anderen dynamischen Typ hat als der &bdquo;neue&rdquo; Zeiger,
den `std::launder` zur�ckliefert.

Ein Aufruf von `std::launder` hat keinen Einfluss auf das Argument.
Der R�ckgabewert muss verwendet werden, um auf das Objekt in einer &bdquo;*neuen Sichtweise*&rdquo; zuzugreifen.
Es ergibt also keinen Sinn, `std::launder` aufzurufen und den R�ckgabewert zu verwerfen.

*Beispiel*:

```cpp
01: static void test_std_launder()
02: {
03:     constexpr int CountPersons{ 5 };
04: 
05:     alignas(class Person) unsigned char buffer[sizeof(class Person) * CountPersons];
06: 
07:     for (size_t i{}; i != CountPersons; ++i) {
08: 
09:         std::string first{ "first_name_" };
10:         std::string last{ "last_name_" };
11: 
12:         // manually construct objects using placement new
13:         new(buffer + sizeof(Person) * i) Person{ first + std::to_string(i), last + std::to_string(i), i};
14:     }
15: 
16:     auto ptr{ std::launder(reinterpret_cast<Person*>(buffer)) };
17: 
18:     for (size_t i{}; i != CountPersons; ++i) {
19: 
20:         std::destroy_at(&ptr[i]);
21:     }
22: }
```

*Ausf�hrung*:

```
c'tor Person
c'tor Person
c'tor Person
c'tor Person
c'tor Person
d'tor Person
d'tor Person
d'tor Person
d'tor Person
d'tor Person
```

*Anwendungsfall*:<br />

Definieren eines Zeigers
mit Hilfe eines anderen Zeigers, der auf einen Speicherbereich verweist,
in dem ein mittels *Placement new* erzeugtes Objekt residiert.
Der Zeiger, der f�r *Placement new* den Speicher bereitstellt,
ist meist von einem skalaren Typ wie `std::byte`, `unsigned char` oder �hnliches.

Einige erg�nzende Erl�uterungen dazu:

  * Bei `std::launder` handelt es sich um eine Funktion f�r den Compiler im Umfeld der Low-Level-Speicherverwaltung und der
  Betrachtung von Lebensdauererwartungen bestimmter Objekte,
  um die Nachverfolgung und Optimierungen zur Kompilierzeit zu deaktivieren, die m�glicherweise nicht korrekt sind.

  * `std::launder` wird normalerweise nur verwendet, wenn Sie die Lebensdauer eines Objekts �ber oder innerhalb eines anderen starten.

  * Wenn Sie m�chten, dass sich der Compiler naiv verh�lt, waschen Sie im Wesentlichen den Zeiger,
  damit der Compiler die komplexe Zustandsnachverfolgung zur Kompilierzeit, die Compiler durchf�hren, absichtlich vergisst und so tut,
  als w�re der Zeiger tats�chlich ein brandneues Objekt, von dem er nichts wusste.

  * Es liegen gewisse �hnlichkeiten zum Schl�sselwort `volatile` vor:
  `volatile` dient zum Deaktivieren von Compiler-Annahmen dar�ber, was ein Wert zur Laufzeit sein k�nnte.
  Wenn Sie beispielsweise zweimal hintereinander von einer regul�ren `int`-Variablen lesen, ohne dazwischen zu schreiben,
  w�rde der Compiler wahrscheinlich den zweiten Lesevorgang entfernen und den ersten Wert wiederverwenden (bessere Codegenerierung),
  da er wei�, dass der Wert nicht ge�ndert wurde.

  * Mit dem Schl�sselwort `volatile` teilen Sie dem Compiler mit, dass er nicht alle �nderungen an der Variable zuverl�ssig beobachten kann,
  sodass jeder Lesevorgang ausgef�hrt werden muss (dasselbe gilt auch f�r Schreibvorg�nge).

  * `std::launder` und `volatile` sind sich insofern �hnlich, als sie existieren, um dem Compiler mitzuteilen,
  dass er keine Annahmen �ber die Werte/Objekte auf Grund m�glicher Beobachtungen treffen soll.

---

## Literatur <a name="link2"></a>

Ein interessanter Artikel zur &bdquo;*Zeigerw�sche*&rdquo;
findet sich in [*stackoverflow*](https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder).

---

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---
