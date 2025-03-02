# Klassische Prinzipien der Clean Code Programmierung

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

  * [KISS](#link1)
  * [YAGNI](#link2)
  * [DRY](#link3)
  * [POLS](#link4)
  * [*Koh�sion* und *Kopplung*](#link5)
  * [RAII](#link6)
  * [Literatur](#link7)

---

#### Quellcode

[*Principles.cpp*](Principles.cpp)<br />

---

## KISS <a name="link1"></a>

KISS ist ein Akronym f�r &bdquo;*Keep it simple and stupid*&bdquo;.

Das KISS-Prinzip besagt, dass Einfachheit ein Hauptziel bei der Softwareentwicklung sein sollte
und dass unn�tige Komplexit�t vermieden werden sollte.

KISS ist eines der Prinzipien ist, die Entwickler bei der Softwareentwicklung oft vergessen.

Es ist ratsam, nicht jedes ausgefallene Feature einer Programmiersprache oder coole Designmuster zu verwenden,
nur weil Sie es k�nnen.

Halten Sie Ihren Code so einfach wie m�glich! Wenn hohe Qualit�tsanforderungen an Flexibilit�t und Erweiterbarkeit gestellt werden,
m�ssen Sie nat�rlich Komplexit�t hinzuf�gen, um diese Anforderungen zu erf�llen.

Sie k�nnen beispielsweise das bekannte &bdquo;Strategie Entwurfsmuster&rdquo; verwenden,
um einen flexiblen Variationspunkt in Ihren Code einzuf�hren, wenn die Anforderungen dies erfordern.

Aber seien Sie vorsichtig und f�gen Sie nur so viel Komplexit�t hinzu, dass solche Dinge einfacher werden.

---

## YAGNI <a name="link2"></a>

YAGNI ist ein Akronym f�r &bdquo;You ain�t gonna need it!&rdquo;.

Das Prinzip besagt, dass Sie keinen Code schreiben sollten, der im Moment nicht erforderlich ist, aber in Zukunft erforderlich sein k�nnte.

Viele Entwickler denken bei der Programmierung eines Programms gerne ein paar Schritte voraus
und programmieren einige zus�tzliche Funktionen, &bdquo;nur f�r den Fall, dass wir sie brauchen&rdquo;
oder zumindest &bdquo;irgendwann einmal brauchen werden&rdquo;.

Nur 5 W�rter: �You Aren�t Gonna Need It�.

Halten Sie sich an YAGNI: Schreiben Sie Qellcode erst, wenn Sie wissen, dass sie ihn wirklich ben�tigen, nicht vorher.

#### Dem Yagni-Prinzip folgen: Ein Selbsttest

Folgen Sie dem Yagni-Prinzip? Wenn ja, wie genau machen Sie das?
Hier ist ein einfacher Test, der Ihnen dabei helfen kann, das zu beurteilen.
Nehmen Sie ihn aber nicht zu ernst.

*Aufgabe*: Erstellen Sie eine Funktion, die die Summe von 2 und 3 berechnet. Wie w�rden Sie diese Funktion implementieren?

*Erste Antwort*:

So w�rden die meisten Entwickler vorgehen:

```cpp
01: static int sum(int x, int y)
02: {
03:     return x + y;
04: }
```

Sehen Sie hier ein Problem? In diesem Code gibt es eine voreilige Verallgemeinerung:
Statt einer Methode, die die Summe von 2 und 3 berechnet, wie es in der Problemdefinition gefordert wurde,
haben wir eine Funktion implementiert, die die Summe zweier beliebiger Zahlen zur�ckgibt.

Hier ist die Version, die dem Yagni-Prinzip folgt:

```cpp
01: static int sumTwoAndThree()
02: {
03:     return 2 + 3;
04: }
```

Nat�rlich ist das nur ein Spielzeugbeispiel, aber es zeigt, wie tief in unserem Geist die Gewohnheit verankert ist,
Verallgemeinerungen vorzunehmen.

---

## DRY <a name="link3"></a>

Wie oft entdeckt man �hnlichen Quellcode in verschiedenen Teilen innerhalb eines Programms?

Genau hier kommt DRY ins Spiel, was im Deutschen so viel bedeutet wie &bdquo;Wiederhole Dich nicht&rdquo; (&bdquo; Don�t Repeat Yourself&rdquo;).

Das DRY-Prinzip, formuliert von Andrew Hunt und David Thomas, besagt, das Code-Wiederholungen zu vermeiden sind.

Also Finger weg von Copy&Paste.

Die DRY-Programmierung kann besonders in gro�en Anwendungen n�tzlich sein, in denen Code st�ndig von vielen Programmierern gepflegt, ge�ndert und erweitert wird.

---

## POLS <a name="link4"></a>

Das &bdquo;Principle of Least Surprise&rdquo; (&bdquo;Prinzip der geringsten �berraschung&rdquo;),
auch unter der Abk�rzung POLS bekannt, ist eine goldene Regel in der Software-Ergonomie, der Mensch-Computer-Interaktion und dem Interfacedesign.

Diese Regel wurde z. B. 1987 von *Geoffrey James* in seinem Buch *The Tao of Programming* als &bdquo;Law of Least Astonishment&rdquo; formuliert.
Sie besagt, dass eine Benutzerschnittstelle so ausgelegt werden sollte, dass der Benutzer m�glichst wenige �berraschungen erlebt.

Das &bdquo;Principle of Least Surprise&rdquo; wird auch auf den Quellcode von Anwendungen erweitert.
Hierbei sollen Objekte wie Variablen, Funktionen, Methoden, Klassen und dergleichen derart benannt werden,
dass deren Funktion und m�gliche Nebenwirkungen klar erkenntlich sind.

*Beispiel*:

```cpp
std::shared_ptr<Customer> getCustomer(int customerId);
```

Gibt einen Kunden anhand einer eindeutigen Identifikationsnummer zur�ck.
Sollte der Kunde nicht gefunden werden, tritt eine Ausnahme auf. Die Methode besitzt keine Nebenwirkungen.

```cpp
std::shared_ptr<Customer> getCustomerOrEmpty(int customerId);
```

Gibt einen Kunden anhand einer eindeutigen Identifikationsnummer zur�ck.
Sollte der Kunde nicht gefunden werden, wird ein leeres std::shared_ptr<Customer>-Objekt zur�ckgeliefert.
Die Methode besitzt keine Nebenwirkungen.

---

## *Koh�sion* und *Kopplung* (*Cohesion* and *Coupling*) <a name="link5"></a>

Wir wenden uns zun�chst einer Erkl�rung der beiden Begrifflichkeiten zu:

W�hrend *Kopplung* ein ziemlich intuitives Konzept ist, das hei�t, dass fast niemand Schwierigkeiten hat, es zu verstehen,
ist der Begriff der *Koh�sion* schwieriger zu begreifen.

Dar�ber hinaus scheinen die Unterschiede zwischen den beiden Begriffen oft unklar zu sein.
Das ist nicht �berraschend: Die Ideen hinter diesen Begriffen sind tats�chlich �hnlich.

Dennoch unterscheiden sie sich:

#### *Koh�sion*

  * *Koh�sion* stellt den Grad dar, in dem ein Teil einer Codebasis eine logisch einzelne, atomare Einheit bildet.

  * Sie kann auch als Anzahl der Verbindungen innerhalb einer Codeeinheit ausgedr�ckt werden.

  * Wenn die Anzahl niedrig ist, sind die Grenzen f�r die Einheit wahrscheinlich schlecht gew�hlt,
  der Code innerhalb der Einheit ist nicht logisch verbunden.

  * Eine Einheit ist hier nicht unbedingt eine Klasse.
  Es k�nnte eine Methode, eine Klasse, eine Gruppe von Klassen oder sogar ein Modul oder eine Baugruppe sein:


Der Begriff der Koh�sion (sowie der Kopplung) ist auf verschiedenen Ebenen anwendbar,
wir f�hren unsere Betrachtungen des Begriffs der *Koh�sion*
mit Klassen fort:

  * Klassen sollten eine kleine Anzahl von Instanzvariablen haben.
  * Jede Methode einer Klasse sollte eine oder mehrere dieser Variablen ver�ndern.
  * Im Allgemeinen gilt: Je mehr Variablen eine Methode ver�ndert, desto koh�siver ist diese Methode f�r ihre Klasse.
  * Eine Klasse, in der jede Variable von jeder Methode verwendet wird, ist maximal koh�siv.
  * Im Allgemeinen ist es weder ratsam noch m�glich, solche maximal koh�siven Klassen zu erstellen.
  * Andererseits kann eine eine starke Koh�sion auch von Vorteil sein:
   Eine starke Koh�sion hat zur Folge,
   dass die Methoden und Variablen der Klasse voneinander abh�ngig sind und als logisches Ganzes zusammenh�ngen.

#### *Kopplung*

  * Die *Kopplung* hingegen stellt den Grad dar, in dem eine einzelne Einheit von anderen unabh�ngig ist.

  * Mit anderen Worten handelt es sich um die Anzahl der Verbindungen zwischen zwei oder mehr Einheiten.

  * Je geringer die Anzahl, desto geringer die Kopplung.

#### Richtlinie f�r starke Koh�sion, geringe Kopplung

  * starke Koh�sion bedeutet im Wesentlichen, dass Teile einer Codebasis, die miteinander in Beziehung stehen,
  an einem einzigen Ort aufbewahrt werden.

  * Geringe Kopplung bedeutet gleichzeitig, nicht miteinander in Beziehung stehende Teile der Codebasis
  so weit wie m�glich voneinander zu trennen.

---

## RAII <a name="link6"></a>

Siehe Repository *Entwurfsmuster*.

---

## Literatur <a name="link6"></a>

Eine gute Beschreibung der Begriffe *Koh�sion* und *Kopplung*  
wird in [Cohesion and Coupling: the difference](https://enterprisecraftsmanship.com/posts/cohesion-coupling-difference/) gegeben.

---

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---
