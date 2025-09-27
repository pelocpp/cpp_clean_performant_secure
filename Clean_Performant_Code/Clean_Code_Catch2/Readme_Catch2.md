# Catch2

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

In diesem Repository findet man einfache, einf�hrende Beispiele
zum &bdquo;*Catch2 Unit Testing Framework*&rdquo; vor.

Weitere Informationen findet man [hier](https://github.com/catchorg/Catch2).

  * [Einige Hinweise zu *Catch2*](#link1)
  * [Einige Best Practices zum Unit Testing](#link2)
  

---

## Einige Hinweise zu *Catch2* <a name="link1"></a>

#### *Catch2* kann �ber die Kommandozeile gesteuert werden

<img src="Command_Line_Arguments.png" width="500">

*Abbildung* 1: Visual Studio und Kommandozeilen Argumente.

#### `TEST_CASE` und `SECTION`

  * **TEST_CASE** ( *test_name* [, *tags* ] )

  * **SECTION** ( *section_name*, [, *section_description* ] )

Die *section_description* kann verwendet werden, um eine ausf�hrliche Beschreibung eines Abschnitts bereitzustellen,
w�hrend *section_name* f�r die Verwendung mit der Kommandozeile vorgesehen ist.


#### `REQUIRE` und `CHECK`

  * **REQUIRE** ( *expression* ) und
  * **CHECK** ( *expression* )

`REQUIRE` testet einen Ausdruck und bricht den Testfall ab, wenn dieser fehlschl�gt.

`CHECK` ist gleichwertig zu `REQUIRE`, die Ausf�hrung wird jedoch im selben Testfall fortgesetzt, auch wenn die Assertion fehlschl�gt.


#### Testf�lle im BDD-Stil (*Behaviour Driven Development*)

Zus�tzlich zu Catch's Schreibweise des klassischen Testfallstils
unterst�tzt Catch eine alternative Syntax, die es erm�glicht,
Tests als &bdquo;ausf�hrbare Spezifikationen&rdquo; zu schreiben,
quasi im Sinne des *Behaviour Driven Development*.


  * **SCENARIO** ( *scenario_name* [, *tags* ] )

Dieses Makro wird auf `TEST_CASE` abgebildet und funktioniert auf die gleiche Weise,
au�er dass dem Testfallnamen das Pr�fix SCENARIO vorangestellt wird.

  * **GIVEN** ( *something* )
  * **WHEN** ( *something* )
  * **THEN** ( *something* )


#### Typparametrisierte Testf�lle


Zus�tzlich zu `TEST_CASE` unterst�tzt Catch2 auch nach Typen parametrisierte Testf�lle
in Form von `TEMPLATE_TEST_CASE`:


  * **TEMPLATE_TEST_CASE** ( *test_name* , *tags*, *type_1*, *type_2*, ... , *type_n* )



#### *Matchers*

Matcher sind eine alternative M�glichkeit, Assertions zu schreiben,
n�tzlich f�r Tests, bei denen Sie mit komplexen Typen arbeiten oder komplexere Eigenschaften �berpr�fen m�ssen:

Matcher werden am h�ufigsten zusammen mit den Makros `REQUIRE_THAT` oder CHECK_THAT verwendet.

---

## Einige Best Practices zum Unit Testing <a name="link2"></a>

### Beachte die Namensgebung f�r Unit Tests 

Der Name eines Unit Tests sollte aus drei Teilen bestehen:

  * Name der zu testenden Methode / Funktion
  * Szenario, in dem die Methode getestet wird
  * Erwartetes Verhalten beim Aufruf des Szenarios

Benennungsstandards sind wichtig, da sie den Zweck und die Anwendung des Tests verdeutlichen.
Tests stellen nicht nur sicher, dass Ihr Code funktioniert.
Sie dienen auch der Dokumentation.
Allein durch die Betrachtung der Unit-Tests sollten Sie das Verhalten Ihres Codes ableiten k�nnen,
ohne den Code selbst betrachten zu m�ssen.
Dar�ber hinaus k�nnen Sie bei fehlgeschlagenen Tests genau erkennen, welche Szenarien nicht Ihren Erwartungen entsprechen.

### Verwende das *AAA*-Muster

Der Code von Unit-Testmethoden sollte leicht lesbar und �bersichtlich sein.
Daher schreiben wir Unit-Tests mit dem *AAA*-Muster.
Das *AAA*-Muster ist ein g�ngiges Muster f�r Unit-Tests.
Es sorgt f�r eine klare Trennung zwischen Testobjekt-Setups, Aktionen und Ergebnissen.
Es unterteilt die Unit-Testmethoden in drei Teile: *Arrange*, *Act* und *Assert*.

  * In *Arrange* erstellen und setzen wir die notwendigen Objekte f�r den Test.
  * In *Act* rufen wir die zu testende Methode auf und ermitteln den tats�chlichen Wert.
  * In *Assert* pr�fen wir den erwarteten und den tats�chlichen Wert. Assertions bestimmen, ob der Test fehlschl�gt oder erfolgreich ist.
    Stimmen Erwartungswert und tats�chlicher Wert �berein, ist der Test erfolgreich.



### Avoid Using Complex Logic


### Mocks und Stubs

Wir wollen die Begrifflichkeiten Mock und Stub an einem Beispiel erl�utern:

Stub:

Ein Stub ist ein einfaches Fake-Objekt. Es stellt sicher, dass ein Test reibungslos abl�uft.
Stub-Objekt implementieren Methoden mit Anweisungen, die ein bestimmtes Ergebnis zur�ckgeben.

Beobachtung:
Der Stub gibt immer dieselbe Antwort zur�ck.

Mock:

Ein Mock-Objekt �berpr�ft, ob ein bestimmtes Verhalten auftritt &ndash; also ob z. B. eine Methode aufgerufen wurde.
Mock-Objekte setzen Erwartungen(d.h. sie treffen Aussagen / Assertions dar�ber, ob oder wie sie aufgerufen werden.

Beispiel:

Wir haben eine UserService-Klasse, die einen UserRepository benutzt:


---

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---
