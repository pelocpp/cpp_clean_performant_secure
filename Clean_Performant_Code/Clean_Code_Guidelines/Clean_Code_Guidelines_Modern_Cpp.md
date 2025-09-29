# Richtlinien Clean Code &bdquo;Modernes C++&rdquo;

[Zur�ck](./Readme_Guidelines.md)

---

## Inhalt

---

### Namensgebung f�r Bezeichner <a name="link1"></a>

Auch wenn es �berraschend klingen mag: Die Namensgebung f�r Bezeichner ist keine einfache Angelegenheit.

Ein sprechender Variablenname &ndash; genauer auch selbsterkl�render Variablenname &ndash; ist ein Bezeichner f�r eine Variable,
aus dem die Bedeutung der Variablen sichtbar wird.

Dies gilt nicht nur f�r Variablen, sondern auch f�r Klassen, Methoden, Funktionen und �hnliches.

Auch wenn Quellcode (oder Quelltext) f�r die Maschine lesbar sein muss und es dieser vollkommen egal ist,
wie die Variablen hei�en, so wird Code dennoch von Menschen entworfen und gepflegt.

Gerade wenn man als Team an einem Projekt arbeitet, ist ein gut kommentierter, also menschenlesbarer Code wichtig.

Bei Vermeidung sprechender Variablennamen werden Quelltexte unleserlich und damit fehleranf�lliger sowie schwerer wartbar.

*Beispiel*:

```cpp
double vatRate;       // Mehrwertsteuersatz
double unitPrice;     // Einzelpreis
double reduceVatRate; // reduzierter Mehrwertsteuersatz
```

versus

```cpp
double u, v, w;
```

*Bemerkung*:<br />
*There are only two hard things in Computer Science: cache invalidation and naming things.* (Phil Karlton)

---

### Kommentare <a name="link1"></a>

*Uncle Bob*<br />
&bdquo;*Nothing can be quite so helpful as a well-placed comment.*&rdquo;

#### Pro und Contra

  * Pro: Code sollte klar genug geschrieben sein, um ohne Kommentare verstanden zu werden.
  * Contra: Das Kommentieren von Code ist eine absolut notwendige Anforderung und Sie sollten Ihren Code so oft wie m�glich kommentieren.


#### Versuch einer neutralen Einsch�tzung

  * Quellcode sollte so geschrieben sein, dass er klar genug ist, um ohne Kommentare verstehen zu k�nnen, was er tut.

  * Kommentare, die erkl�ren, was der Code tut, sind im besten Fall eine Kr�cke und im schlimmsten Fall veraltet, wenn der Code aktualisiert wurde, ohne die Kommentare zu aktualisieren.

  * Es sollten Kommentare hinzugef�gt werden, um in nicht zwingend einleuchtenden Situationen zu erkl�ren, warum der Code tut, was er tut.

  * Machen Sie Ihren Code selbstdokumentierend, indem Sie gute Namen f�r Variablen und Methoden w�hlen.

  * F�gen Sie Kommentare hinzu, um zu erkl�ren, warum der Code auf eine bestimmte Weise funktioniert. Geben Sie sich besonders viel M�he, dies zu tun, wenn die Funktionsweise des Codes seltsam erscheint, die offensichtlichen L�sungen jedoch bereits ausprobiert wurden und besondere Probleme aufwerfen.


---


### Klasse (`class`) oder Struktur (`struct`) zum Ersten <a name="link1"></a>

  * Verwenden Sie Strukturen (`struct`), wenn der zu konzipierende Datentyp
   haupts�chlich ein Halter von Daten ist.
  * Verwenden Sie Klassen (`class`), wenn f�r den zu konzipierenden Datentyp
   die Daten eher als Implementierungsdetails anzusehen sind
   und es das Verhalten (Methoden) ist, was man nach au�en zeigen m�chte.

*Beispiel*:

```cpp
01: struct Point2D
02: {
03:     int m_x;
04:     int m_y;
05: 
06:     Point2D() : Point2D{ 0, 0 } {}
07:     Point2D(int x, int y) : m_x{ x }, m_y{ y } {}
08: };
```

---

### Klasse (`class`) oder Struktur (`struct`) zum Zweiten <a name="link2"></a>

Eine Richtlinie stammt von Bjarne Stroustrup *himself*:

  * Verwenden Sie Klasse (`class`), wenn es *Invarianten* gibt.
  * Verwenden Sie Strukturen (`struct`), wenn alle Datenelemente sich unabh�ngig voneinander ver�ndern k�nnen.

Es bleibt nur noch zu kl�ren, was man unter einer *Invarianten* versteht?
Dazu ein Beispiel einer Klasse `Game`,
das eine Instanzvariable `m_position` hat, die sich bestimmten Grenzwerten
(hier: `m_width` und `m_height`) unterwerfen muss.

*Beispiel*:

```cpp
01: class Game
02: {
03: private:
04:     int     m_width;       // right bound 
05:     int     m_height;      // height bound 
06:     Point2D m_position;    // position of a figure on the game board
07: 
08: public:
09:     Game(int width, int height, Point2D position)
10:         : m_width{}, m_height{}, m_position{ m_position }
11:     {
12:         // ensure that the object was constructed correctly 
13:         checkInvariant();
14:     }
15: 
16:     void moveTo(Point2D inc)
17:     {
18:         m_position.m_x += inc.m_x;
19:         m_position.m_y += inc.m_y;
20: 
21:         // ensure that the figure wasn't moved out of play bounds 
22:         checkInvariant();
23:     }
24: 
25:     // calling 'checkInvariant' is unnecessary because this is an accessor method 
26:     Point2D getLoc() const
27:     {
28:         return m_position;
29:     }
30: 
31: private:
32:     void checkInvariant() const
33:     {
34:         if (!(m_position.m_x >= 0 && m_position.m_x <= m_width)) {
35:             throw std::out_of_range("Position exceeds width of game board!");
36:         }
37:                  
38:         if (!(m_position.m_y >= 0 && m_position.m_y <= m_height)) {
39:             throw std::out_of_range("Position exceeds height of game board!");
40:         }
41:     }
42: };
```

---

### Klasse (`class`) oder Struktur (`struct`) zum Dritten <a name="link3"></a>

Wenn ein (oder mehrere Elemente) mit `private` zu kennzeichnen sind,
sollte man `class` verwenden.

---

### Organisation von Include-Files <a name="link4"></a>

Die goldene Regel f�r #include-Dateien lautet:

Wenn ein Modul `main.c` ein Untermodul `submodule.c` verwendet,
sollte alles, was zum Kompilieren von `submodule.c` ben�tigt wird,
in `submodule.h` definiert sein.

Die empfohlene Regel lautet:

  1. Lokale #include-Dateien
  2. Drittanbieter #include-Dateien
  3. STL #include-Dateien

---

### Interfaces (Schnittstellen): Wozu? <a name="link5"></a>

*Interfaces* stellen nicht nur eine Beschreibung von Methoden dar, sie bewirken damit
auch eine *Hervorhebung*:

  * Zur Implementierung in einer Klasse wird ein Bezug zur Schnittstelle gegeben.
  * In einer Klasse lassen sich Daten kapseln, die relevanten Methoden der Schnittstelle sind hervorgehoben.

*Beispiel*:

```cpp
01: struct ICloneable
02: {
03:     virtual ~ICloneable() {};
04:     virtual std::shared_ptr<ICloneable> clone() const = 0;
05: };
```

---

### Minimieren Sie die Sichtbarkeit von Mitgliedern einer Klasse <a name="link6"></a>

Ohne Worte :)

---

### Lebenszyklus eines Objekts: Spezielle Member-Funktionen <a name="link7"></a>

C++ definiert f�r den Lebenszyklus eines Objekts eine Reihe von Methoden/Operatoren,
die eine besondere Rolle einnehmen.

Die Schnittstellen dieser Operationen sehen so aus:

| Beschreibung | Schnittstelle |
| :-------- | :------- |
| Standardkonstruktor (Default Constructor) | `X()` |
| Kopierkonstruktor (Copy Constructor) | `X(const X&)` |
| Kopierende Zuweisung (Assignment Operator) | `X& operator=(const X&)` |
| Verschiebekonstruktor (Move Constructor) | `X(X&&) noexcept` |
| Verschiebende Zuweisung (Move Assignment) | `X& operator=(X&&) noexcept` |
| Destruktor (Destructor) | `~X()` |

*Tabelle* 1: Spezielle Member-Funktionen eines C++ Objekts.

---

### Automatische Erzeugung der speziellen Member-Funktionen: Wann? <a name="link8"></a>

In manchen Situationen nimmt der Compiler einem Entwickler die Arbeit ab und 
erzeugt f�r eine oder mehrere der speziellen Member-Funktionen eine Realisierung.

