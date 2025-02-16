# *SafeInt*: Eine Bibliothek zum Schutz vor Integer-Überläufen


[Zurück](../../Markdown/Readme_Secure_Programming.md)

---

## Inhalt

  * [Überlauf ganzzahliger Variablen](#link1)
  * [Literatur](#link7)

---

#### Quellcode

[*SafeInt.cpp*](SafeInt.cpp)<br />

---

## Überlauf ganzzahliger Variablen <a name="link1"></a>

Der Wertebereich von ganzzahligen Variablen ist nicht unendlich.
Was die Sache noch ein klein wenig in C++ erschwert, ist der Umstand,
dass C++ Programme auf unterschiedlichen Rechnersystemen mit unterschiedlicher Hardware ablaufen können.
Oder etwas anders formuliert: Der Wertebereich einer `int`-Variablen
ist leider von Rechner zu Rechner unterschiedlich!

Man hat es in C/C++ versäumt, den Datentyp `int` rechnerunabhängig zu definieren.
Okay, wir sollten an dieser Stelle hinzufügen, das dies nie das Ziel in der Konzeption
von C/C++ gewesen ist, sondern ganz im Gegenteil: Auf vielen Rechnersystem sollte &ndash; historisch betrachtet &ndash; 
die Bitbreite einer  `int`-Variablen mit der Bitbreite des Akkumulators  zusammenpassen.
Der Akkumulator ist das zentale Register innerhalb einer CPU,
in dem Ergebnisse der Recheneinheit gespeichert werden.

In Sprachen wir C# oder Java hat man hier einen anderen Weg eingeschlagen:
`int`-Variablen in einem C#-Programm nehmen in jedem Programm 4 Byte in Abspruch,
egal, auf welchem Zielsystem das Programm ausgeführt wird.

Zur Wahrheit gehört auch, dass es in C# den `int`-Datentyp so nicht wirklich gibt:
Der Bezeichner  `int` wird auf den Strukturtyp `System.Int32` abgebildet, 
und dieser definiert eben eine 32-Bit-Ganzzahl mit Vorzeichen

Zurück zu C++:

Betrachten Sie folgendes Beispielprogramm:

```cpp
01: int16_t sum(const std::vector<int16_t>& values)
02: {
03:     int16_t result{};
04: 
05:     for (auto num : values)
06:     {
07:         result += num;
08:     }
09: 
10:     return result;
11: }
12: 
13: void test()
14: {
15:     std::vector<int16_t> vec{ 30'000, 2'000, 700, 60, 7, 0 };
16:     auto result{ sum(vec) };
17:     std::println("Sum: {}", result);
18: }
```

*Ausgabe*:

```
Sum: 32767
```

Ändern wir den letzen Wert des Vektors `vec` in Zeile 15 in 1 ab,
so hingegen erhalten wir das Ergebnis

```
Sum: -32768
```

Hmm, dies ist offentlich falsch, wie haben es mit einem Überlauf in der Arithmetik
einer 16-Bit-Ganzzahl mit Vorzeichen zu tun.

Natürlich könnten wir dieses Problem lösen, indem wir von 16-Bit-Ganzzahlen zu 32-Bit-Ganzzahlen
wechseln. Aber auch hier könnten wir ein ähnlich gelagertes Problem generieren.

Welche Lösungen bieten sich an:

  * Möchte man die standardmäßig in C/C++ eingebauten arithmetischen Operationen nicht verlassen,
   dann gibt es nur eine Vorgehensweise: Man muss die Wertebereiche überprügen.
  * Big Integer Arithmetik

---

## Literatur <a name="link6"></a>

Eine gute Beschreibung der Begriffe *Kohäsion* und *Kopplung*  
wird in [Cohesion and Coupling: the difference](https://enterprisecraftsmanship.com/posts/cohesion-coupling-difference/) gegeben.

---

[Zurück](../../Markdown/Readme_Secure_Programming.md)

---
