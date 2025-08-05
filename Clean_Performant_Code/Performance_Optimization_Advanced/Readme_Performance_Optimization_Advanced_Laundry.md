# &bdquo;*Zeigerwäsche*&rdquo;: `std::launder`

[Zurück](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link2)

---

#### Quellcode

[*Laundry.cpp*](Laundry.cpp)<br />

---

### Allgemeines <a name="link1"></a>

Die STL-Funktion ist etwas kurios, sie führt eine &bdquo;*Zeigerwäsche*&rdquo; durch.

`std::launder` gibt einen Zeiger auf denselben Speicher zurück, auf den das Argument von `std::launder` zeigt.
Es wird jedoch angenommen,
dass das Referenzobjekt eine andere Lebensdauer und einen anderen dynamischen Typ hat als der &bdquo;neue&rdquo; Zeiger,
den `std::launder` zurückliefert.

Ein Aufruf von `std::launder` hat keinen Einfluss auf das Argument.
Der Rückgabewert muss verwendet werden, um auf das Objekt in einer &bdquo;*neuen Sichtweise*&rdquo; zuzugreifen.
Es ergibt also keinen Sinn, `std::launder` aufzurufen und den Rückgabewert zu verwerfen.

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

*Ausführung*:

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
Der Zeiger, der für *Placement new* den Speicher bereitstellt,
ist meist von einem skalaren Typ wie `std::byte`, `unsigned char` oder Ähnliches.

Einige ergänzende Erläuterungen dazu:

  * Bei `std::launder` handelt es sich um eine Funktion für den Compiler im Umfeld der Low-Level-Speicherverwaltung und der
  Betrachtung von Lebensdauererwartungen bestimmter Objekte,
  um die Nachverfolgung und Optimierungen zur Kompilierzeit zu deaktivieren, die möglicherweise nicht korrekt sind.

  * `std::launder` wird normalerweise nur verwendet, wenn Sie die Lebensdauer eines Objekts über oder innerhalb eines anderen starten.

  * Wenn Sie möchten, dass sich der Compiler naiv verhält, waschen Sie im Wesentlichen den Zeiger,
  damit der Compiler die komplexe Zustandsnachverfolgung zur Kompilierzeit, die Compiler durchführen, absichtlich vergisst und so tut,
  als wäre der Zeiger tatsächlich ein brandneues Objekt, von dem er nichts wusste.

  * Es liegen gewisse Ähnlichkeiten zum Schlüsselwort `volatile` vor:
  `volatile` dient zum Deaktivieren von Compiler-Annahmen darüber, was ein Wert zur Laufzeit sein könnte.
  Wenn Sie beispielsweise zweimal hintereinander von einer regulären `int`-Variablen lesen, ohne dazwischen zu schreiben,
  würde der Compiler wahrscheinlich den zweiten Lesevorgang entfernen und den ersten Wert wiederverwenden (bessere Codegenerierung),
  da er weiß, dass der Wert nicht geändert wurde.

  * Mit dem Schlüsselwort `volatile` teilen Sie dem Compiler mit, dass er nicht alle Änderungen an der Variable zuverlässig beobachten kann,
  sodass jeder Lesevorgang ausgeführt werden muss (dasselbe gilt auch für Schreibvorgänge).

  * `std::launder` und `volatile` sind sich insofern ähnlich, als sie existieren, um dem Compiler mitzuteilen,
  dass er keine Annahmen über die Werte/Objekte auf Grund möglicher Beobachtungen treffen soll.

---

## Literatur <a name="link2"></a>

Ein interessanter Artikel zur &bdquo;*Zeigerwäsche*&rdquo;
findet sich in [*stackoverflow*](https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder).

---

[Zurück](Readme_Performance_Optimization_Advanced.md)

---
