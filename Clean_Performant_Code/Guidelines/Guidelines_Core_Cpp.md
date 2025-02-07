# Guidelines Core C++

[Zurück](Guidelines.md)

---

## Inhalt

  * [Zum Ersten: Klasse (`class`) oder Struktur (`struct`)?](#link1)
  * [Zum Zweiten: Klasse (`class`) oder Struktur (`struct`)?](#link2)
  * [Zum Dritten: Klasse (`class`) oder Struktur (`struct`)?](#link3)
  * [Interfaces (Schnittstellen): Wozu?](#link4)
  * [Minimieren Sie die Sichtbarkeit von Mitgliedern einer Klasse](#link5)
  * [Lebenszyklus eines Objekts: Spezielle Member-Funktionen](#link6)
  * [Automatische Erzeugung der speziellen Member-Funktionen: Wann?](#link7)
  * [Automatische Erzeugung der speziellen Member-Funktionen: Wie?](#link8)
  * [*Explicitly defaulted* Standardkonstruktor](#link9)
  * [*Explicitly deleted* Standardkonstruktor](#link10)
  * [Automatische Erzeugung spezieller Member-Funktionen: Fazit](#link11)
  * [Literatur](#link)
---

#### Zum Ersten: Klasse (`class`) oder Struktur (`struct`)? <a name="link1"></a>

  * Verwenden Sie Strukturen (`struct`), wenn der zu konzipierende Datentyp
   hauptsächlich ein Halter von Daten ist.
  * Verwenden Sie Klassen (`class`), wenn für den zu konzipierenden Datentyp
   die Daten eher als Implementierungsdetails anzusehen sind
   und es das Verhalten (Methoden) ist, was man nach außen zeigen möchte.

#### Zum Zweiten: Klasse (`class`) oder Struktur (`struct`)? <a name="link2"></a>

Eine Richtlinie stammt von Bjarne Stroustrup *himself*:

  * Verwenden Sie Klasse (`class`), wenn es *Invarianten* gibt.
  * Verwenden Sie Strukturen (`struct`), wenn alle Datenelemente sich unabhängig voneinander verändern können.

Es bleibt nur noch zu klären, was man unter einer *Invarianten* versteht?

#### Zum Dritten: Klasse (`class`) oder Struktur (`struct`)? <a name="link3"></a>

Wenn ein (oder mehrere Elemente) mit `private` zu kennzeichnen sind, sollte man `class` verwenden.

#### Interfaces (Schnittstellen): Wozu? <a name="link4"></a>

*Interfaces* stellen nicht nur eine Beschreibung von Methoden dar, sie bewirken damit
auch eine *Hervorhebung*:

  * Zur Implementierung in einer Klasse wird ein Bezug zur Schnittstelle gegeben.
  * In einer Klasse lassen sich Daten kapseln, die relevanten Methoden der Schnittstell sind hervorgehoben.

#### Minimieren Sie die Sichtbarkeit von Mitgliedern einer Klasse <a name="link5"></a>

Ohne Worte :)

#### Lebenszyklus eines Objekts: Spezielle Member-Funktionen <a name="link6"></a>

C++ definiert für den Lebenszyklus eines Objekts eine Reihe von Methoden/Operatoren,
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

#### Automatische Erzeugung der speziellen Member-Funktionen: Wann? <a name="link7"></a>

In manchen Situationen nimmt der Compiler einem Entwickler die Arbeit ab und 
erzeugt für eine oder mehrere der speziellen Member-Funktionen eine Realisierung.

Das gesamte Regelwerk in diesem Umfeld ist nicht ganz trivial,
einen Überblick versucht das folgende Bild zu geben ([hier](https://howardhinnant.github.io/classdecl.html) vorgefunden):

<img src="cpp_special_member_functions.svg" width="700">

*Abbildung* 1: Überblick über die automatische Erzeugung spezieller Member-Funktionen.

Aus *Abbildung* 1 kann man einige wesentliche Aussagen ableiten:

  * Standardkonstruktor:<br/>Der Standardkonstruktor wird automatisch generiert, wenn Sie in der betroffenen Klasse selbst *überhaupt keinen* Konstruktor implementiert haben.
  * Kopierkonstruktor:<br/>Der Kopierkonstruktor wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Kopierende Zuweisung:<br/>Die Kopierzuweisung wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Verschiebekonstruktor:<br/>Der Verschiebekonstruktor wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden können.
  * Verschiebende Zuweisung:<br/>Die verschiebende Zuweisung wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden können.
  * Destruktor:<br/>Der Destruktor wird automatisch generiert, wenn Sie keinen Destruktor implementiert haben.

Was erkennen wir an diesen Aussagen?

  * Die beiden *Kopieroperationen* sind unabhängig voneinander:
  Wenn in einer Klasse eine der beiden Kopieroperationen definiert ist,
  könnte der Übersetzer immer noch die andere Operation automatisch erzeugen.

  * Für die beiden *Verschiebeoperationen* gilt das Gegenteil:
  Wird vom Entwickler eine der beiden Operationen implementiert,
  stellt der Compiler *keine* Realisierung der anderen Operation bereit.

#### Automatische Erzeugung der speziellen Member-Funktionen: Wie? <a name="link8"></a>

Natürlich wäre es interessant zu wissen, wie der Quellcode von automatisch erzeugten Member-Funktionen aussieht.
Das ist keine triviale Frage, dennoch kann man ein paar Richtlinien erkennen:

  * Standardkonstruktor:<br/>
    * Für alle Instanzvariablen, die Objekte sind, wird deren Standardkonstruktor aufgerufen.
    * Für alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt **keine** Initialisierung, also auch keine Vorbelegung mit einem datentypspezifischen Null-Wert!

  * Kopierende Operationen:<br/>
    * Das Objekt wird elementweise kopiert.<br/>
    * Darunter ist zu verstehen, dass für jedes Element, das selbst wieder ein Objekt
    oder eine Variable elementaren Datentyps ist, der zugehörige Zuweisungsoperator aufgerufen wird.
    Für Variablen elementaren Datentyps (und dazu zählen auch Zeigervariablen) bewirkt dies eine bitweise Kopie.
    * Falls Konstanten oder Referenzen vorhanden sind, geschieht dies jedoch nicht, weil sie
    nur initialisiert, aber nicht durch Zuweisung verändert werden können.
    Der Zuweisungsoperator wird vom System in diesem Fall *nicht* erzeugt.
    * *Hinweis*:<br/>Im Falle  von (klassischen) Zeigervariablen entstehen in diesem Fall Probleme (Stichwort: flache versus tiefe Kopie).

  * Verschiebende Operationen:<br/>
    * Für alle Instanzvariablen, die Objekte sind, wird deren entsprechende Verschiebeoperation aufgerufen, falls vorhanden. Anderenfalls erfolgt eine Kopie.
    * Für alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt eine bitweise Kopie.


#### *Explicitly defaulted* Standardkonstruktor <a name="link9"></a>

Um das manuelle Schreiben leerer Standardkonstruktoren zu vermeiden,
unterstützt C++ das Konzept von *explicitly defaulted* Standardkonstruktoren (*Explicitly defaulted Default Constructors*).

Dadurch können Sie die Klassendefinition wie folgt schreiben,
ohne eine leere Implementierung für den Standardkonstruktor bereitstellen zu müssen:

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
da dieses mithilfe des Schlüsselworts `default` explizit festgelegt wird.

Sie können das `= default` direkt in die Klassendefinition einfügen,
damit kann man sich in der Klassenimplementierung eine leere Implementierung für den Standardkonstruktor sparen.

Zur Vermeidung von Missverständnissen:

Die Implementierung des automatisch erzeugten Standardkonstruktors ist so,
als hätte man `= default` in der Klassendefinition geschrieben.
Diese Aussage gilt für alle speziellen Member-Funktionen.


#### *Explicitly deleted* Standardkonstruktor <a name="link10"></a>

Auch das Gegenteil von *explicitly defaulted Default Constructors* ist möglich
und wird als *Explicitly deleted Default Constructor* bezeichnet.

Sie können beispielsweise eine Klasse mit nur statischen Memberfunktionen definieren
für die Sie keine Konstruktoren schreiben möchten und für die Sie auch nicht möchten,
dass der Compiler den Standardkonstruktor automatisch generiert.

In diesem Fall müssen Sie den Standardkonstruktor explizit löschen:

```cpp
class X
{
public:
    X() = delete;
};
```

#### Automatische Erzeugung spezieller Member-Funktionen: Fazit <a name="link11"></a>

Was sieht die einfachste Strategie aus:

**Rule-of-Zero**

  * Alles sechs speziellen Member-Funktionen dürfen weder implementiert noch mit `= delete` gelöscht werden.
  * Der Compiler generiert alle Member-Funktionen so, wie es benötigt wird.
  * Das funktioniert natürlich nur dann, wenn die automatisch erzeugten Operationen das tun, was sie müssen:
    Korrekt initialisieren, kopieren und verschieben &ndash; und keine Ressourcen übrig lassen.
  * Wann funktioniert das? Wenn man sich auf folgenden Member-Variablen einschränkt:
    * Standard Datentypen
    * Aggregate (Strukturen)
    * STL Standard Container
    * Smart Pointer
 * Was geht nicht:
    * Wenn in den Instanzvariablen klassische Zeiger, die auf mit `new` allokierte Daten zeigen, vorhanden sind.
    * Wenn Referenzen in den Instanzvariablen vorhanden sind.

Wenn keine der fünf speziellen Member-Funktionen selbst geschrieben werden (*Rule-of-Zero*),
weil Sie geschickt auf `new` usw. verzichten, werden diese sämtlich vom Compiler erzeugt,
einschließlich des verschiebenden Konstruktors und des verschiebenden Zuweisungsoperators.

Damit wird Ihr Programmcode performant, einfacher und wartbarer &ndash; und es spart Schreibarbeit.

---

[Zurück](Guidelines.md)

---
