# *InfInt*: Eine Bibliothek für sehr große Zahlen

[Zurück](../Secure_Programming/Readme_Secure_Programming.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Boost.Multiprecision](#link2)
  * [Eine leichgewichtige Alternative: *InfInt* ](#link3)
  * [Zwei Beispiele mit *InfInt*](#link4)
  * [Literatur](#link5)

---

#### Quellcode

[*InfInt.h*](InfInt.h)<br />
[*InfIntExamples.cpp*](InfIntExamples.cpp)

---

## Allgemeines <a name="link1"></a>

Die Arithmetik von ganzen Zahlen (Zahl ohne Dezimalpunkt) als auch von Gleitkommazahlen (Zahl mit Dezimalpunkt)
muss in C++&ndash;Programmen immer innerhalb eines bestimmten Wertebereichs stattfinden.
Wird dieser Wertebereich verlassen, kommt es zu Rechenungenauigkeiten oder noch schlimmer, zu falschen Resultaten.

Wird mit den intrinsischen Datentypen höherer Programmiersprachen (wie z. B. `int`, `long`, `float`, `double` etc. in C++)
gearbeitet, gibt es keine unmittelbare Lösung für dieses Problem.

Für eine Arithmetik mit beliebiger Genauigkeit benötigt man spezielle Realisierungen (externe Bibliotheken, Header-Files),
die neue Datentypen und Rechenoperationen (Strukturen oder Klassen, Funktionen, Methoden) bereitstellen,
die Zahlen mit der geforderten Genauigkeit speichern und exakte Berechnungen durchführen können.

---

## Boost.Multiprecision <a name="link2"></a>

Dies ist der Goldstandard für professionelle C++ Anwendungen.

Die Bibliothek ist ein Teil der Boost-Bibliothek,
die Typen für ganze, rationale, Gleitkomma- und komplexe Zahlen bereitstellt (z. B. eine Klasse namens `cpp_int`),
welche einen weitaus größeren Wertebereich und eine höhere Präzision bieten als die standardmäßigen integrierten Typen (wie `double` oder `long long`).

Sie bietet eine konsistente, reine Header-Schnittstelle für Anwendungen, die eine hohe Präzision benötigen.

### Wichtige Merkmale und Funktionen:

  * Beliebige Präzision: Ermöglicht Berechnungen, die lediglich durch den verfügbaren Arbeitsspeicher begrenzt sind,
  oder solche mit fester Präzision (z. B. 100 Dezimalstellen).
  * Verfügbare Typen: Umfasst `cpp_int` (ganze Zahlen mit beliebiger Präzision), `cpp_bin_float` (binäre Gleitkommazahlen) und `cpp_dec_float` (dezimale Gleitkommazahlen).
  * Einfache Integration: Als reine Header-Bibliothek vereinfacht sie die Einbindung in bestehende C++-Projekte, da keine vorkompilierten Bibliotheken erforderlich sind.
  * Interoperabilität: Typen aus Boost.Multiprecision können mit den integrierten C++-Typen interagieren.

### Typische Anwendungsfälle:

  * Wissenschaftliche Simulationen mit hoher Präzision.
  * Kryptografie, die sehr große ganze Zahlen erfordert.
  * Finanzmathematische Berechnungen, die exakte, hochpräzise Dezimalwerte benötigen.
  * Berechnung mathematischer Konstanten auf viele Nachkommastellen.

Die Bibliothek genießt hohes Ansehen, da sie die Lücke zwischen der standardmäßigen Arithmetik mit begrenzter Präzision und spezialisierter, komplexer mathematischer Software schließt.

---

## Eine leichtgewichtige Alternative: *InfInt*  <a name="link3"></a>

Wenn Sie keine riesige Bibliothek wie Boost einbinden wollen, ist InfInt eine hervorragende Alternative.

Sie besteht aus nur einer einzigen Header-Datei.

  * Vorteile: Extrem einfach zu integrieren – einfach die Datei InfInt.h in Ihr Projekt kopieren und fertig.
  * Fokus: Reine Funktionalität für große Ganzzahlen ohne Schnickschnack.

---

## Zwei Beispiele mit *InfInt* <a name="link4"></a>

*Beispiel*:

```cpp
01: void test() {
02: 
03:     // testing addition
04:     InfInt n1 = InfInt{ "999999999999999999999999999999" };
05:     InfInt n2 = InfInt{ "1" };
06:     InfInt n = n1 + n2;
07:     std::println("{} + {} = {}", n1, n2, n);
08: }
```


*Ausgabe*:

```cpp
999999999999999999999999999999 + 1 = 1000000000000000000000000000000
```

*Beispiel*:

```cpp
01: class BigFactorial
02: {
03: public:
04:     template <typename T>
05:     static T factorial(const T& n)
06:     {
07:         if (n == static_cast<T> (1)) {
08:             return static_cast<T> (1);
09:         }
10:         else {
11:             return n * factorial(n - static_cast<T>(1));
12:         }
13:     }
14: };
15: 
16: void test() {
17: 
18:     for (std::size_t n{ 2 }; n < 51; ++n)
19:     {
20:         InfInt f{ BigFactorial::factorial<InfInt>(n) };
21:         std::println("Faculty of {:2}: {}", n, f);
22:     }
23: }
```


*Ausgabe*:

```cpp
Faculty of  2: 2
Faculty of  3: 6
Faculty of  4: 24
Faculty of  5: 120
Faculty of  6: 720
Faculty of  7: 5040
Faculty of  8: 40320
Faculty of  9: 362880
Faculty of 10: 3628800
Faculty of 11: 39916800
Faculty of 12: 479001600
Faculty of 13: 6227020800
Faculty of 14: 87178291200
Faculty of 15: 1307674368000
Faculty of 16: 20922789888000
Faculty of 17: 355687428096000
Faculty of 18: 6402373705728000
Faculty of 19: 121645100408832000
Faculty of 20: 2432902008176640000
Faculty of 21: 51090942171709440000
Faculty of 22: 1124000727777607680000
Faculty of 23: 25852016738884976640000
Faculty of 24: 620448401733239439360000
Faculty of 25: 15511210043330985984000000
Faculty of 26: 403291461126605635584000000
Faculty of 27: 10888869450418352160768000000
Faculty of 28: 304888344611713860501504000000
Faculty of 29: 8841761993739701954543616000000
Faculty of 30: 265252859812191058636308480000000
Faculty of 31: 8222838654177922817725562880000000
Faculty of 32: 263130836933693530167218012160000000
Faculty of 33: 8683317618811886495518194401280000000
Faculty of 34: 295232799039604140847618609643520000000
Faculty of 35: 10333147966386144929666651337523200000000
Faculty of 36: 371993326789901217467999448150835200000000
Faculty of 37: 13763753091226345046315979581580902400000000
Faculty of 38: 523022617466601111760007224100074291200000000
Faculty of 39: 20397882081197443358640281739902897356800000000
Faculty of 40: 815915283247897734345611269596115894272000000000
Faculty of 41: 33452526613163807108170062053440751665152000000000
Faculty of 42: 1405006117752879898543142606244511569936384000000000
Faculty of 43: 60415263063373835637355132068513997507264512000000000
Faculty of 44: 2658271574788448768043625811014615890319638528000000000
Faculty of 45: 119622220865480194561963161495657715064383733760000000000
Faculty of 46: 5502622159812088949850305428800254892961651752960000000000
Faculty of 47: 258623241511168180642964355153611979969197632389120000000000
Faculty of 48: 12413915592536072670862289047373375038521486354677760000000000
Faculty of 49: 608281864034267560872252163321295376887552831379210240000000000
Faculty of 50: 30414093201713378043612608166064768844377641568960512000000000000
```

---

## Literatur <a name="link5"></a>

---

  * [*Boost Multiprecision Library*](https://github.com/boostorg/multiprecision)<br/>
    (abgerufen am 15.05.2026).


  * [*infint &ndash; Arbitrary-sized integer class for C++*](https://sercantutar.github.io/infint/)<br/>
    (abgerufen am 15.05.2026).

oder auf Github:

  * [*infint &ndash; Arbitrary-Precision Integer Arithmetic*](https://github.com/sercantutar/infint)<br/>
    (abgerufen am 15.05.2026).

---

[Zurück](../Secure_Programming/Readme_Secure_Programming.md)

---
