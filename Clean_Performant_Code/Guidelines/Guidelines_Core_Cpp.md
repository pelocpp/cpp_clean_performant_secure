# Guidelines Core C++

[Zurück](Guidelines.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link19)

---

## Allgemeines <a name="link1"></a>

---

## Klasse (`class`) oder Struktur (`struct`)

#### Klasse (`class`) oder Struktur (`struct`)?

  * Verwenden Sie Strukturen (`struct`), wenn der zu konzipierende Datentyp
   hauptsächlich ein Halter von Daten ist.
  * Verwenden Sie Klassen (`class`), wenn für den zu konzipierendem Datentyp
   die Daten eher als Implementierungsdetails anzusehen sind
   und es das Verhalten (Methoden) ist, was man nach außen zeigen möchte.

#### Klasse (`class`) oder Struktur (`struct`)?

Eine Richtlinie stammt von Bjarne Stroustrup *himself*:

  * Verwenden Sie Klasse (`class`), wenn es *Invarianten* gibt.
  * Verwenden Sie Strukturen (`struct`), wenn alle Datenelemente sich unabhängig voneinander verändern können.

Es bleibt nur noch zu klären, was man unter einer *Invarianten* versteht?

#### Klasse (`class`) oder Struktur (`struct`)?

Wenn ein (oder mehrere Elemente) mit `private` zu kennzeichnen sind, sollte man `class` verwenden.

#### Interfaces (Schnittstellen) wozu?

*Interfaces* stellen nicht nur eine Beschreibung von Methoden dar, sie bewirken damit
auch eine *Hervorhebung*.

Zur Implementierung in einer Klasse wird ein Bezug zur Schnittstelle gegeben.

In einer Klasse lassen sich Daten kapseln, die relevanten Methoden der Schnittstelle
sind hervorgehoben.

#### Minimieren Sie die Sichtbarkeit von Mitgliedern einer Klasse

Ohne Worte :)

#### Lebenszyklus eines Objekts: Spezielle Member-Funktionen

C++ definiert für den Lebenszyklus eines Objekts eine Reihe von Methoden/Operatoren,
die eine besondere Rolle einnehmen.

Die Schnittstellen dieser Operationen sehen so aus:

| Beschreibung | Schnittstelle |
| :-------- | :------- |
| Standardkonstruktor<br/>(Default Constructor) | `X()` |
| Kopierkonstruktor<br/>(Copy Constructor) | `X(const X&)` |
| Kopierende Zuweisung<br/>(Assignment Operator) | `X& operator=(const X&)` |
| Verschiebekonstruktor<br/>(Move Constructor) | `X(X&&) noexcept` |
| Verschiebende Zuweisung<br/>(Move Assignment) | `X& operator=(X&&) noexcept` |
| Destruktor<br/>(Destructor) | `~X()` |

*Tabelle*: Spezielle Member-Funktionen

#### Automatische Erzeugung der speziellen Member-Funktionen: Wann

In manchen Situationen nimmt der Compiler einem Entwickler die Arbeit ab und 
erzeugt für eine oder mehrere der speziellen Member-Funktionen eine Realisierung.

Das gesamte Regelwerk in diesem Umfeld ist nicht ganz trivial,
einen Überblick versucht das folgende Bild zu geben:

<img src="cpp_special_member_functions.svg" width="700">

*Abbildung* 1: Überblick über die automatische Erzeugung spezieller Member-Funktionen.

Aus *Abbildung* 1 kann man einige wesentliche Aussagen ableiten:

  * Standardkonstruktor:<br/>Der Standardkonstruktor wird automatisch generiert, wenn Sie in der betroffenen Klasse selbst *übnerhaupt keinen* Konstruktor implementiert haben.
  * Kopierkonstruktor:<br/>Der Kopierkonstruktor wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Kopierende Zuweisung:<br/>Die Kopierzuweisung wird automatisch generiert, wenn Sie keine der beiden Verschiebeoperationen implementieren.
  * Verschiebekonstruktor:<br/>Der Verschiebekonstruktor wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden können.
  * Verschiebende Zuweisung:<br/>Die verschiebende Zuweisung wird automatisch generiert, wenn Sie keine der beiden Kopieroperationen implementieren und alle Elemente verschoben werden können.
  * Destruktor:<br/>Der Destruktor wird automatisch generiert, wenn Sie keinen Destruktor implementiert haben.