Das gesamte Regelwerk in diesem Umfeld ist nicht ganz trivial,
einen �berblick versucht das folgende Bild zu geben ([hier](https://howardhinnant.github.io/classdecl.html) vorgefunden):

<img src="cpp_special_member_functions.svg" width="700">

*Abbildung* 1: �berblick �ber die automatische Erzeugung spezieller Member-Funktionen.

Aus *Abbildung* 1 kann man einige wesentliche Aussagen ableiten:

  * Standardkonstruktor:<br/>Der Standardkonstruktor wird automatisch generiert, wenn Sie in der betroffenen Klasse selbst *�berhaupt keinen* Konstruktor implementiert haben.
  * Kopierkonstruktor:<br/>Der Kopierkonstruktor wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Kopierende Zuweisung:<br/>Die Kopierzuweisung wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Verschiebekonstruktor:<br/>Der Verschiebekonstruktor wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden k�nnen.
  * Verschiebende Zuweisung:<br/>Die verschiebende Zuweisung wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden k�nnen.
  * Destruktor:<br/>Der Destruktor wird automatisch generiert, wenn Sie keinen Destruktor implementiert haben.

Was erkennen wir an diesen Aussagen?

  * Die beiden *Kopieroperationen* sind unabh�ngig voneinander:
  Wenn in einer Klasse eine der beiden Kopieroperationen definiert ist,
  k�nnte der �bersetzer immer noch die andere Operation automatisch erzeugen.

  * F�r die beiden *Verschiebeoperationen* gilt das Gegenteil:
  Wird vom Entwickler eine der beiden Operationen implementiert,
  stellt der Compiler *keine* Realisierung der anderen Operation bereit.

---

### Automatische Erzeugung der speziellen Member-Funktionen: Wie? <a name="link9"></a>

Nat�rlich w�re es interessant zu wissen, wie der Quellcode von automatisch erzeugten Member-Funktionen aussieht.
Das ist keine triviale Frage, dennoch kann man ein paar Richtlinien erkennen:

  * Standardkonstruktor:<br/>
    * F�r alle Instanzvariablen, die Objekte sind, wird deren Standardkonstruktor aufgerufen.
    * F�r alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt **keine** Initialisierung, also auch keine Vorbelegung mit einem datentypspezifischen Null-Wert!

  * Kopierende Operationen:<br/>
    * Das Objekt wird elementweise kopiert.<br/>
    * Darunter ist zu verstehen, dass f�r jedes Element, das selbst wieder ein Objekt
    oder eine Variable elementaren Datentyps ist, der zugeh�rige Zuweisungsoperator aufgerufen wird.
    F�r Variablen elementaren Datentyps (und dazu z�hlen auch Zeigervariablen) bewirkt dies eine bitweise Kopie.
    * Falls Konstanten oder Referenzen vorhanden sind, geschieht dies jedoch nicht, weil sie
    nur initialisiert, aber nicht durch Zuweisung ver�ndert werden k�nnen.
    Der Zuweisungsoperator wird vom System in diesem Fall *nicht* erzeugt.
    * *Hinweis*:<br/>Im Falle  von (klassischen) Zeigervariablen entstehen in diesem Fall Probleme (Stichwort: flache versus tiefe Kopie).

  * Verschiebende Operationen:<br/>
    * F�r alle Instanzvariablen, die Objekte sind, wird deren entsprechende Verschiebeoperation aufgerufen, falls vorhanden. Anderenfalls erfolgt eine Kopie.
    * F�r alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt eine bitweise Kopie.


---

### *Explicitly defaulted* Standardkonstruktor <a name="link10"></a>

Um das manuelle Schreiben leerer Standardkonstruktoren zu vermeiden,
unterst�tzt C++ das Konzept von *explicitly defaulted* Standardkonstruktoren (*Explicitly defaulted Default Constructors*).

Dadurch k�nnen Sie die Klassendefinition wie folgt schreiben,
ohne eine leere Implementierung f�r den Standardkonstruktor bereitstellen zu m�ssen:

```cpp
class X
{
public:
    X() = default;
    X(double initialValue);
    X(std::string initialValue);
    // ...
};
```

Klasse `X` definiert zwei benutzerdefinierte Konstruktoren.

Der Compiler generiert jedoch weiterhin einen vom Standardcompiler generierten Standardkonstruktor,
da dieses mithilfe des Schl�sselworts `default` explizit festgelegt wird.

Sie k�nnen das `= default` direkt in die Klassendefinition einf�gen,
damit kann man sich in der Klassenimplementierung eine leere Implementierung f�r den Standardkonstruktor sparen.

Zur Vermeidung von Missverst�ndnissen:<br />
Die Implementierung des automatisch erzeugten Standardkonstruktors ist so,
als h�tte man `= default` in der Klassendefinition geschrieben.
Diese Aussage gilt f�r alle speziellen Member-Funktionen.

---

### *Explicitly deleted* Standardkonstruktor <a name="link11"></a>

Auch das Gegenteil von *Explicitly defaulted Default Constructors* ist m�glich
und wird als *Explicitly deleted Default Constructor* bezeichnet.

Sie k�nnen beispielsweise eine Klasse mit nur statischen Memberfunktionen definieren,
f�r die Sie keine Konstruktoren schreiben m�chten und f�r die Sie auch nicht m�chten,
dass der Compiler den Standardkonstruktor automatisch generiert.

In diesem Fall m�ssen Sie den Standardkonstruktor explizit l�schen:

```cpp
class X
{
public:
    X() = delete;
};
```

---

### Automatische Erzeugung spezieller Member-Funktionen: *Rule-of-Zero* <a name="link12"></a>

Was sieht die einfachste Strategie aus:

**Rule-of-Zero**

  * Alles sechs speziellen Member-Funktionen d�rfen weder implementiert noch mit `= delete` gel�scht werden.
  * Der Compiler generiert alle Member-Funktionen so, wie es ben�tigt wird.
  * Das funktioniert nat�rlich nur dann, wenn die automatisch erzeugten Operationen das tun, was sie m�ssen:
    Korrekt initialisieren, kopieren und verschieben &ndash; und keine Ressourcen �brig lassen.
  * Wann funktioniert das? Wenn man sich auf folgenden Member-Variablen einschr�nkt:
    * Standard Datentypen
    * Aggregate (Strukturen)
    * STL Standard Container
    * Smart Pointer
 * Was geht nicht:
    * Wenn in den Instanzvariablen klassische Zeiger, die auf mit `new` allokierte Daten zeigen, vorhanden sind.
    * Wenn Referenzen in den Instanzvariablen vorhanden sind.

Wenn keine der sechs speziellen Member-Funktionen selbst geschrieben werden (*Rule-of-Zero*),
weil Sie geschickt auf `new` usw. verzichten, werden diese s�mtlich vom Compiler erzeugt,
einschlie�lich des verschiebenden Konstruktors und des verschiebenden Zuweisungsoperators.

Damit wird Ihr Programmcode performant, einfacher und wartbarer &ndash; und es spart Schreibarbeit.

---

### Initialisierung von Strukturen <a name="link13"></a>

Wir starten eine Reihe von �berlegungen zu folgender Struktur `Point2D`:

```cpp
01: struct Point2D
02: {
03:     double m_x;
04:     double m_y;
05: };
06: 
07: void test() {
08: 
09:     Point2D point;
10:     std::println("x: {} - y: {}", point.m_x, point.m_y);
11: }
```

#### Variante 1

Im ersten Listing starten wir mit der schlechtesten aller Varianten:
Die Struktur `Point2D` besitzt keine Vorkehrungen, um ihre Membervariablen vorzubelegen &ndash;
und in Zeile 9 ist es sogar m�glich, eine Strukturvariable `point` anzulegen, deren Werte
nicht vorbelegt sind.

#### Variante 2

```cpp
01: struct Point2D
02: {
03:     double m_x{};
04:     double m_y{};
05: };
06: 
07: void test() {
08: 
09:     Point2D point;
10: 
11:     point.m_x = 1.0;
12:     point.m_y = 2.0;
13: 
14:     Point2D copy{ point };   // automatically generated copy-c'tor
15: 
16:     Point2D anotherPoint;
17:     anotherPoint = copy;     // automatically generated assignment operator
18: }
```

In dieser Variante besitzen alle Membervariablen so genannte &bdquo;Default Initializer&rdquo;.
Eine Variable ` Point2D point;` repr�sentiert damit den Punkt (0,0) und alle sechs speziellen
Membermethoden funktionieren wie erwartet.

*1. Bemerkung*:<br />
Ein Destruktor tut hier nichts, da es nichts zu tun gibt.

*2. Bemerkung*:<br />
Die zwei verschiebenden Methoden (Verschiebe-Konstruktor, verschiebende Wertzuweisung)
verhalten sich wie ihre kopierenden Pendants, da es nicht wirklich etwas zu verschieben gibt
(alle Membervariablen sind elementaren Typs).

#### Variante 3

Was f�llt Ihnen an dieser dritten Variante auf?

```cpp
01: struct Point2D
02: {
03:     double m_x{};
04:     double m_y{};
05: 
06:     Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
07: };
08: 
09: void test() {
10: 
11:     // Point2D point;  // error: does not compile
12:     Point2D anotherPoint{ 1.0, 2.0 };
13: }
```

Die Struktur weist nun einen benutzerdefinierten Konstruktor auf.
Damit ist Zeile 11 des letzten Listings nicht mehr �bersetzungsf�hig,
es gibt also keinen Default-Konstruktor mehr.

Das sollte so nicht sein, eine Abhilfe finden Sie in der n�chsten Variante 4 vor:

#### Variante 4

```cpp
01: struct Point2D
02: {
03:     double m_x;   // no more need for default initialization
04:     double m_y;   // no more need for default initialization
05: 
06:     Point2D() : m_x{}, m_y{} {}
07:     Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
08: };
09: 
10: void test() {
11: 
12:     Point2D point; 
13:     Point2D anotherPoint{ 1.0, 2.0 };
14: }
```

Die aktuelle Version der Struktur `Point2D` definiert nun alle erw�nschten Konstruktoren explizit;
damit besteht keine Notwendigkeit mehr, die Membervariablen mit Hilfe von
&bdquo;Default Initializern&rdquo; vorzubelegen.
 

#### Variante 5

Die zwei benutzerdefinierten Konstruktoren aus dem letzten Beispiel kann man auch eleganter
mit dem sprachlichen Mittel des &bdquo;*Constructor Chainings*&rdquo; zusammenfassen:


```cpp
01: struct Point2D
02: {
03:     double m_x;
04:     double m_y;
05: 
06:     Point2D() : Point2D{ 0.0, 0.0 }  {}   // delegate work to another constructor
07:     Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
08: };
09: 
10: void test() {
11: 
12:     Point2D point;
13:     Point2D anotherPoint{ 1.0, 2.0 };
14: }
```

#### Variante 6

Wir kommen noch einmal auf eine Version der Struktur `Point2D` zu sprechen,
die den Standardkonstruktor verloren hatte. Mit dem Schl�sselwort `default` kann man diesen wieder hinzuf�gen.
Dazu m�ssen alle Membervariablen aber mit &bdquo;Default Initializern&rdquo; vorbelegt werden,
sonst funktioniert diese Variante nicht:


```cpp
01: struct Point2D
02: {
03:     double m_x{};
04:     double m_y{};
05: 
06:     Point2D() = default;
07:     Point2D(double x, double y) : m_x{ x }, m_y{ y } {}
08: };
09: 
10: void test() {
11: 
12:     Point2D point;
13:     Point2D anotherPoint{ 1.0, 2.0 };
14: }
```

H�ufig kann man die Beobachtung machen, dass diese Art des Entwurfs zu gutem Maschinencode f�hrt.

#### Variante 7

Zum Abschluss stellen wir einen letzen, aber alternativen Ansatz vor:
Die Struktur `Point2D` besitzt weder Konstruktoren noch &bdquo;Default Initializer&rdquo; f�r ihre Membervariablen.
Daf�r kommt bei Verwendung der Struktur die Aggregat-Initialisierung zum Einsatz:


```cpp
01: struct Point2D
02: {
03:     double m_x;
04:     double m_y;
05: };
06: 
07: void test() {
08: 
09:     // taking advantage of struct Point2D beeing an 'aggregate' type
10:     Point2D point{};
11:     Point2D anotherPoint{ 1.0, 2.0 };
12: }
```

Dazu muss der beteiligte Strukturtyp allerdings die Vorrausetzungen eines
&bdquo;Aggregat-Typs&rdquo; aufweisen.

Kompakt formuliert lauten diese:

  * Keine benutzerdeklarierten Konstruktoren
  * Keine geerbten Konstruktoren
  * Keine privaten, nicht statischen Datenelemente
  * Keine virtuellen Basisklassen
  * ... einige weitere mehr detailliertere Eigenschaften


---

### Initialisierung von Objekten <a name="link14"></a>

Wie bei Strukturen betrachten wir nun eine Reihe von Entwicklungsschritten
in der Entwicklung einer Klasse `SimpleString`.

#### Variante 1

Diese Variante stellt den Rohentwurf einer Klasse `SimpleString` dar &ndash;
und ist mit mehreren Fehlern behaftet. Erkennen Sie diese?

```cpp
01: class SimpleString
02: {
03: private:
04:     char*       m_data{};   // pointer to the characters of the string (nullptr)
05:     std::size_t m_elems{};  // number of elements (zero)
06:             
07: public:
08:     // c'tors / d'tor
09:     SimpleString() = default; // empty string
10: 
11:     SimpleString(const char* s)
12:         : m_elems{ std::strlen(s) }
13:     {
14:         m_data = new char[size() + 1];      // need space for terminating '\0'
15:         std::copy(s, s + size(), m_data);
16:         m_data[size()] = '\0';
17:     }
18: 
19:     ~SimpleString() {
20:         delete[] m_data;
21:     }
22: 
23:     // getter
24:     std::size_t size() const { return m_elems; }
25:     bool empty() const { return size() == 0; }
26:     const char* data() const { return m_data; }
27: 
28:     // operators (no index-checking intentionally)
29:     char operator[](std::size_t n) const { return m_data[n]; }
30:     char& operator[](std::size_t n) { return m_data[n]; }
31: };
```

#### Variante 2

F�r Klasse `SimpleString` werden die speziellen Member-Funktionen wie bei Strukturen automatisch
erzeugt &ndash; nur funktionieren diese nicht! Die `SimpleString`-Klasse enth�lt Zeiger,
die durch `new` erzeugt worden sind. Prinzipiell sind derartige Zeiger kopierbar &ndash;
nur d�rfen derartige Zeiger nicht zweimal mit `delete` freigegeben werden.

Ungeachtet dessen sollte man auch nicht �bersehen, dass eine Kopie eines Zeigers nicht zu einer
Kopie der Daten f�hrt, auf die ein Zeiger zeigt. Also auch diese Schwachstelle wird von den
automatisch generierten speziellen Member-Funktionen nicht ber�cksichtigt.

Damit kommen wir zum zweiten Entwicklungsschritt der Klasse `SimpleString`,
es sind die Kopieroperationen (Kopier-Konstruktor, kopierender Zuweisungsoperator) explizit realisiert:


```cpp
01: class SimpleString
02: {
03: private:
04:     char*       m_data{};   // pointer to the characters of the string (nullptr)
05:     std::size_t m_elems{};  // number of elements (zero)
06: 
07: public:
08:     // c'tors / d'tor
09:     SimpleString() = default; // empty string
10: 
11:     SimpleString(const char* s)
12:         : m_elems{ std::strlen(s) }
13:     {
14:         m_data = new char[size() + 1];      // need space for terminating '\0'
15:         std::copy(s, s + size(), m_data);
16:         m_data[size()] = '\0';
17:     }
18: 
19:     // copy-constructor
20:     SimpleString(const SimpleString& other)
21:         : m_data{ new char[other.size() + 1] }, m_elems{ other.size() }
22:     {
23:         std::copy(other.m_data, other.m_data + other.size(), m_data);
24:         m_data[size()] = '\0';
25:     }
26: 
27:     ~SimpleString() {
28:         delete[] m_data;
29:     }
30: 
31:     // assignment operator
32:     SimpleString& operator=(const SimpleString& other)
33:     {
34:         delete[] m_data;
35:         m_data = new char[other.size() + 1];
36:         std::copy(other.m_data, other.m_data + other.size(), m_data);
37:         m_elems = other.size();
38:         m_data[size()] = '\0';
39:         return *this;
40:     }
41: 
42:     // getter
43:     std::size_t size() const { return m_elems; }
44:     bool empty() const { return size() == 0; }
45:     const char* data() const { return m_data; }
46: 
47:     // operators (no index-checking intentionally)
48:     char operator[](std::size_t n) const { return m_data[n]; }
49:     char& operator[](std::size_t n) { return m_data[n]; }
50: };
```

Die vorgestelle Realisierung l�uft &ndash; in den meisten F�llen &ndash;,
dennoch weist sie einen schweren Entwurfsfehler auf! Erkennen Sie die Schwachstellen?

#### Variante 3

Die Schwachstellen im Listing von Variante 2 sind die Zeilen 34 und 35.
Wirft der Aufruf des `new`-Operators eine Exception (`std::bad_alloc`),
so befindet sich das `SimpleString`-Objekt in einem inkorrekten Zustand.
Der Zeiger `m_data` zeigt auf falsche Daten, jeglicher Zugriff auf das Objekt zieht *Undefined Behavior* nach sich.

Wir versuchen es mit einem Redesign des Zuweisungsoperators `operator=`:

```cpp
01: SimpleString& operator=(const SimpleString& other)
02: {
03:     char* tmp = new char[other.size() + 1];
04:     delete[] m_data;
05:     m_data = tmp;
06:     std::copy(other.m_data, other.m_data + other.size(), m_data);
07:     m_elems = other.size();
08:     m_data[size()] = '\0';
09:     return *this;
10: }
```

Diese Version ruiniert zumindest kein Objekt, wenn es w�hrend der Ausf�hrung zu einer Ausnahme kommt.
Dennoch weist auch diese Realisierung einen &ndash; subtilen &ndash; Fehler auf! Welchen?


#### Variante 4

Betrachten Sie hierzu folgendes Beispiel, dass Sie mit der Realisierung aus Variante 3 aufrufen:


```cpp
01: SimpleString s{ "Hello World" };
02: s = s;
03: std::println("{}", s.data());
```

Das Programm enth�lt eine Selbstzuweisung. Das ergibt zwar keinen Sinn, in *Production-Ready* Quellcode 
sollte das aber ber�cksichtigt werden. Au�erdem darf nicht �bersehen werden, dass das Beispiel zwar nicht abst�rzt,
aber zu einem unerwarteten Ergebnis f�hrt, da auf Grund der Freigabe von `s` auf der rechten Seite der Wertzuweisung
keine Daten mehr f�r den Kopiervorgang zur linken Seite vorhanden sind:

*Ausgabe*:

```
???????????
```

Damit kommen wir zur n�chsten �berarbeitung des Zuweisungsoperators:


```cpp
01: SimpleString& operator=(const SimpleString& other)
02: {
03:     // prevent self-assignment
04:     if (this == &other) {
05:         return *this;
06:     }
07: 
08:     char* tmp = new char[other.size() + 1];
09:     delete[] m_data;
10:     m_data = tmp;
11:     std::copy(other.m_data, other.m_data + other.size(), m_data);
12:     m_elems = other.size();
13:     m_data[size()] = '\0';
14:     return *this;
15: }
```

Diese Variante des Zuweisungsoperators ist nun fehlerfrei. Okay, sie zeichnet sich nicht gerade durch �bersichtlichkeit aus, 
und die Zeilen 4 bis 6 schmerzen etwas: Sie werden *immer* ausgef�hrt, und damit eben auch in fast allen F�llen,
in denen die beiden Objekte links und rechts von der Wertzuweisung verschieden sind.
Wir haben es mit einem Fall von &bdquo;*Pessimization*&rdquo; zu tun:


##### *Pessimization*

Das Wort *Pessimization* wird im Allgemeinen als Gegenteil von *Optimierung* verwendet und bezeichnet
ein Programmierman�ver oder eine Technik, die das Programmverhalten weniger effizient macht als es sein sollte.

Der obige Fall ist ein bekanntes Beispiel f�r ein solches Man�ver: Jeder zahlt f�r eine potenzielle Sprung-Anweisung (Assembler: `jmp`-Befehl),
der durch die `if`-Anweisung eingeleitet wird, obwohl dieser nur in seltenen und ung�nstigen F�llen erforderlich ist.

Bei einer *Pessimization*-Situation lohnt es sich oft, einen Schritt zur�ckzutreten und zu �berdenken.
Vielleicht haben wir das Problem aus dem falschen Blickwinkel angegangen.

Und damit sind wir beim n�chsten Thema angekommen: Das *Copy-and-Swap-Idiom*.

---

### Das *Copy-and-Swap-Idiom* <a name="link15"></a>

Das *Copy-and-Swap-Idiom* wurde eingef�hrt, um zwei Ziele zu erreichen:

  * Realisierung der Kopier-Konstruktoren und Zuweisungsoperatoren (sowohl &bdquo;kopierende&rdquo; als auch &bdquo;verschiebende&rdquo; Semantik) auf eine einfache Weise (Vermeidung von Code-Duplikationen).
  * Bereitstellung der so genannten *Strong Exception Guarantee*.

Auf die *Strong Exception Guarantee* gehen wir sp�ter ein, wir verweilen beim *Copy-and-Swap-Idiom*:
Dieses besteht im Wesentlichen aus zwei Teilen:


  * Einem destruktiven Teil, der den bestehenden Zustand des Zielobjekts aufr�umt (die linke Seite der Zuweisung).
  * Einem konstruktiven Teil, der den Zustand vom Quellobjekt (rechte Seite der Zuweisung) zum Zielobjekt kopiert.

Der destruktive Teil entspricht im Allgemeinen dem Code im Destruktor des Typs, der konstruktive Teil im Allgemeinen dem Code im Kopierkonstruktor des Typs.

Der Name *Copy-and-Swap* f�r diese Technik r�hrt daher,
dass sie �blicherweise durch eine Kombination aus dem Kopierkonstruktor des Typs,
seinem Destruktor und einer `swap`()-Memberfunktion implementiert wird,
die die Membervariablen einzeln austauscht.

Mit dem *Copy-and-Swap*-Idiom k�nnen wir nun folgende Realisierung des 
Zuweisungsoperators betrachten:

```cpp
01: void swap(SimpleString& other) noexcept
02: {
03:     std::swap(m_data, other.m_data);      // swap data member
04:     std::swap(m_elems, other.m_elems);    // swap data member
05: }
06: 
07: // refined copy assignment operator
08: SimpleString& operator=(SimpleString other) {
09: 
10:     this->swap(other);
11:     return *this;
12: }
```

*Hinweise*:

  * Am Beispiel des Parameters `other` wenden wir eine �u�erst n�tzliche Richtlinie an:
  Wenn Sie in einer Funktion eine Kopie erstellen, lassen Sie den Compiler dies in der Parameterliste tun.

  * Nachdem (durch den Aufruf) die Kopie erstellt ist, k�nnen wir mit dem Tauschen der Membervariableninhalte beginnen.

  * Beachten Sie, dass beim Aufrufen der Funktion alle neuen Daten bereits allokiert, kopiert und einsatzbereit sind.
  Dadurch erhalten wir kostenlos eine *Strong Exception Guarantee* &ndash; dazu sp�ter noch mehr.

  * An diesem Punkt sind wir quasi schon fertig, da `swap` keine Fehler ausl�st.
  Wir tauschen unsere aktuellen Daten mit den kopierten aus, �ndern unseren Zustand sicher und legen die alten Daten in einem tempor�ren Objekt ab.
  
  * Die alten Daten werden dann freigegeben, wenn die Funktion verlassen wird:
  Es endet der G�ltigkeitsbereich des Parameterobjekts und sein Destruktor wird aufgerufen!

  * Beachten Sie, dass die Notwendigkeit einer Selbstzuweisungspr�fung beseitigt wurde
  und eine einheitliche Implementierung des `operator=` erm�glicht wurde.
  Dar�berhinaus gibt es keine Leistungseinbu�en mehr bei Nicht-Selbstzuweisungen.

---

### Verschiebeoperationen <a name="link16"></a>

Wir haben bislang in der Klasse `SimpleString` die
traditionellen Regel der drei speziellen Methoden
Kopier-Konstruktor, Zuweisungsoperator und Destruktor betrachtet.

Seit C++ 11 k�nnen wir solchen Code durch die so genannte *Move-Semantik* effizienter (performanter) gestalten.
Es gesellen sich zwei weitere spezielle Methoden (Verschiebe-Konstruktor, Verschiebe-Zuweisungsoperator) zur Klasse hinzu.

Ihr Aufruf wird vom Compiler implizit aktiviert, wenn der Compiler Objekte bearbeitet,
von denen er wei�, dass sie nicht mehr verwendet werden. Das sind beispielsweise Objekte im Programm,
die keinen Namen haben (tempor�re Objekte, Objekte, die als Zwischenergebnis fungieren).

Am Beispiel der Klasse `SimpleString` k�nnten diese beiden Methoden so aussehen:

```cpp
01: SimpleString(SimpleString&& other) noexcept
02: {
03:     m_data = std::move(other.m_data);
04:     m_elems = std::move(other.m_elems);
05:     other.m_data = nullptr;
06:     other.m_elems = 0;
07: }
08: 
09: SimpleString& operator=(SimpleString&& other) noexcept {
10: 
11:     SimpleString tmp{ std::move(other) };
12:     tmp.swap(*this);
13:     return *this;
14: }
```

Mit Hilfe von `std::exchange` kann man den verschiebenden Konstruktor noch
etwas kompakter realisieren:

```cpp
SimpleString(SimpleString&& other) noexcept
{
    m_data = std::exchange(other.m_data,  nullptr);
    m_elems = std::exchange(other.m_elems, 0);
}
```

Die freie Funktion `std::exchange` funktioniert dabei wie folgt:

```cpp
int z = std::exchange(x, y);
```

Nach der Ausf�hrung dieser Anweisung hat

  * `x` den Wert von `y` zugewiesen bekommen und
  * `z` den urspr�ngliche Wert von `x` zugewiesen bekommen.


---

### Schreiben Sie kleine, fokussierte Funktionen (Methoden) <a name="link17"></a>

Funktionen (Methoden) sind die Bausteine der *Clean Code* Programmierung.
Eine gute Funktion sollte klein und fokussiert sein und genau eine Sache tun.
Kleine Funktionen sind leichter zu verstehen, zu testen und wiederzuverwenden.

Einige Richtlinien zum Schreiben sauberer Funktionen:

  * Halten Sie Funktionen kurz, normalerweise unter 50 Zeilen Code.
  * Geben Sie Funktionen beschreibende Namen, die klar vermitteln, was sie tun.
  * Vermeiden Sie Nebeneffekte &ndash; eine Funktion sollte entweder einen Wert berechnen oder eine Aktion ausf�hren, nicht beides.
  * Bevorzugen Sie &bdquo;reine&rdquo; (*pure*) Funktionen, die bei denselben Eingaben immer das gleiche Ergebnis liefern.
  * Verwenden Sie Parameter f�r die Eingabe und R�ckgabewerte f�r die Ausgabe zur�ck, anstatt sich auf den globalen Status zu verlassen.
  * Halten Sie die Anzahl der Parameter niedrig, normalerweise unter 5.


Stellen Sie sich beispielsweise eine Funktion `isValidUsername` vor,
die einen Benutzernamen validiert. Eine saubere Version k�nnte so aussehen:

```cpp
01: bool isValidUsername(const std::string& username) {
02: 
03:     const auto MinLength{ 8 };
04:     const auto MaxLength{ 30 };
05:         
06:     const std::string ValidCharacters {
07:         "abcdefghijklmnopqrstuvwxyz0123456789_-."
08:     };
09: 
10:     if (username.length() < MinLength || username.length() > MaxLength) {
11:         return false;
12:     }
13: 
14:     for (char ch : username) {
15:         if (ValidCharacters.find(ch) == std::string::npos) {
16:             return false;
17:         }
18:     }
19: 
20:     return true;
21: }
```

Diese Funktion ist kurz (weniger als 20 Zeilen), hat einen eindeutigen Namen, nimmt einen einzigen Eingabeparameter an,
gibt ein boolesches Ergebnis zur�ck und hat keine Nebenwirkungen. Sie ist leicht zu verstehen und kann ausf�hrlichen Unit-Tests unterzogen werden.

---

### Verwenden Sie `const` gro�z�gig <a name="link18"></a>

`const` ist ein leistungsstarkes Sprachfeature in C++, um Absichten auszudr�cken und potenzielle Fehler zur Kompilierzeit abzufangen:

  * Wird es auf eine Variable angewendet, zeigt es an, dass der Wert nicht ge�ndert wird.
  * Wird es auf eine Methode angewendet, zeigt es an, dass die Methode das aufgerufene Objekt nicht �ndert.

Einige Richtlinien zur Verwendung von `const`:

  * Deklarieren Sie Funktionsparameter mit `const`, wenn diese von der Funktion nicht ge�ndert werden.
  * Deklarieren Sie Methoden mit `const`, wenn sie den Zustand des gerufenen Objekts nicht �ndern.
  * Deklarieren Sie Variablen mit `const`, wenn sich ihre Werte nach der Initialisierung nicht �ndern sollen.
  * Verwenden Sie `const`-Referenzen, um unn�tiges Kopieren gro�er Objekte zu vermeiden.

*Beispiel*:

```cpp
01: class Rectangle
02: {
03: private:
04:     double m_width;
05:     double m_height;
06: 
07: public:
08:     Rectangle(double width, double height) 
09:         : m_width(width), m_height(height) 
10:     {}
11: 
12:     double area() const {
13:         return  m_width * m_height;
14:     }
15: 
16:     void scale(double factor) {
17:         m_width *= factor;
18:         m_height *= factor;
19:     }
20: };
21: 
22: void printArea(const Rectangle& rect) {
23:     std::println("Area: {}", rect.area());
24: }
```

In diesem Beispiel wird die Methode `area()` als `const` deklariert, da sie den Zustand des Rechtecks ??nicht �ndert.

Die Funktion `printArea` nimmt ihren Parameter per `const`-Referenz, was bedeutet,
dass sie das Rechteck nicht �ndert und ein unn�tiges Kopieren vermeidet.

Die konsequente Verwendung von `const` macht Ihren Code selbstdokumentierender und hilft,
potenzielle Fehler zu erkennen, wie z. B. versehentliche �nderungen an Werten, die konstant sein sollten.

---


### Ausnahmesicherheit (*Exception Safety*) <a name="link19"></a>

Die Idee hinter der Thematik *Exception Safety* besteht darin, dass Funktionen bzw. eine Klasse und ihre Methoden
ihren Clients eine Art Versprechen bzw. eine Garantie hinsichtlich m�glicherweise ausgel�ster oder nicht ausgel�ster Ausnahmen geben.

Es gibt vier Stufen der *Exception Safety*:

#### 1. Keine Ausnahmesicherheit

Mit dieser niedrigsten Stufe der Ausnahmesicherheit wird im wahrsten Sinne des Wortes &ndash; &bdquo;keine Ausnahmesicherheit&rdquo; &ndash; absolut nichts garantiert.


#### 2. Elementare Ausnahmesicherheit (*Basic Exception Safety*)

Diese Ebene der Ausnahmesicherheit kann mit mit relativ geringem Implementierungsaufwand erreicht werden:

  * Wenn w�hrend eines Funktions- oder Methodenaufrufs eine Ausnahme ausgel�st wird, ist sichergestellt, dass keine Ressourcen verloren gehen!
  * Wenn w�hrend eines Funktions- oder Methodenaufrufs eine Ausnahme ausgel�st wird, kommt es anschlie�end zu keiner Besch�digung der Daten oder allokierten Speichers,
  und alle Objekte befinden sich in einem fehlerfreien und konsistenten Zustand.
  * Aber: Es kann nicht garantiert werden, dass die Daten dieselben sind wie vor dem Aufruf der Funktion oder Methode.


*Bemerkung*:

Die STL erwartet von von allen benutzerdefinierten Datentypen,
dass sie zumindest die *Basic Exception Safety* einhalten.

#### 3. Starke Ausnahmesicherheit (*Strong Exception Safety*)

Die starke Ausnahmesicherheit garantiert alles, was auch die elementare Ausnahmesicherheit gew�hrleistet.
Dar�ber hinaus stellt sie sicher, dass im Ausnahmefall die Daten genauso wiederhergestellt werden, wie sie vor dem Aufruf der Funktion oder Methode waren.

Mit anderen Worten: Mit dieser Ausnahmesicherheitsstufe erhalten wir eine Art *Commit*- oder *Rollback*-Semantik
wie bei der Transaktionsverarbeitung eines Datenbanksystems.


Es ist leicht nachvollziehbar, dass diese Ausnahmesicherheit einen h�heren Implementierungsaufwand bedeutet und zur Laufzeit kostspielig sein kann.

Ein Beispiel f�r diesen zus�tzlichen Aufwand ist das sogenannte *Copy-and-Swap-Idiom*,
das verwendet werden muss, um diese Ebene der Ausnahmesicherheit bei kopierenden
Zuweisungen zu gew�hrleisten.

#### 4. Die No-Throw-Garantie (*No-Throw Guarantee*)

Die vierte und letzte Ebene der Ausnahmesicherheit behandeln wir im n�chsten Abschnitt.

---

### Die No-Throw-Garantie (*No-Throw Guarantee*) <a name="link20"></a>


Diese Ebene der Ausnahmesicherheit ist die h�chste Ausnahmesicherheitsstufe:

  * Vereinfacht ausgedr�ckt bedeutet diese Stufe, dass Sie sich als Aufrufer einer Funktion oder Methode keine Gedanken �ber Ausnahmen machen m�ssen.
  * Der Funktions- oder Methodenaufruf ist erfolgreich. Immer!
  * Es wird niemals eine Ausnahme ausgel�st, da alles intern ordnungsgem�� behandelt wird.

Nat�rlich ist diese Stufe nicht immer ganz einfach zu erreichen, zum Beispiel dann,
wenn mit dem `new`-Operator dynamische Daten ins Spiel kommen.

In den folgenden F�llen ist die No-Throw-Garantie zwingend:

#### 1. Destruktoren

Destruktoren von Klassen m�ssen unter allen Umst�nden die No-Throw-Garantie gew�hren!
Der Grund daf�r ist, dass Destruktoren unter anderem auch w�hrend des so genannten &bdquo;Stack Unwinding&rdquo;-Prozesses aufgerufen werden, wenn eine Ausnahme aufgetreten ist.
Zu diesem Zeitpunkt kann eine geschachtelt auftretende Ausnahme nicht bearbeitet werden,
das laufende Programm reagiert in solchen F�llen mit einer unmittelbaren Terminierung (Aufruf von `terminate()`).

#### 2. Verschiebeoperationen (Move Operations)

Verschiebe-Konstruktoren und Verschiebe-Zuweisungsoperatoren sollten garantiert keine Fehler enthalten.

Wenn eine Verschiebeoperation eine Ausnahme ausl�st, ist die Wahrscheinlichkeit gro�,
dass die Verschiebung nicht stattgefunden hat.

Dar�ber hinaus ist es wichtig, die No-Throw-Garantie f�r benutzerdefinierte Datentypen zu gew�hrleisten,
die f�r die Verwendung mit STL-Containern vorgesehen sind.

Wenn der Verschiebekonstruktor f�r einen Elementtyp in einem Container keine No-Throw-Garantie bietet &ndash;
d.h., der Verschiebekonstruktor ist nicht mit dem Spezifizierer `noexcept` deklariert &ndash;,
verwendet der Container bevorzugt die kopierenden (und damit zeitaufw�ndigeren) Operationen
gegen�ber den verschiebenden Operationen.

#### 3. Konstruktoren

Das Ausl�sen einer Ausnahme in einem Konstruktor ist weder w�nschenswert, und vor allem: Es kann auch vermieden werden!

Ein &bdquo;halbkonstruiertes Objekt&rdquo; wird kaum im weiteren Gebrauch seinen Client zufriedenstellen.

#### 4. `swap`-Funktionen

Eine `swap`-Funktion muss unter allen Umst�nden die No-Throw-Garantie gew�hrleiten.


---

### Das `noexcept` Schl�sselwort <a name="link21"></a>

Der Spezifizierer `noexcept` in der Signatur einer Funktion gibt an,
dass diese Funktion **keine** Ausnahme ausl�sen darf.

Dasselbe gilt f�r `noexcept(true)`, das lediglich ein Synonym f�r `noexcept` ist.

Stattdessen kann eine mit `noexcept(false)` deklarierte Funktion Ausnahme werfen.


Wenn eine als `noexcept` markierte Funktion trotzdem eine Ausnahme ausl�st,
setzt der C++&ndash;Compiler einen `terminate()`-Aufruf ab, um die Anwendung zu beenden.

Hier einige Beispiele:

```cpp
void aNonThrowingFunction() noexcept;
void anotherNonThrowingFunction() noexcept(true);
void aPotentiallyThrowingFunction() noexcept(false);
```

Es gibt zwei gute Gr�nde f�r die Verwendung von `noexcept`:

  * Ausnahmen, die eine Funktion oder Methode ausl�sen kann (oder nicht),
  sollten Teil der Funktionsschnittstelle sein.<br />
  Es hilft Entwicklern beim Lesen des Quellcodes zu erkennen, was passieren kann und was nicht.<br />
  `noexcept` signalisiert Entwicklern, dass sie diese Funktion sicher in ihren eigenen *non-throwing* Funktionen verwenden k�nnen.

  * Zweitens kann diese Information vom Compiler f�r Optimierungen genutzt werden.<br />
  `noexcept` erm�glicht es einem Compiler, die Funktion m�glicherweise ohne den Laufzeit-Overhead zu kompilieren,
  der sonst im Falle des Eintretens von Ausnahme erforderlich w�re.


*Bemerkung*:<br />
Destruktoren einer Klasse sind implizit als `noexcept` definiert.


---

### Ausnahmen (*Exceptions*) sind Fehlercodes (*Error Codes*) vorzuziehen <a name="link22"></a>

Ausnahmen (*Exceptions*) sind die bevorzugte Methode zum Melden und Behandeln von Fehlern in Modern C++.
Sie haben mehrere Vorteile gegen�ber herk�mmlichen Fehlercodes:

  * Ausnahmen k�nnen nicht ignoriert werden, Fehlercodes hingegen schon.
  * Ausnahmen werden automatisch an rufende Methoden weitergegeben, w�hrend Fehlercodes an jeder Aufrufstelle manuell �berpr�ft und manuell weitergegeben werden m�ssen.
  * Ausnahmen trennen den Fehlerbehandlungscode vom normalen Kontrollfluss und machen den Code �bersichtlicher

Einige Richtlinien zur Verwendung von Ausnahmen:

  * Verwenden Sie Ausnahmen f�r au�ergew�hnliche Bedingungen, nicht f�r den normalen Kontrollfluss.
  * Werfen Sie Ausnahmen *by value* und fangen Sie sie *by reference*.
  * Verwenden Sie nach M�glichkeit Standardausnahmetypen wie `std::runtime_error`, `std::out_of_range` usw.
  * Definieren Sie Ihre eigenen Ausnahmetypen f�r dom�nenspezifische Fehler.
  * Vermeiden Sie das Ausl�sen von Ausnahmen in Destruktoren.
  * Verwenden Sie RAII, um sicherzustellen, dass Ressourcen bei Ausnahmen aufger�umt werden.

So k�nnten Sie beispielsweise einen Dateifehler mit Ausnahmen behandeln:

```cpp
01: try {
02:     std::ifstream file("data.txt");
03:             
04:     if (!file.is_open()) {
05:         throw std::runtime_error("Could not open file");
06:     }
07:             
08:     // process the file
09: }
10: catch (const std::exception& e) {
11:     std::cerr << "Error: " << e.what() << std::endl;
12: }
```

Vergleichen Sie dies mit einer Version, die Fehler Codes hantiert:

```cpp
01: std::ifstream file("data.txt");
02: 
03: if (!file.is_open()) {
04:     // handle the error, maybe return an error code  
05: }
06: 
07: // process the file
```

Die Version mit *Exception Handling* trennt die Fehlerbehandlung vom normalen Ablauf
und der Fehler kann nicht ignoriert werden.

---

### R�ckgabetyp einer Methode <a name="link23"></a>

Wir betrachten folgendes Beispiel:

```cpp
01: class Person
02: {
03: private:
04:     std::string   m_name;
05:     size_t        m_age;
06: 
07: public:
08:     Person() = default;
09:     Person(const std::string& name, size_t age)
10:         : m_name{ name }, m_age{ age }
11:     {}
12: 
13:     std::string getName() const { return m_name; }
14:     size_t getAge() const { return m_age; }
15: };
```

*Vorgehensweise*: &bdquo;*Copied to Caller*&rdquo;

Es gibt auch eine zweite M�glichkeit:

```cpp
01: class Person
02: {
03: private:
04:     std::string   m_name;
05:     size_t        m_age;
06: 
07: public:
08:     Person() = default;
09:     Person(const std::string& name, size_t age)
10:         : m_name{ name }, m_age{ age }
11:     {}
12: 
13:     const std::string& getName() const { return m_name; }
14:     size_t getAge() const { return m_age; }
15: };
```

*Vorgehensweise*: &bdquo;*Lifetime by Voucher*&rdquo;

---

### Bevorzuge Komposition der Vererbung gegen�ber <a name="link24"></a>

  * Vererbung ist ein leistungsstarkes Feature der objektorientierten Programmierung,
  wird aber oft �berstrapaziert.

  * Vererbung koppelt die abgeleitete Klasse eng an die Basisklasse
  (Stichwort &bdquo;*Strong Coupling*&rdquo; versus &bdquo;*Lose Coupling*&rdquo;),
  wodurch der Code starrer und schwerer zu �ndern wird.

  * Sie f�hrt au�erdem tendenziell zu tiefen und komplexen Vererbungshierarchien,
  die schwer zu verstehen sind.

  * Komposition hingegen ist eine Technik, bei der eine Klasse Instanzen anderer Klassen enth�lt,
  anstatt von ihnen zu erben.

  * Dies f�hrt zu einem flexibleren und modulareren Design.

*Beispiel*:

Wenn beispielsweise viele unterschiedliche Klassen in einem 3D-Szenario
ein *Transformations*-Eigenschaft (also eine Position, eine Rotation und eine Skalierung) ben�tigen,
k�nnten Sie alle Ihre Klassen von einer Klasse `Transform` ableiten.

Sie k�nnten eine *Transform*-Eigenschaft aber auch als Attribut den Klassen hinzuf�gen:

```cpp
01: class Vector3D{};
02: 
03: class Transform {
04:     Vector3D m_position;
05:     Vector3D m_rotation;
06:     Vector3D m_scale;
07: };
08: 
09: class Character
10: {
11: public:
12:     // functions that might use *m_transform* if they need it
13: 
14: private:
15:     Transform m_transform;
16: };
17: 
18: class Obstacle
19: {
20: public:
21:     // functions that might use *m_transform* if they need it
22: 
23: private:
24:     Transform m_transform;
25: };
```

  * Dadurch haben Sie mehr Kontrolle �ber die Schnittstelle der &bdquo;untergeordneten&rdquo; Klassen.

  * Denn wenn Sie eine �bergeordnete Klasse �ndern, wird die �nderung zwangsl�ufig in allen untergeordneten Klassen widergespiegelt, was oft nicht erw�nscht ist.

  * Oft m�ssen Sie der �bergeordneten Klasse etwas hinzuf�gen, weil eine bestimmte untergeordnete Klasse es braucht. Indem Sie dies tun, zwingen Sie es allen anderen untergeordneten Klassen auf, die es nicht unbedingt brauchen oder wollen.

  * Mit Komposition haben Sie mehr Kontrolle dar�ber.

  * Komposition erm�glicht es auch, kleinere Klassen zu schreiben, anstatt von einer gro�en �bergeordneten Klasse zu erben.

---

### Implizite Konvertierungen vermeiden <a name="link25"></a>

In der Sprache C++ gibt es des Feature so genannter &bdquo;impliziter Typkonvertierungen&rdquo;.

Hierunter versteht man Konvertierungen, die der �bersetzer automatisch durchf�hrt:

  * Sie werden immer dann durchgef�hrt, wenn ein Ausdruck eines Typs `T1` in einem Kontext verwendet wird,
  der diesen Typ nicht akzeptiert, aber einen anderen Typ `T2`.

  * Obwohl diese n�tzlich sein k�nnen, k�nnen sie auch zu unerwarteten Ergebnissen und Fehlern f�hren.

Daher ist es manchmal w�nschenswert, implizite Konvertierungen zu vermeiden.

*Beispiel*:

```cpp
01: class Distance
02: {
03: private:
04:     int m_meters;
05: 
06: public:
07:     Distance(int meters) : m_meters{ meters } {}
08: 
09:     void display() const {
10:         std::println("Distance: {}", m_meters);
11:     }
12: };
13: 
14: void printDistance(const Distance& d) {
15:     d.display();
16: }
17: 
18: void test() {
19:     auto meters{ 10 };
20:     printDistance(meters);
21: }
```

In diesem Beispiel gibt es neben der Klasse `Distance` eine Funktion `printDistance`,
die ein Objekt vom Typ `Distance` als Argument annimmt.

In der rufenden Funktion haben wir jedoch eine `int`-Variable an die Funktion �bergeben
und dennoch wird der Quellcode fehlerfrei �bersetzt und korrekt ausgef�hrt:

Der Compiler hat eine implizite Konvertierung von `int` nach `Distance` durchgef�hrt!

Um implizite Konvertierungen in C++ zu vermeiden, k�nnen wir das Schl�sselwort `explicit` verwenden.

Wenn das Schl�sselwort `explicit` mit einem Konstruktor oder einer Konvertierungsfunktion verwendet wird,
verhindert es implizite Konvertierungen, die automatisch vom Compiler durchgef�hrt werden.

*Hinweis*:
Mit einer expliziten Typwandlung k�nnte man die Konvertierung wieder aktivieren,
aber dieses Mal eben nicht versteckt, sondern sichtbar f�r den Entwickler!

*Frage*:


---

### Schl�sselwort `auto` verwenden oder nicht? <a name="link26"></a>

Empfielt sich der Einsatz des Schl�sselworts `auto` oder nicht?

`auto`: **You totally know what you�re doing, or you totally don�t**

#### Erstes Beispiel

*Frage*:<br />
Warum ist dieses Code-Fragment nicht �bersetzungsf�hig?

```cpp
01: void test()
02: {
03:     std::map<int, std::string> aMap{ { 1, "Hello"  } };
04: 
05:     std::map<int, std::string>::iterator it{ aMap.begin() };
06: 
07:     std::pair<int, std::string>& entry1{ *it };  // Why this line DOES NOT compile ???
08: 
09:     auto& entry2{ *it };                         // This line compiles :)
10: }
```

*Zwei Empfehlungen*:

  * Verwenden Sie `auto` prinzipiell gro�z�gig. Es kann die Lesbarkeit verbessern.
    Der Compiler kann Typen f�r uns besser ableiten als wir selbst (*Type Deduction*).

  * Ist der Typ einer Variablen von entscheidender Natur, dann sollte man diesen auch explizit hinschreiben.


#### Zweites Beispiel

Lambda-Funktionen (pr�ziser: Lambda-*Objekte*) sind zu einem integralen Bestandteil der C++&ndash;Programmierung geworden.
Es gibt im Wesentlichen drei M�glichkeiten, diese zu vereinbaren:

  * <i>Inplace</i> &ndash; also als anonyme Objekte.
  * Mit dem Schl�sselwort `auto`.
  * Mit dem Klassentemplate `std::function`.

Bei Lambda-Funktionen sollten wir den Weg mit `auto` einschlagen. Warum?

Der Typ einer Lambda-Funktion kann von uns als Entwickler nicht hingeschrieben werden (der Bezeichner des Typs ist f�r uns nicht bekannt),
aber `auto` leitet den Typ f�r uns pr�size ab!

Dies bedeutet unter anderem: Zwei Lambda-Funktionen mit denselben Parametern und demselben R�ckgabetyp sind von unterschiedlichem Typ!

Allerdings k�nnten beide Lambda-Funktionen derselben `std::function<>`-Variablen zugewiesen werden.

Das hat aber Nachteile:
Wenn wir eine Lambda-Funktion in eine `std::function<>`-Variable kopieren, kann diese nicht mehr *inline* verwendet werden,
sodass der Aufruf m�glicherweise langsamer ist.
Ferner kann das Kopieren der Lambda-Funktion in eine `std::function<>`-Variable auch Anforderungen an die dynamische Speicherverwaltung nach sich ziehen.

Wenn wir eine Lambda-Funktion mit `auto` deklarieren, vermeiden wir Overhead!

*Beispiel*:

Betrachten Sie zu diesem Beispiel den erzeugten Maschinencode!


```cpp
01: void test()
02: {
03:     std::vector<int> numbers{ 1, 2, 3, 4, 5 };
04: 
05:     auto print = [](int value) { std::print("{} ", value); };
06: 
07:     std::function<void(int)> morePrint = [](int value) { std::print("{} ", value); };
08: 
09:     std::for_each(
10:         numbers.begin(),
11:         numbers.end(),
12:         [](int value) { std::print("{} ", value); }
13:     );
14: 
15:     std::println();
16: 
17:     std::for_each(
18:         numbers.begin(),
19:         numbers.end(),
20:         print
21:     );
22: 
23:     std::println();
24: 
25:     std::for_each(
26:         numbers.begin(),
27:         numbers.end(),
28:         morePrint
29:     );
30: }
```

#### Ein Hinweis

*Achtung*:

Verlust von `const` und `&` bei Verwendung von `auto`:

```cpp
01: class Person
02: {
03: private:
04:     std::string   m_name;
05: 
06: public:
07:     Person() = default;
08:     Person(const std::string& name) : m_name{ name } {}
09: 
10:     const std::string& getName() const { return m_name; }
11: };
12: 
13: void test()
14: {
15:     Person jack{ "Jack", 50 };
16:     auto name = jack.getName();
17: }
```

Es gibt  hier nur eine Warnung zu Zeile 16:<br />
*auto does not deduce references a possibly unintended copy is being made*.

---

### Schl�sselwort `auto`: *Left-to-Right* Initialisierungsstil <a name="link27"></a>

Verwenden Sie das Schl�sselwort `auto`, um damit einen einheitlichen, besser lesbaren Initialisierungsstil
f�r Variablen zu haben:

```cpp
01: auto var1 = 0;
02: auto var2 = Foo{};
03: auto var3 = createFooOject();
04: auto var4 = std::mutex{};
05: auto name = std::string{ "Hans" };
06: auto anotherName = std::make_unique<Foo>(123);
07: auto myLambda = [](auto n, auto m) { return n + m; };
```

Welche Vorteile bietet dieser Initialisierungsstil?

##### 1. Das Konsistenzargument

Im Wesentlichen besteht das Argument darin, dass sich der Standardstil in der Initialisierung von Variablen in C++
in Richtung einer &bdquo;von links nach rechts&rdquo;-Syntax bewegt.

##### 2. Das Initialisierungsargument

Eines der st�rksten Argumente f�r den *Left-to-Right* Initialisierungsstil ist,
dass er das Fehlen eines Initialisierungswerts f�r Variablen/Objekte unm�glich macht.

```cpp
int i;         // bad
```

```cpp
int i = 0;     // better
```

```cpp
auto i;        // impossible, doesn't compile
```

```cpp
auto i = 0;    // good, i is now initialized (to 0) and can be read from
```

##### 3. Das Argument, dass bei der Initialisierung die Konvertierung nicht eingeschr�nkt wird

```cpp
float x = 123.45;        // Hmmm, 123.45 is of type double ?!?
```

Es findet eine &bdquo;*Narrowing Conversion*&rdquo; statt:
`123.45` ist vom Typ `double` und wird in den weniger pr�zisen Typ `float` konvertiert.

```cpp
auto x = 123.45f;
```

Mit dem *Left-to-Right* Initialisierungsstil wurde �berhaupt kein `double`-Wert erzeugt.
Und es findet keine &bdquo;*Narrowing Conversion*&rdquo; statt.


##### 4. Das Argument, dass es die Leistung (fast) nie beeintr�chtigt

Betrachten Sie diesen Ausdruck mit dem *Left-to-Right* Initialisierungsstil:


```cpp
auto name = std::string{ "Hans" };
```

Was verbirgt sich hinter dem Gleichheitszeichen? Wird da nicht eine Kopie von `std::string{ "Hans" }` erstellt?

Theoretisch erzeugt der Ausdruck `std::string{ "Hans" }` ein tempor�res `std::string`-Objekt,
das dann nach `name` verschoben wird.
Diese Syntax k�nnte also die Kosten einer Verschiebung verursachen.

Ab C++ ist das Feature [Copy Elision](https://en.cppreference.com/w/cpp/language/copy_elision) 
in der Sprache per Definition vorhanden.
Deshalb hat der *Left-to-Right* Initialisierungsstil keine Auswirkungen auf die Laufzeitleistung!

*Bemerkung*:
Dieses Feature ist in C++ 14 und kleiner nicht vorhanden &ndash;
hier sollten wir diesen Initialisierungsstil deshalb vermeiden.

##### 5. Das &bdquo;*most vexing*&rdquo; Parse-Argument

```cpp
X x();
```

Diese Deklaration wird vom Compiler als eine Funktionsaufrufdeklaration interpretiert,
die eine Funktion des Namens `x` ohne Parameter deklariert, die ein Objekt vom Typ `X` zur�ckliefert.


```cpp
auto x = X(); // no way to interpret this as a function declaration
```

In der letzten Deklaration ist `x` ein Objekts des Typs `X`.


---

### Konstante Referenzen (`const auto&`) <a name="link28"></a>

Eine `const auto&`-Referenz kann an alles gebunden werden.

Das urspr�ngliche Objekt kann durch eine solche Referenz niemals ver�ndert werden.

Der Gebrauch von `const auto&`-Referenzen f�r Objekte sollte die Vorzugswahl sein,
vor allem dann, wenn das Kopieren des Originalobjekts teuer ist.

Wenn die `const auto&`-Referenz an ein tempor�res Objekt gebunden ist,
verl�ngert sich die Lebensdauer des tempor�ren Objekts angepasst an die Lebensdauer der beteiligten Referenz.

*Beispiel*:

```cpp
01: std::pair<int, int> func(int x) { 
02:     return{ x, x }; 
03: }
04: 
05: auto another_func() {
06: 
07:     const std::pair<int, int>& cv1 = func(1);  // temporary object of type std::pair<int,int>
08:     const std::pair<int, int>& cv2 = func(2);  // temporary object of type std::pair<int,int>
09:     const std::pair<int, int>& cv3 = func(3);  // temporary object of type std::pair<int,int>
10: 
11:     // Temporary Lifetime Extension: 
12:     return cv1.first + cv2.first + cv3.first;;
13:     // cv1, cv2 and cv3 are going out of scope, temporaries will be destroyed right now
14: }
15: 
16: void test()
17: {
18:     auto result = another_func();
19:     std::println("Value: {}", result);
20: }
```

*Hinweis*:

Es ist m�glich, bei Verwendung von `auto&` eine `const auto&`-Referenz zu erhalten.

Dies ist im folgenden Beispiel zu sehen:

```cpp
class Foo
{
public:
    auto val() const {
        return m_value;
    }

    auto& cref() const {
        return m_value;
    }

    auto& mref() {
        return m_value;
    }

private:
    int m_value{};
};

void test()
{
    auto foo = Foo{};
    auto& cref = foo.cref(); // <== cref is a const reference (!!!)
    auto& mref = foo.mref(); // <== mref is a mutable reference
}
```

*Fazit*:<br />
  * Stilistisch sollte man immer `const auto&` verwenden, um explizit auszudr�cken, dass man es mit einer konstanten Referenz zu tun hat!
  * `auto&` wiederum sollte man nur dann verwenden, wenn wir eine ver�nderliche (mutable) Referenz kennzeichnen wollen.


---

### Ver�nderbare Referenzen (`auto&`) <a name="link29"></a>

Im Gegensatz zu einer konstanten Referenz kann eine ver�nderbare Referenz (`auto&`) nicht an eine tempor�re Variable / ein tempor�res Objekt gebunden werden.

Wir verwenden `auto&` immer dann, wenn wir das referenzierte Objekt �ndern m�chten.

---

### *Forwarding* Referenzen (`auto&&`) <a name="link30"></a>

`auto&&` wird als *Forwarding* Referenz (auch universelle Referenz) bezeichnet.

Sie kann an alles gebunden werden, was sie in bestimmten F�llen n�tzlich macht.

*Forwarding* Referenzen verl�ngern, genau wie konstante Referenzen, die Lebensdauer einer tempor�ren Variablen / eines tempor�ren Objekts.

Im Gegensatz zur konstanten Referenz erm�glicht `auto&&` jedoch das �ndern referenzierter Objekte, auch tempor�rer Variablen / tempor�rer Objekte.

Wir verwenden nur dann, wenn wir Variablen/Objekte an anderen, unterlagerten Code weiterleiten.

In diesen Weiterleitungsszenarien spielt es keine Rolle, ob die Variable eine konstante oder ver�nderliche Variable ist;
man m�chte sie lediglich an Code weitergeben, der die Variable tats�chlich verwendet.

*Hinweis*:<br />
  * Wichtig zu beachten: `auto&&` und `T&&` sind nur dann Weiterleitungsreferenzen, wenn sie in einem Funktionstemplate verwendet werden,
  in der `T` ein Templateparameter des Funktionstemplates ist.

  * Die Verwendung der `&&`-Syntax mit einem expliziten Typ, z. B. `std::string&&`, bezeichnet eine *RValue*-Referenz
  und hat nichts mit einer *Forwarding* Referenz zu tun.


---

### `const` Propagation f�r Zeiger <a name="link31"></a>

Mit dem Schl�sselwort `const` k�nnen wir dem Compiler mitteilen,
welche Objekte unver�nderlich sind.
Der Compiler kann dann �berpr�fen, ob wir versuchen, Objekte zu ver�ndern, die nicht ver�ndert werden sollen.

Anders ausgedr�ckt: Der Compiler pr�ft unseren Code auf `const`-Korrektheit.

Ein h�ufiger Fehler beim Schreiben von `const`-korrektem Code in C++ ist,
dass ein `const`-initialisiertes Objekt die Werte, auf die Member-Zeiger zeigen,
weiterhin manipulieren kann.

Das folgende Beispiel veranschaulicht dieses Problem.

```cpp
01: class Foo {
02: public:
03:     Foo(int* ptr) : m_ptr{ ptr } {}
04: 
05:     auto set_value_behind_pointer(int value) const {
06: 
07:         *m_ptr = value;  // compiles despite function being declared const!
08:     }
09: 
10:     auto print() const
11:     {
12:         std::println("{}", *m_ptr);
13:     }
14: 
15: private:
16:     int* m_ptr{};
17: };
18: 
19: void test()
20: {
21:     auto i = 0;
22: 
23:     const auto foo = Foo{ &i };
24: 
25:     foo.print();
26:     foo.set_value_behind_pointer(123);
27:     foo.print();
28: }
```

Die Funktion `set_value_behind_pointer()` ver�ndert den `int`-Wert,
obwohl sie als `const` deklariert ist.

Es wird eben nicht der Zeiger `m_ptr` selbst ver�ndert,
sondern nur der `int`-Wert, auf den der Zeiger zeigt.

Um dies lesbar zu verhindern, wurde der Standardbibliothek ein Wrapper namens
`propagate_const` hinzugef�gt.

*Bemerkung*:<br />
Aktuell ist dieser Wrapper nur im Namensraum `std::experimental` verf�gbar,
und Visual C++ unterst�tzt dieses Feature aktuell �berhaupt nicht,
daf�r aber der GCC:


```cpp
01: class FooImproved {
02: public:
03:     FooImproved(int* ptr) : m_ptr{ ptr } {}
04: 
05:     auto print() const
06:     {
07:         std::println("{}", *m_ptr);
08:     }
09: 
10:     auto set_value_behind_pointer(int value) const
11:     {
12:         *m_ptr = value;     // will not compile, const is propagated
13:     }
14: 
15: private:
16:     std::experimental::propagate_const<int*> m_ptr{ nullptr };
17:     int m_value{};
18: };
```

Siehe in Zeile 16 die Verwendung des `const`-propagierenden Wrappers `std::experimental::propagate_const`
f�r Zeiger.

---

### Strong Typing <a name="link32"></a>

Was versteht man unter dem Begriff &bdquo;*Strong Typing*&rdquo;?

Ein *Strong Type* ist ein Typ, der anstelle eines anderen Typs verwendet wird
und durch seinen Namen eine bestimmte Bedeutung vermittelt.

Im Gegensatz zu starken Typen stehen meistens elementare Datentypen wie beispielsweise `int` oder `double`.

Oftmals sagen elementare Typen nicht viel �ber die Bedeutung ihrer Variablen aus,
und es ist auch nicht m�glich, Einschr�nkungen bzgl. des Wertebereichs ohne Weiteres vornehmen zu k�nnen.

*Strong Types* sind meist Wrapper-Klassen f�r eine Variable elementaren Datentyps,
um diese bzgl. ihres Wertebereichs und ihres Verhaltens besser kontrollieren zu k�nnen.

*Erstes Beispiel*:

```cpp
01: enum class Suit
02: {
03:     Hearts,   // Herz
04:     Diamonds, // Karo
05:     Clubs,    // Kreuz
06:     Spades    // Pik
07: };
08: 
09: struct Card
10: {
11:     int value{};
12:     Suit suit{};
13: };
14: 
15: void test()
16: {
17:     Card card{ 2, Suit::Diamonds };
18:     std::cout << card << std::endl;
19: }
```


*Zweites Beispiel*:

```cpp
01: class FaceValue
02: {
03: private:
04:     int m_value;
05: 
06: public:
07:     explicit FaceValue(int value) : m_value{ value }
08:     {
09:         if (m_value < 7 || m_value > 14)
10:         {
11:             throw std::invalid_argument("Face value invalid");
12:         }
13:     }
14: 
15:     int value() const
16:     {
17:         return m_value;
18:     }
19: };
20: 
21: class Card
22: {
23: private:
24:     FaceValue m_value{ 7 };
25:     Suit m_suit{};
26: 
27: public:
28:     Card() = default;
29:     Card(FaceValue value, Suit suit) : m_value{ value }, m_suit{ suit } {}
30: 
31:     FaceValue value() const { return m_value; }
32:     Suit suit() const { return m_suit; }
33: };
34: 
35: void test()
36: {
37:     // Card card{ 2, Suit::Diamonds };              // does not compile (!!!)
38:                 
39:     Card defaultCard{};                             // compiles and runs
40:     std::cout << defaultCard << std::endl;
41: 
42:     Card pikAss{ FaceValue{ 14 }, Suit::Spades };   // compiles and runs
43:     std::cout << pikAss << std::endl;
44: }
```


Im zweiten Beispiel wurde der Kartenwert der Karte

  * 7, 8, 9, 10: Ebenfalls 7, 8, 9 oder 10
  * Bube: Jack
  * Dame: Queen
  * K�nig: King
  * Ass: Ace

durch eine Klasse `FaceValue` umgesetzt.
Dies erm�glicht gegen�ber dem Datentyp `int` eine doch sch�rfere 
Hantierung aller Regeln, die mit einer bestimmten Instanzvariablen verbunden sind.


---

###  `if`- und *Range-based* `for`-Anweisungen mit Initialisierer <a name="link33"></a>

Ab C++ 17 kann eine `if`-Anweisung einen *Initialisierer* enthalten,
der eine benannte Variable deklariert und initialisiert.

Diese Form der `if`-Anweisung bietet sich an,
wenn die Variable ausschlie�lich innerhalb des Blocks der `if`-Anweisung ben�tigt wird:.

1. *Beispiel*:

```cpp
01: void test()
02: {
03:     std::map<int, std::string> m;
04:     if (auto it = m.find(10); it != m.end()) {
05:         auto size = it->second.size();
06:     }
07: }
```

2. *Beispiel*:

```cpp
01: void test()
02: {
03:     std::vector<int> numbers = { 1, 2, 3, 4, 5 };
04:     for (int i{}; auto n : numbers) {
05: 
06:         std::println("{:02}: {}", i, n);
07:         ++i;
08:     }
09: }
```

---

# Literatur

---

[Zur�ck](./Readme_Guidelines.md)

---
