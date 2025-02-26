# Guidelines zur STL (Standard Template Library)

[Zur�ck](./Readme_Guidelines.md)

---

## Inhalt

  * [Bevorzugen Sie Elemente der Standardbibliothek](#link1)
  * [Benutze STL Standardalgorithmen](#link2)

---

#### Bevorzugen Sie Elemente der Standardbibliothek <a name="link1"></a>

  * C++ verf�gt �ber eine umfangreiche und leistungsstarke Standardbibliothek,
    die gut getestete, effiziente und portable Implementierungen g�ngiger Algorithmen und Datenstrukturen bietet.

  * Verwenden Sie nach M�glichkeit Elemente der Standardbibliothek als selbst geschriebene.


Einige Beispiel:

  * Bevorzugen Sie `std::vector` oder `std::array` gegen�ber C-Stil-Arrays.
  * Bevorzugen Sie `std::string` oder `std::string_view_` gegen�ber Zeichenketten im C-Stil.
  * Bevorzugen Sie `std::unique_ptr` oder `std::shared_ptr` gegen�ber klassischen Zeigern.
  * Bevorzugen Sie Standardalgorithmen wie `std::find`, `std::sort` usw. gegen�ber handgeschriebenen Schleifen.

---

#### Bevorzugen Sie  STL Standardalgorithmen <a name="link2"></a>

Was spricht daf�r, STL-Standardalgorithmen einfachen `for`-Schleifen vorzuziehen?

  * Der Hauptgrund daf�r ist, dass diese Algorithmen im Namen ausdr�cken, was passiert, und die Schleifenlogik kapseln.

Es gibt aber noch einen zweiten Grund:

  * Vor allem die komplexeren Algorithmen k�nnen leicht un�bersichtlich werden, wenn wir sie von Hand implementieren.

  * Daher ist es gut, neben dem Namen des Algorithmus im Code den Algorithmusteil vom Rest der Logik zu trennen.

  * Dadurch wird der Code weniger kompliziert und entspricht somit dem KISS-Prinzip.


*Beispiel*:

Stellen Sie sich vor, wir bekommen einen Container, handgeschrieben oder aus einer Drittanbieterbibliothek.

Er hat standardkompatible Iteratoren und enth�lt eine Reihe von Mitarbeiterdaten.

Um diese Daten in unserer Gesch�ftslogik wiederzuverwenden, ohne den benutzerdefinierten Container verwenden zu m�ssen,
werden die Daten in ein `std::vector`-Objekt �bertragen:

```cpp
01: std::vector<Employee> source;
02: //...
03: 
04: std::vector<Employee> employees;
05: employees.reserve(source.size());
06: 
07: // using for loop
08: for (auto const& employee : source) {
09:     employees.push_back(employee);
10: }
```

Das Ersetzen der Schleife durch einen STL-Algorithmus ist nicht sonders schwer &ndash; wir verwenden `std::copy`:

```cpp
01: std::vector<Employee> source;
02: //...
03: 
04: std::vector<Employee> employees;
05: employees.reserve(source.size());
06: 
07: // using STL algorithm 'std::copy'
08: std::copy(
09:     source.begin(),
10:     source.end(),
11:     std::back_inserter(employees)
12: );
```

Es ginge mit einem speziellen Konstruktor der `std::vector`-Klasse noch einfacher:


```cpp
01: std::vector<Employee> source;
02: //...
03: 
04: // using appropriate container constructor
05: std::vector<Employee> employees (source.begin(), source.end());
```

Sp�ter in unserer Codebasis wollen wir die Geh�lter der Mitarbeiter analysieren.

Die Klasse `Employee` hat eine `uniqueName`-Methode,
sodass wir alle Namen und Geh�lter der Mitarbeiter in ein `std::map`-Objekt einf�gen k�nnen:

```cpp
01: std::vector<Employee> employees;
02: 
03: std::map<std::string, unsigned int> salariesByName;
04: 
05: // for loop
06: for (auto const& employee : employees) {
07:     salariesByName[employee.uniqueName()] = employee.salary();
08: }
09: 
10: // or
11: for (auto const& employee : employees) {
12:     salariesByName.insert(
13:         std::make_pair(employee.uniqueName(), employee.salary())
14:     );
15: }
```


Der Algorithmus zum Entnehmen von Elementen aus einem Container und zum Erstellen daraus verschiedener Elemente
f�r einen anderen Container lautet `std::transform`:


```cpp
01: std::vector<Employee> employees;
02: 
03: std::map<std::string, unsigned int> salariesByName;
04: 
05: // using STL algorithm
06: std::transform(
07:     employees.begin(),
08:     employees.end(),
09:     std::inserter(salariesByName, salariesByName.end()),
10:     [](auto const& employee) {
11:         return std::make_pair(
12:             employee.uniqueName(),
13:             employee.salary()
14:         );
15:     }
16: );
```

Der `std::inserter` �hnelt dem `back_inserter`, ben�tigt aber einen Iterator, mit dem er `insert` aufruft.

Im Fall von `std::map` ist dies ein *Hinweis* darauf, wo das Element eingef�gt werden k�nnte.

Das Lambda-Objekt f�hrt die eigentliche Umwandlung eines `Employees`-Objekts in einen `std::map`-Eintrag durch-

Es ist sehr interessant, die Geh�lter aller Mitarbeiter aufgelistet zu haben,
aber vielleicht m�chten Ihre Manager nicht, dass Sie wissen, wie hoch ihr Gehaltsscheck aussieht.

Daher erhalten wir die zus�tzliche Anforderung,
die Geh�lter der Manager aus dieser Karte herauszuhalten.
Im Fall unserer urspr�nglichen Schleife ist die �nderung einfach:

```cpp
01: std::vector<Employee> employees;
02: //...
03: 
04: std::map<std::string, unsigned> salariesByName;
05: 
06: for (auto const& employee : employees) {
07:     if (!employee.isManager()) {
08:         salariesByName[employee.uniqueName()] = employee.salary();
09:     }
10: }
```

Die Schleife wird etwas komplizierter, ist aber immer noch lesbar.

Wir sind vielleicht nicht davon �berzeugt, dass die Verwendung eines Algorithmus hier notwendig ist, um es lesbarer zu machen.

Aber sehen wir uns an, wie es aussieht, wenn wir es tun. Normalerweise haben Algorithmen mit einer Bedingung oder, im Standardjargon, einem Pr�dikat, das Suffix _if in ihrem Namen.

Es gibt `std::copy_if`, um nur Dinge zu kopieren, die eine Bedingung erf�llen, und `std::find_if` und `std::remove_if` arbeiten mit Elementen, die einem Pr�dikat statt einem Wert entsprechen.

Der Algorithmus, nach dem wir suchen, ist also `transform_if`.

Aber das gibt es nicht in der Standardbibliothek.

Schade. Zum Gl�ck ist es nicht schwer zu implementieren, wenn wir einen Blick auf die Implementierungen von `std::transform` und `std::copy_if` werfen.

Also starten wir jetzt unsere eigene Algorithmenbibliothek. Der gesamte Code sieht jetzt so aus:


```cpp
01: template <typename InIter, typename OutIter, typename UnaryOp, typename Pred>
02: OutIter transform_if(InIter first, InIter last,
03:     OutIter result, UnaryOp unaryOp, Pred pred) {
04:     for (; first != last; ++first) {
05:         if (pred(*first)) {
06:             *result = unaryOp(*first);
07:             ++result;
08:         }
09:     }
10:     return result;
11: }
12: 
13: static void guidelines_stl_algorithms_transform_04() {
14: 
15:     std::vector<Employee> employees;
16: 
17:     std::map<std::string, unsigned int> salariesByName;
18: 
19:     // using STL algorithm
20:     transform_if(
21:         employees.begin(),
22:         employees.end(),
23:         std::inserter(salariesByName, salariesByName.end()),
24:         [](auto const& employee) {
25:             return std::make_pair(
26:                 employee.uniqueName(),
27:                 employee.salary()
28:             );
29:         },
30:         [](auto const& employee) {
31:             return !employee.isManager();
32:         }
33:     );
34: }
```

---

## Literatur <a name="link19"></a>

Das Beispiel zu KISS und der Verwendung von STL Algorithmen wurde in
[Algorithms and the KISS Principle](https://arne-mertz.de/2019/05/algorithms-and-the-kiss-principle/)
von Arne Mertz gefunden.

---

[Zur�ck](./Readme_Guidelines.md)

---