Was erkennen wir an diesen Aussagen?

Die beiden *Kopieroperationen* sind unabhängig voneinander:
Wenn in einer Klasse eine der beiden Kopieroperationen definiert ist,
könnte der Übersetzer immer noch die andere Operation automatisch implementieren.

Für die beiden *Verschiebeoperationen* gilt das Gegenteil:
Wird vom Entwickler eine der beiden Operationen implementiert,
stellt der Compiler *keine* Realisierung der anderen Operation bereit.

#### Automatische Erzeugung der speziellen Member-Funktionen: Wie

Natürlich wäre es interessant zu wissen, wie der Quellcode von automatisch erzeugten  Member-Funktionen aussieht.
Das ist keine triviale Frage, dennoch kann man ein paar Richtlinien erkennen:

  * Standardkonstruktor:<br/>
    * Für alle Instanzvariablen, die Objekte sind, wird deren Standardkonstruktor aufgerufen.
    * Für alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt **keine** Initialisierung, also auch keine Vorbelegung mit einem datentypspezifischen Null-Wert!

  * Kopierende Operationen:<br/>
    Es werden alle Instanzvariablen bitweise kopiert.
    Für elementare Instanzvariablen ist das eine gute Vorgehensweise, haben wir es mit (klassischen) Zeigervariablen zu tun,
    entstehen Probleme (Stichwort: flache versus tiefe Kopie).

  * Verschiebende Operationen:<br/>
    * Für alle Instanzvariablen, die Objekte sind, wird deren entsprechende Verschiebeoperation aufgerufen, falls vorhanden. Anderenfalls erfolgt eine Kopie.
    * Für alle Instanzvariablen, die elementar sind (`int`, `double`), erfolgt eine bitweise Kopie.


#### Explicitly defaulted Default Constructors

Um das manuelle Schreiben leerer Standardkonstruktoren zu vermeiden,
unterstützt C++ das Konzept von *Explicitly defaulted* Standardkonstruktoren.

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


#### Explicitly deleted Default Constructors

Gregoier // Seite 275 // Verion 6

Auch das Gegenteil von explizit standardmäßigen Standardkonstruktoren ist möglich
und wird als explizit gelöschte Standardkonstruktoren bezeichnet.

Sie können beispielsweise eine Klasse mit nur statischen Memberfunktionen definieren
für die Sie keine Konstruktoren schreiben möchten und für die Sie auch nicht möchten,
dass der Compiler den Standardkonstruktor generiert.

In diesem Fall müssen Sie die Standardeinstellung explizit löschen.
Konstruktor.


#### Automatische Erzeugung spezieller Member-Funktionen: Fazit

Was sieht der einfachste Hinweis aus:

**Rule-of-ZERO**

  * Alles sechs speziellen Member-Funktionen dürfen weder implementiert noch mit `= delete` gelöscht werden.
  * Der Compiler generiert alle Member-Funktionen so, wie es benötigt wird.
  * Das funktioniert natürlich nur dann, wenn die automatisch erzeugten operattionen das tun, was sie müssen:
    Korrekt initialisieren, kopieren und verschieben &ndash; und keine Ressourcen übrig lassen.
  * Wann funktioniert das? Wenn man sich auf folgenden Member-Variablen einschränkt:
   * Standard Datentypen
   * Aggregate (Strukturen)
   * STL Standard Container
   * Smart Pointer
 * Was geht nicht: Klassische Zeiger, die auf mit `new` allokierte Daten zeigen
 * Referenzen in den Instanzvariablen



---

[Zurück](Guidelines.md)

---

*Beispiel*:

```cpp
```


*Ausgabe*:

```
```
