# Einige Hinweise zu *Catch2*

[Zur�ck](./Readme_Catch2.md)

---

## Inhalt

  * [Dokumentation zu *Catch2*](#link1)
  * [*Catch2* kann �ber die Kommandozeile gesteuert werden](#link2)
  * [`TEST_CASE` und `SECTION`](#link3)
  * [`REQUIRE` und `CHECK`](#link4)
  * [Testf�lle im BDD-Stil (*Behaviour Driven Development*)](#link5)
  * [Typparametrisierte Testf�lle](#link6)
  * [*Test Fixtures*](#link7)
  * [*Matchers*](#link8)

---

#### Quellcode

[*Catch_Demo_Getting_Started.cpp*](Catch_Demo_Getting_Started.cpp)<br />

---

## Dokumentation zu *Catch2* <a name="link1"></a>

Informationenen zu *Catch2* findet man [hier](https://github.com/catchorg/Catch2).


## *Catch2* kann �ber die Kommandozeile gesteuert werden <a name="link2"></a>

<img src="Command_Line_Arguments.png" width="500">

*Abbildung* 1: Visual Studio und Kommandozeilen Argumente.

## `TEST_CASE` und `SECTION` <a name="link3"></a>

  * **TEST_CASE** ( *test_name* [, *tags* ] )

  * **SECTION** ( *section_name*, [, *section_description* ] )

Die *section_description* kann verwendet werden, um eine ausf�hrliche Beschreibung eines Abschnitts bereitzustellen,
w�hrend *section_name* f�r die Verwendung mit der Kommandozeile vorgesehen ist.


## `REQUIRE` und `CHECK` <a name="link4"></a>

  * **REQUIRE** ( *expression* ) und
  * **CHECK** ( *expression* )

`REQUIRE` testet einen Ausdruck und bricht den Testfall ab, wenn dieser fehlschl�gt.

`CHECK` ist gleichwertig zu `REQUIRE`, die Ausf�hrung wird jedoch im selben Testfall fortgesetzt, auch wenn die Assertion fehlschl�gt.


## Testf�lle im BDD-Stil (*Behaviour Driven Development*) <a name="link5"></a>

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


## Typparametrisierte Testf�lle <a name="link6"></a>


Zus�tzlich zu `TEST_CASE` unterst�tzt Catch2 auch nach Typen parametrisierte Testf�lle
in Form von `TEMPLATE_TEST_CASE`:


  * **TEMPLATE_TEST_CASE** ( *test_name* , *tags*, *type_1*, *type_2*, ... , *type_n* )


## *Test Fixtures* <a name="link7"></a>

Im Kontext von Software wird ein Test-Fixture (auch &bdquo;Testkontext&rdquo; genannt) verwendet,
um den Systemzustand und die Eingabedaten einzurichten, die f�r die Testausf�hrung erforderlich sind.

F�r wiederverwendbares Setup/Teardown bieten sich du eine Fixture-Klasse definieren.
Konstruktor = Setup, Destruktor = Teardown


  * **TEST_CASE_METHOD**(  *class_name* , *unique_test_name_name*, *tags* )
  

## *Matchers* <a name="link8"></a>

Matcher sind eine alternative M�glichkeit, Assertions zu schreiben,
n�tzlich f�r Tests, bei denen Sie mit komplexen Typen arbeiten oder komplexere Eigenschaften �berpr�fen m�ssen:

Matcher werden am h�ufigsten zusammen mit den Makros `REQUIRE_THAT` oder CHECK_THAT verwendet.

---

[Zur�ck](./Readme_Catch2.md)

---
