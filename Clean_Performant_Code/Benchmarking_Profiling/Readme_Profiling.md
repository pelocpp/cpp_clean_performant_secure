# Profiling

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Visual Studio Diagnostic Tools](#link2)
  * [Ein Beispiel: &bdquo;*Plutonian Pebbles*&rdquo;](#link3)

---

#### Quellcode

[*Advent_of_Code_2024_No_11.cpp*](Advent_of_Code_2024_No_11.cpp)<br />

---

## Allgemeines <a name="link1"></a>

Aus dem Buch &bdquo;[*Professional C++*](https://www.amazon.de/Professional-C-Marc-Gregoire/dp/1394193173)&rdquo; von Marc Gregoire zitiert:

&bdquo;Es ist ratsam, beim Entwerfen und Programmieren an die Effizienz zu denken.
Es macht keinen Sinn, offensichtlich ineffiziente Programme zu schreiben, wenn dies mit gesundem Menschenverstand oder erfahrungsbasierter Intuition vermieden werden kann.

Ich rate Ihnen jedoch dringend, sich in der Design- und Programmierphase nicht zu sehr auf die Leistung zu konzentrieren.

Es empfiehlt sich, zun�chst ein sauberes, gut strukturiertes Design und eine gut strukturierte Implementierung zu erstellen,
dann einen Profiler zu verwenden und nur die Teile zu optimieren, die vom Profiler als Leistungsengp�sse gekennzeichnet werden.

Denken Sie an die in dem Buch
&bdquo;[*Computer Architecture: A Quantitative Approach*](https://www.amazon.de/Computer-Architecture-Quantitative-Approach-Kaufmann/dp/0443154066)&rdquo;
von John L. Hennessy, David A. Patterson, Christos Kozyrakis
eingef�hrte &bdquo;90/10&rdquo;-Regel, die besagt, dass 90 Prozent der Laufzeit der meisten Programme
auf nur 10 Prozent des Codes entfallen.

Das bedeutet, dass Sie 90 Prozent Ihres Codes optimieren k�nnten, die Laufzeit des Programms aber dennoch nur um 10 Prozent verbessern w�rden.

Nat�rlich m�chten Sie die am h�ufigsten beanspruchten Codeteile f�r die spezifische Arbeitslast optimieren, die das Programm voraussichtlich ausf�hren wird.&rdquo;

Und weiter:

&bdquo;Daher ist es oft hilfreich, ein Programm zu &bdquo;*profilen*&rdquo;, um festzustellen, welche Codeteile optimiert werden m�ssen.

Es gibt viele Profiling-Tools, die Programme w�hrend der Ausf�hrung analysieren und Daten zur Leistung generieren.

Die meisten Profiling-Tools erm�glichen Analysen auf Funktionsebene, indem sie die f�r jede Funktion im Programm ben�tigte Zeit (oder den Prozentsatz der gesamten Ausf�hrungszeit) angeben.

Nachdem Sie ein Profiling f�r Ihr Programm ausgef�hrt haben, k�nnen Sie in der Regel sofort erkennen, welche Programmteile optimiert werden m�ssen.

Profiling vor und nach der Optimierung ist unerl�sslich, um den Erfolg Ihrer Optimierungen nachzuweisen.&rdquo;

---

## Visual Studio Diagnostic Tools <a name="link2"></a>

Abh�ngig von Ihrer Version von VC++ stehen Ihnen verschiedene Analysetools zur Verf�gung.
Wir geben in den folgenden drei Abbildungen einen �berblick �ber deren Funktionalit�t:

In *Abbildung* 1 finden Sie eine Darstellung des *Hot Path*:

<img src="Profiling_01.png" width="600">

*Abbildung* 1: Darstellung des *Hot Path*.


Die Drilldown-Ansicht in *Abbildung* 2 (*Call-Tree*) zeigt die Ausf�hrungszeiten der einzelnen Funktions-/Methodenaufrufe
der kritischsten Funktion/Methode des *Hot Path*.

<img src="Profiling_02.png" width="800">

*Abbildung* 2: Der *Call-Tree* mit Ausf�hrungszeiten: *Total CPU* und *Self CPU*.

Die Codeansicht in *Abbildung* 3 zeigt den prozentualen Anteil der Ausf�hrungszeit, den eine Codezeile ben�tigt.

Die rechenintensivsten Zeilen werden rot dargestellt. Diese Ansicht macht deutlich, dass das
Einlesen des linken Teils (`leftpart`) in etwa halb so lange dauert
wie das des rechten Teils (`rightPart`).

<img src="Profiling_03.png" width="500">

*Abbildung* 3: Quelltext mit Hervorhebung der rechenintensivsten Zeilen.

---

## Ein Beispiel: &bdquo;*Plutonian Pebbles*&rdquo; <a name="link3"></a>

Wir ben�tigen ein geeignetes Beispiel, um Profiling Tools anzuwenden.

Beim &bdquo;[Advent of Code](https://adventofcode.com/2024)&rdquo; handelt es sich ein ein j�hrliches Event im Dezember, bei dem es darum geht, kleinere bis gr��ere Programmier-Aufgaben
(&bdquo;*Puzzles*&rdquo;) zu l�sen. Definitiv sind diese Aufgaben nicht einfach, stellen eine Herausforderung f�r jeden Entwickler dar
und eignen sich sehr gut, um m�glicherweise zu laufzeitintensive L�sungen mit einem Blick hinter die Kulissen zu tunen.

<img src="AdventOfCode.png" width="600">

*Abbildung* 4: Startseite des &bdquo;[Advent of Code 2024](https://adventofcode.com/2024)&rdquo;.

Genaue Beobachter und Kenner des Wettbewerbs k�nnen in *Abbildung* 4 erkennen,
dass ich bis zur 12. Aufgabe mithalten konnte. Okay, Fairness muss sein:
Es hat mir an der Zeit gemangelt, um mich den restlichen Puzzles zuzuwenden.
Vielleicht sieht es in diesem Jahr besser aus :)

Im Quellcode zu diesem Abschnitt habe ich eine &bdquo;*quick-and-dirty*&rdquo; Realisierung des so genannten &bdquo;*Plutonian Pebbles*&rdquo;&ndash;Puzzles beigef�gt.
Um es vorweg zu sagen: Diese L�sung ist weit davon entfernt, gut zu sein.
Deshalb soll sie mit den *Visual Studio Profiling Tools* analysiert werden:

  * Finden Sie den *Hot Path* der vorliegenden L�sung?
  * Wie k�nnte ein Redesign des gefundenen Engpasses aussehen?

---

[Zur�ck](../Performance_Optimization/Readme_Performance_Optimization.md)

---
