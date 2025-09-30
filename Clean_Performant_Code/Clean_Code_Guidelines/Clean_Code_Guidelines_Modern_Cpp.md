# Richtlinien Clean Code &bdquo;Modernes C++&rdquo;

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

  * [Schlüsselwort `auto` verwenden oder nicht? / *Automatic Type Deduction*](#link1)
  * [Schlüsselwort `auto`: *Left-to-Right* Initialisierungsstil](#link2)
  * [Konstante Referenzen (`const auto&`)](#link3)
  * [Veränderbare Referenzen (`auto&`)](#link4)
  * [*Forwarding* Referenzen (`auto&&`)](#link5)
  * [Nutzung von Smart Pointer Klassen (`std::unique_ptr`, `std::shared_ptr` und `std::weak_ptr`)](#link6)
  * [Ressourcenverwaltung mit RAII](#link7)
  * [Verwenden Sie Container und Algorithmen der C++&ndash;Bibliothek STL](#link8)
  * [C-Style Arrays oder `std::array` zusammen mit `std::span`?](#link9)
  * [Wozu gibt es den &bdquo;*lightweight*&rdquo; Container `std::initializer_list`?](#link10)
  * [Welche Vorteile bietet die Klasse `std::string_view` gegenüber `std::string`?](#link11)
  * [Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`)](#link12)
  * [Bessere Lesbarkeit durch *Structured Binding*](#link13)
  * [Vorteile von Lambda-Objekten](#link14)
  * [Typreiche Programmierung (*Type-Rich Programming*, *Type Traits*)](#link15)
  * [Verwendung von Konzepten (`concept`) und Anforderungen (`requires`)](#link16)
  * [Ausnahmesicherheit (*Exception Safety*)](#link17)
  * [Die No-Throw-Garantie (*No-Throw Guarantee*)](#link18)
  * [Das `noexcept` Schlüsselwort](#link19)
  * [*Strong Types*](#link20)
  * [`const` Propagation für Zeiger](#link21)
  * [Literatur](#link22)

---

#### Quellcode

[*Clean_Code_Guidelines_Modern_Cpp.cpp*](Clean_Code_Guidelines_Modern_Cpp.cpp)

---

### Schlüsselwort `auto` verwenden oder nicht? / *Automatic Type Deduction* <a name="link1"></a>

Empfielt sich der Einsatz des Schlüsselworts `auto` oder nicht?

`auto`: **You totally know what you’re doing, or you totally don’t**

#### Erstes Beispiel

*Frage*:<br />
Warum ist dieses Code-Fragment nicht übersetzungsfähig?

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

  * Verwenden Sie `auto` prinzipiell großzügig. Es kann die Lesbarkeit verbessern.
    Der Compiler kann Typen für uns besser ableiten als wir selbst (*Type Deduction*).

  * Ist der Typ einer Variablen von entscheidender Natur, dann sollte man diesen auch explizit hinschreiben.


#### Zweites Beispiel

Lambda-Funktionen (präziser: Lambda-*Objekte*) sind zu einem integralen Bestandteil der C++&ndash;Programmierung geworden.
Es gibt im Wesentlichen drei Möglichkeiten, diese zu vereinbaren:

  * <i>Inplace</i> &ndash; also als anonyme Objekte.
  * Mit dem Schlüsselwort `auto`.
  * Mit dem Klassentemplate `std::function<>`.

Bei Lambda-Funktionen sollten wir den Weg mit `auto` einschlagen. Warum?

Der Typ einer Lambda-Funktion kann von uns als Entwickler nicht hingeschrieben werden (der Bezeichner des Typs ist für uns nicht bekannt),
aber `auto` leitet den Typ für uns präsize ab!

Dies bedeutet unter anderem: Zwei Lambda-Funktionen mit denselben Parametern und demselben Rückgabetyp sind von unterschiedlichem Typ!

Allerdings könnten beide Lambda-Funktionen derselben `std::function<>`-Variablen zugewiesen werden.

Das hat aber Nachteile:
Wenn wir eine Lambda-Funktion in eine `std::function<>`-Variable kopieren, kann diese nicht mehr *inline* verwendet werden,
sodass der Aufruf möglicherweise langsamer ist.
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

Es gibt hier nur eine Warnung zu Zeile 16:<br />
*auto does not deduce references a possibly unintended copy is being made*.

---

### Schlüsselwort `auto`: *Left-to-Right* Initialisierungsstil <a name="link2"></a>

Verwenden Sie das Schlüsselwort `auto`, um damit einen einheitlichen, besser lesbaren Initialisierungsstil
für Variablen zu haben:

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

Eines der stärksten Argumente für den *Left-to-Right* Initialisierungsstil ist,
dass er das Fehlen eines Initialisierungswerts für Variablen/Objekte unmöglich macht.

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

##### 3. Das Argument, dass bei der Initialisierung die Konvertierung nicht eingeschränkt wird

```cpp
float x = 123.45;        // Hmmm, 123.45 is of type double ?!?
```

Es findet eine &bdquo;*Narrowing Conversion*&rdquo; statt:
`123.45` ist vom Typ `double` und wird in den weniger präzisen Typ `float` konvertiert.

```cpp
auto x = 123.45f;
```

Mit dem *Left-to-Right* Initialisierungsstil wurde überhaupt kein `double`-Wert erzeugt.
Und es findet keine &bdquo;*Narrowing Conversion*&rdquo; statt.


##### 4. Das Argument, dass es die Leistung (fast) nie beeinträchtigt

Betrachten Sie diesen Ausdruck mit dem *Left-to-Right* Initialisierungsstil:


```cpp
auto name = std::string{ "Hans" };
```

Was verbirgt sich hinter dem Gleichheitszeichen? Wird da nicht eine Kopie von `std::string{ "Hans" }` erstellt?

Theoretisch erzeugt der Ausdruck `std::string{ "Hans" }` ein temporäres `std::string`-Objekt,
das dann nach `name` verschoben wird.
Diese Syntax könnte also die Kosten einer Verschiebung verursachen.

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
die eine Funktion des Namens `x` ohne Parameter deklariert, die ein Objekt vom Typ `X` zurückliefert.


```cpp
auto x = X(); // no way to interpret this as a function declaration
```

In der letzten Deklaration ist `x` ein Objekts des Typs `X`.


---

### Konstante Referenzen (`const auto&`) <a name="link3"></a>

Eine `const auto&`-Referenz kann an alles gebunden werden.

Das ursprüngliche Objekt kann durch eine solche Referenz niemals verändert werden.

Der Gebrauch von `const auto&`-Referenzen für Objekte sollte die Vorzugswahl sein,
vor allem dann, wenn das Kopieren des Originalobjekts teuer ist.

Wenn die `const auto&`-Referenz an ein temporäres Objekt gebunden ist,
verlängert sich die Lebensdauer des temporären Objekts angepasst an die Lebensdauer der beteiligten Referenz.

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

Es ist möglich, bei Verwendung von `auto&` eine `const auto&`-Referenz zu erhalten.

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
  * Stilistisch sollte man immer `const auto&` verwenden, um explizit auszudrücken, dass man es mit einer konstanten Referenz zu tun hat!
  * `auto&` wiederum sollte man nur dann verwenden, wenn wir eine veränderliche (mutable) Referenz kennzeichnen wollen.


---

### Veränderbare Referenzen (`auto&`) <a name="link4"></a>

Im Gegensatz zu einer konstanten Referenz kann eine veränderbare Referenz (`auto&`) nicht an eine temporäre Variable / ein temporäres Objekt gebunden werden.

Wir verwenden `auto&` immer dann, wenn wir das referenzierte Objekt ändern möchten.

---

### *Forwarding* Referenzen (`auto&&`) <a name="link5"></a>

`auto&&` wird als *Forwarding* Referenz (auch universelle Referenz) bezeichnet.

Sie kann an alles gebunden werden, was sie in bestimmten Fällen nützlich macht.

*Forwarding* Referenzen verlängern, genau wie konstante Referenzen, die Lebensdauer einer temporären Variablen / eines temporären Objekts.

Im Gegensatz zur konstanten Referenz ermöglicht `auto&&` jedoch das Ändern referenzierter Objekte, auch temporärer Variablen / temporärer Objekte.

Wir verwenden nur dann, wenn wir Variablen/Objekte an anderen, unterlagerten Code weiterleiten.

In diesen Weiterleitungsszenarien spielt es keine Rolle, ob die Variable eine konstante oder veränderliche Variable ist;
man möchte sie lediglich an Code weitergeben, der die Variable tatsächlich verwendet.

*Hinweis*:<br />
  * Wichtig zu beachten: `auto&&` und `T&&` sind nur dann Weiterleitungsreferenzen, wenn sie in einem Funktionstemplate verwendet werden,
  in der `T` ein Templateparameter des Funktionstemplates ist.

  * Die Verwendung der `&&`-Syntax mit einem expliziten Typ, z. B. `std::string&&`, bezeichnet eine *RValue*-Referenz
  und hat nichts mit einer *Forwarding* Referenz zu tun.

---

### Nutzung von Smart Pointer Klassen (`std::unique_ptr`, `std::shared_ptr` und `std::weak_ptr` <a name="link6"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Ressourcenverwaltung mit RAII <a name="link7"></a>

Siehe Repository [*C++ Design Patterns*](https://github.com/pelocpp/cpp_design_patterns).

---

### Verwenden Sie Container und Algorithmen der C++&ndash;Bibliothek STL <a name="link8"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### C-Style Arrays oder `std::array` zusammen mit `std::span`? <a name="link9"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Wozu gibt es den &bdquo;*lightweight*&rdquo; Container `std::initializer_list`? <a name="link10"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Welche Vorteile bietet die Klasse `std::string_view` gegenüber `std::string`? <a name="link11"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Perfektes Weiterleiten (*Perfect Forwarding*, `std::forward`) <a name="link12"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Bessere Lesbarkeit durch *Structured Binding* <a name="link13"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Vorteile von Lambda-Objekten <a name="link14"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Typreiche Programmierung (*Type-Rich Programming*, *Type Traits*) <a name="link15"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Verwendung von Konzepten (`concept`) und Anforderungen (`requires`) <a name="link16"></a>

Siehe Repository [*Modern C++*](https://github.com/pelocpp/cpp_modern).

---

### Ausnahmesicherheit (*Exception Safety*) <a name="link17"></a>

Die Idee hinter der Thematik *Exception Safety* besteht darin, dass Funktionen bzw. eine Klasse und ihre Methoden
ihren Clients eine Art Versprechen bzw. eine Garantie hinsichtlich möglicherweise ausgelöster oder nicht ausgelöster Ausnahmen geben.

Es gibt vier Stufen der *Exception Safety*:

#### 1. Keine Ausnahmesicherheit

Mit dieser niedrigsten Stufe der Ausnahmesicherheit wird im wahrsten Sinne des Wortes &ndash; &bdquo;keine Ausnahmesicherheit&rdquo; &ndash; absolut nichts garantiert.


#### 2. Elementare Ausnahmesicherheit (*Basic Exception Safety*)

Diese Ebene der Ausnahmesicherheit kann mit mit relativ geringem Implementierungsaufwand erreicht werden:

  * Wenn während eines Funktions- oder Methodenaufrufs eine Ausnahme ausgelöst wird, ist sichergestellt, dass keine Ressourcen verloren gehen!
  * Wenn während eines Funktions- oder Methodenaufrufs eine Ausnahme ausgelöst wird, kommt es anschließend zu keiner Beschädigung der Daten oder allokierten Speichers,
  und alle Objekte befinden sich in einem fehlerfreien und konsistenten Zustand.
  * Aber: Es kann nicht garantiert werden, dass die Daten dieselben sind wie vor dem Aufruf der Funktion oder Methode.


*Bemerkung*:

Die STL erwartet von von allen benutzerdefinierten Datentypen,
dass sie zumindest die *Basic Exception Safety* einhalten.

#### 3. Starke Ausnahmesicherheit (*Strong Exception Safety*)

Die starke Ausnahmesicherheit garantiert alles, was auch die elementare Ausnahmesicherheit gewährleistet.
Darüber hinaus stellt sie sicher, dass im Ausnahmefall die Daten genauso wiederhergestellt werden, wie sie vor dem Aufruf der Funktion oder Methode waren.

Mit anderen Worten: Mit dieser Ausnahmesicherheitsstufe erhalten wir eine Art *Commit*- oder *Rollback*-Semantik
wie bei der Transaktionsverarbeitung eines Datenbanksystems.


Es ist leicht nachvollziehbar, dass diese Ausnahmesicherheit einen höheren Implementierungsaufwand bedeutet und zur Laufzeit kostspielig sein kann.

Ein Beispiel für diesen zusätzlichen Aufwand ist das sogenannte *Copy-and-Swap-Idiom*,
das verwendet werden muss, um diese Ebene der Ausnahmesicherheit bei kopierenden
Zuweisungen zu gewährleisten.

#### 4. Die No-Throw-Garantie (*No-Throw Guarantee*)

Die vierte und letzte Ebene der Ausnahmesicherheit behandeln wir im nächsten Abschnitt.

---

### Die No-Throw-Garantie (*No-Throw Guarantee*) <a name="link18"></a>


Diese Ebene der Ausnahmesicherheit ist die höchste Ausnahmesicherheitsstufe:

  * Vereinfacht ausgedrückt bedeutet diese Stufe, dass Sie sich als Aufrufer einer Funktion oder Methode keine Gedanken über Ausnahmen machen müssen.
  * Der Funktions- oder Methodenaufruf ist erfolgreich. Immer!
  * Es wird niemals eine Ausnahme ausgelöst, da alles intern ordnungsgemäß behandelt wird.

Natürlich ist diese Stufe nicht immer ganz einfach zu erreichen, zum Beispiel dann,
wenn mit dem `new`-Operator dynamische Daten ins Spiel kommen.

In den folgenden Fällen ist die No-Throw-Garantie zwingend:

#### 1. Destruktoren

Destruktoren von Klassen müssen unter allen Umständen die No-Throw-Garantie gewähren!
Der Grund dafür ist, dass Destruktoren unter anderem auch während des so genannten &bdquo;Stack Unwinding&rdquo;-Prozesses aufgerufen werden, wenn eine Ausnahme aufgetreten ist.
Zu diesem Zeitpunkt kann eine geschachtelt auftretende Ausnahme nicht bearbeitet werden,
das laufende Programm reagiert in solchen Fällen mit einer unmittelbaren Terminierung (Aufruf von `terminate()`).

#### 2. Verschiebeoperationen (Move Operations)

Verschiebe-Konstruktoren und Verschiebe-Zuweisungsoperatoren sollten garantiert keine Fehler enthalten.

Wenn eine Verschiebeoperation eine Ausnahme auslöst, ist die Wahrscheinlichkeit groß,
dass die Verschiebung nicht stattgefunden hat.

Darüber hinaus ist es wichtig, die No-Throw-Garantie für benutzerdefinierte Datentypen zu gewährleisten,
die für die Verwendung mit STL-Containern vorgesehen sind.

Wenn der Verschiebekonstruktor für einen Elementtyp in einem Container keine No-Throw-Garantie bietet &ndash;
d.h., der Verschiebekonstruktor ist nicht mit dem Spezifizierer `noexcept` deklariert &ndash;,
verwendet der Container bevorzugt die kopierenden (und damit zeitaufwändigeren) Operationen
gegenüber den verschiebenden Operationen.

#### 3. Konstruktoren

Das Auslösen einer Ausnahme in einem Konstruktor ist weder wünschenswert, und vor allem: Es kann auch vermieden werden!

Ein &bdquo;halbkonstruiertes Objekt&rdquo; wird kaum im weiteren Gebrauch seinen Client zufriedenstellen.

#### 4. `swap`-Funktionen

Eine `swap`-Funktion muss unter allen Umständen die No-Throw-Garantie gewährleiten.

---

### Das `noexcept` Schlüsselwort <a name="link19"></a>

Der Spezifizierer `noexcept` in der Signatur einer Funktion gibt an,
dass diese Funktion **keine** Ausnahme auslösen darf.

Dasselbe gilt für `noexcept(true)`, das lediglich ein Synonym für `noexcept` ist.

Stattdessen kann eine mit `noexcept(false)` deklarierte Funktion Ausnahme werfen.

Wenn eine als `noexcept` markierte Funktion trotzdem eine Ausnahme auslöst,
setzt der C++&ndash;Compiler einen `terminate()`-Aufruf ab, um die Anwendung zu beenden.

Hier einige Beispiele:

```cpp
void aNonThrowingFunction() noexcept;
void anotherNonThrowingFunction() noexcept(true);
void aPotentiallyThrowingFunction() noexcept(false);
```

Es gibt zwei gute Gründe für die Verwendung von `noexcept`:

  * Ausnahmen, die eine Funktion oder Methode auslösen kann (oder nicht),
  sollten Teil der Funktionsschnittstelle sein.<br />
  Es hilft Entwicklern beim Lesen des Quellcodes zu erkennen, was passieren kann und was nicht.<br />
  `noexcept` signalisiert Entwicklern, dass sie diese Funktion sicher in ihren eigenen *non-throwing* Funktionen verwenden können.

  * Zweitens kann diese Information vom Compiler für Optimierungen genutzt werden.<br />
  `noexcept` ermöglicht es einem Compiler, die Funktion möglicherweise ohne den Laufzeit-Overhead zu kompilieren,
  der sonst im Falle des Eintretens von Ausnahme erforderlich wäre.


*Bemerkung*:<br />
Destruktoren einer Klasse sind implizit als `noexcept` definiert.


---

### *Strong Types* <a name="link20"></a>

Was versteht man unter dem Begriff &bdquo;*Strong Typing*&rdquo;?

Ein *Strong Type* ist ein Typ, der anstelle eines anderen Typs verwendet wird
und durch seinen Namen eine bestimmte Bedeutung vermittelt.

Im Gegensatz zu starken Typen stehen meistens elementare Datentypen wie beispielsweise `int` oder `double`.

Oftmals sagen elementare Typen nicht viel über die Bedeutung ihrer Variablen aus,
und es ist auch nicht möglich, Einschränkungen bzgl. des Wertebereichs ohne Weiteres vornehmen zu können.

*Strong Types* sind meist Wrapper-Klassen für eine Variable elementaren Datentyps,
um diese bzgl. ihres Wertebereichs und ihres Verhaltens besser kontrollieren zu können.

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
  * König: King
  * Ass: Ace

durch eine Klasse `FaceValue` umgesetzt.
Dies ermöglicht gegenüber dem Datentyp `int` eine doch schärfere 
Hantierung aller Regeln, die mit einer bestimmten Instanzvariablen verbunden sind.

---

### `const` Propagation für Zeiger <a name="link21"></a>

Mit dem Schlüsselwort `const` können wir dem Compiler mitteilen,
welche Objekte unveränderlich sind.
Der Compiler kann dann überprüfen, ob wir versuchen, Objekte zu verändern, die nicht verändert werden sollen.

Anders ausgedrückt: Der Compiler prüft unseren Code auf `const`-Korrektheit.

Ein häufiger Fehler beim Schreiben von `const`-korrektem Code in C++ ist,
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

Die Funktion `set_value_behind_pointer()` verändert den `int`-Wert,
obwohl sie als `const` deklariert ist.

Es wird eben nicht der Zeiger `m_ptr` selbst verändert,
sondern nur der `int`-Wert, auf den der Zeiger zeigt.

Um dies lesbar zu verhindern, wurde der Standardbibliothek ein Wrapper namens
`propagate_const` hinzugefügt.

*Bemerkung*:<br />
Aktuell ist dieser Wrapper nur im Namensraum `std::experimental` verfügbar,
und Visual C++ unterstützt dieses Feature aktuell überhaupt nicht,
dafür aber der GCC:


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
für Zeiger.

---

# Literatur <a name="link22"></a>

Zum Schlagwort &bdquo;*Temporary Lifetime Extension*&rdquo;
gibt es im Netz zwei interessante Aufsätze:

[Temporary Lifetime Extension: Mistakes and Solutions](https://hackernoon.com/temporary-lifetime-extension-mistakes-and-solutions)

und

[Lifetime extension of temporary objects in C++](https://pvs-studio.com/en/blog/posts/cpp/1006).

---

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---
