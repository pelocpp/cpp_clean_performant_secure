# Catch2

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

In diesem Repository findet man einfache, einführende Beispiele
zum &bdquo;*Catch2 Unit Testing Framework*&rdquo; vor.

Weitere Informationen findet man [hier](https://github.com/catchorg/Catch2).

  * [Einige Hinweise zu *Catch2*](#link1)
  * [Einige Best Practices zum Unit Testing](#link2)
  

---

## Einige Hinweise zu *Catch2* <a name="link1"></a>

#### *Catch2* kann über die Kommandozeile gesteuert werden

<img src="Command_Line_Arguments.png" width="500">

*Abbildung* 1: Visual Studio und Kommandozeilen Argumente.

#### `TEST_CASE` und `SECTION`

  * **TEST_CASE** ( *test_name* [, *tags* ] )

  * **SECTION** ( *section_name*, [, *section_description* ] )

Die *section_description* kann verwendet werden, um eine ausführliche Beschreibung eines Abschnitts bereitzustellen,
während *section_name* für die Verwendung mit der Kommandozeile vorgesehen ist.


#### `REQUIRE` und `CHECK`

  * **REQUIRE** ( *expression* ) und
  * **CHECK** ( *expression* )

`REQUIRE` testet einen Ausdruck und bricht den Testfall ab, wenn dieser fehlschlägt.

`CHECK` ist gleichwertig zu `REQUIRE`, die Ausführung wird jedoch im selben Testfall fortgesetzt, auch wenn die Assertion fehlschlägt.


#### Testfälle im BDD-Stil (*Behaviour Driven Development*)

Zusätzlich zu Catch's Schreibweise des klassischen Testfallstils
unterstützt Catch eine alternative Syntax, die es ermöglicht,
Tests als &bdquo;ausführbare Spezifikationen&rdquo; zu schreiben,
quasi im Sinne des *Behaviour Driven Development*.


  * **SCENARIO** ( *scenario_name* [, *tags* ] )

Dieses Makro wird auf `TEST_CASE` abgebildet und funktioniert auf die gleiche Weise,
außer dass dem Testfallnamen das Präfix SCENARIO vorangestellt wird.

  * **GIVEN** ( *something* )
  * **WHEN** ( *something* )
  * **THEN** ( *something* )


#### Typparametrisierte Testfälle


Zusätzlich zu `TEST_CASE` unterstützt Catch2 auch nach Typen parametrisierte Testfälle
in Form von `TEMPLATE_TEST_CASE`:


  * **TEMPLATE_TEST_CASE** ( *test_name* , *tags*, *type_1*, *type_2*, ... , *type_n* )



#### *Matchers*

Matcher sind eine alternative Möglichkeit, Assertions zu schreiben,
nützlich für Tests, bei denen Sie mit komplexen Typen arbeiten oder komplexere Eigenschaften überprüfen müssen:

Matcher werden am häufigsten zusammen mit den Makros `REQUIRE_THAT` oder CHECK_THAT verwendet.

---

## Einige Best Practices zum Unit Testing <a name="link2"></a>

### Beachte die Namensgebung für Unit Tests 

Der Name eines Unit Tests sollte aus drei Teilen bestehen:

  * Name der zu testenden Methode / Funktion
  * Szenario, in dem die Methode getestet wird
  * Erwartetes Verhalten beim Aufruf des Szenarios

Benennungsstandards sind wichtig, da sie den Zweck und die Anwendung des Tests verdeutlichen.
Tests stellen nicht nur sicher, dass Ihr Code funktioniert.
Sie dienen auch der Dokumentation.
Allein durch die Betrachtung der Unit-Tests sollten Sie das Verhalten Ihres Codes ableiten können,
ohne den Code selbst betrachten zu müssen.
Darüber hinaus können Sie bei fehlgeschlagenen Tests genau erkennen, welche Szenarien nicht Ihren Erwartungen entsprechen.

### Verwende das *AAA*-Muster

Der Code von Unit-Testmethoden sollte leicht lesbar und übersichtlich sein.
Daher schreiben wir Unit-Tests mit dem *AAA*-Muster.
Das *AAA*-Muster ist ein gängiges Muster für Unit-Tests.
Es sorgt für eine klare Trennung zwischen Testobjekt-Setups, Aktionen und Ergebnissen.
Es unterteilt die Unit-Testmethoden in drei Teile: *Arrange*, *Act* und *Assert*.

  * In *Arrange* erstellen und setzen wir die notwendigen Objekte für den Test.
  * In *Act* rufen wir die zu testende Methode auf und ermitteln den tatsächlichen Wert.
  * In *Assert* prüfen wir den erwarteten und den tatsächlichen Wert. Assertions bestimmen, ob der Test fehlschlägt oder erfolgreich ist.
    Stimmen Erwartungswert und tatsächlicher Wert überein, ist der Test erfolgreich.



### Avoid Using Complex Logic


### Mocks und Stubs

Wir wollen die Begrifflichkeiten Mock und Stub an einem Beispiel erläutern:

Stub:

Ein Stub ist ein einfaches Fake-Objekt. Es stellt sicher, dass ein Test reibungslos abläuft.
Stub-Objekt implementieren Methoden mit Anweisungen, die ein bestimmtes Ergebnis zurückgeben.

Beobachtung:
Der Stub gibt immer dieselbe Antwort zurück.

Mock:

Ein Mock-Objekt überprüft, ob ein bestimmtes Verhalten auftritt &ndash; also ob z. B. eine Methode aufgerufen wurde.
Mock-Objekte setzen Erwartungen(d.h. sie treffen Aussagen / Assertions darüber, ob oder wie sie aufgerufen werden.

Beispiel:

Wir haben eine UserService-Klasse, die einen UserRepository benutzt:


---

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---
