# Catch2

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

In diesem Repository findet man einfache, einf�hrende Beispiele
zum &bdquo;*Catch2 Unit Testing Framework*&rdquo; vor.

Weitere Informationen findet man [hier](https://github.com/catchorg/Catch2).

---

## Einige Hinweise

#### *Catch2* kann �ber die Kommandozeile gesteuert werden

<img src="Command_Line_Arguments.png" width="500">

*Abbildung* 1: Visual Studio und Kommandozeilen Argumente.

#### `TEST_CASE` und `SECTION`

  * **TEST_CASE** ( *test_name* [, tags ] )

  * **SECTION** ( *section_name*, [, *section_description* ] )

Die *section_description* kann verwendet werden, um eine ausf�hrliche Beschreibung eines Abschnitts bereitzustellen,
w�hrend *section_name* f�r die Verwendung mit der Kommandozeile vorgesehen ist.


#### `REQUIRE` und `CHECK`

  * **REQUIRE** ( *expression* ) und
  * **CHECK** ( *expression* )

`REQUIRE` testet einen Ausdruck und bricht den Testfall ab, wenn dieser fehlschl�gt.

`CHECK` sind gleichwertig zu **REQUIRE**, die Ausf�hrung wird jedoch im selben Testfall fortgesetzt, auch wenn die Assertion fehlschl�gt.


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

[Zur�ck](../Clean_Code/Readme_Clean_Code.md)

---
