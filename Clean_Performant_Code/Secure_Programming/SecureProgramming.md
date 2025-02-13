# Secure Programming

[Zur�ck](../../Markdown/Readme_Performance_Optimization.md)

---

Incorrect type conversion


## Inhalt

  * [Puffer �ber-/Unterlauf (*Buffer Overflows/Underflows*)](#link)
  * [Speicherlecks (*Memory Leaks*)](#link)
  * [Race Conditions](#link)
  * [Arithmetischer �ber-/Unterlauf (*Integer overflow/underflow*)](#link)
  * [Injection attacks](#link)
  * [Sicherer Umgang mit Pointern](#link)
  * [Input validation and sanitization](#link)
  * [Memory management](#link)
  * [Miscellaneous Secure coding techniques](#link)
  * [Error handling and logging](#link)
  * [Nutzung sicherer Bibliotheken und Funktionen](#link)
  * [Incorrect type conversion / Type Punning](#link)
  * [Type Punning](#link)
  * [XXX](#link)
  * [XXX](#link)
  * [Literatur](#link6)

  Tools / Code-Analyse und -Bewertung
  
  * [Clang-Tidy](#link)
  * [Cppcheck](#link)


  Coding Standards:

  CERT C++ Secure Coding Standard
  MISRA C++
  C++ Core Guidelines

---

## *Type Punning*

Was bezeichnet man als *Type Punning*?

&bdquo;Eine Form des Zeigeraliasings, bei der zwei Zeiger auf dieselbe Stelle im Speicher verweisen,
diese Stelle aber aus der Sichtweise unterschiedlicher Datentypen betrachten.<br />
Der Compiler behandelt beide &bdquo;Punnings&rdquo; als nicht verwandte Zeiger.

*Type Punnings* k�nnen Abh�ngigkeitsprobleme f�r alle Daten verursachen, auf die �ber beide Zeiger zugegriffen wird.&rdquo;




---

#### Quellcode

[*BestPractices.cpp*](BestPractices.cpp)<br />

---

## Tool *Cppcheck*

*Cppcheck* ist ein statisches Analysetool f�r C/C++-Quellcode.
Es bietet eine einzigartige Codeanalyse zum Erkennen von Fehlern und konzentriert sich auf das Erkennen undefinierten Verhaltens
und gef�hrlicher Codekonstrukte.

Das Ziel besteht darin, m�glichst wenige Fehlalarme zu haben.
Cppcheck ist so konzipiert, dass es C/C++-Code analysieren kann,
auch wenn er eine nicht standardm��ige Syntax aufweist (was h�ufig bei Embedded C++ Projekten der Fall ist).

Mehrere M�glichkeiten zum Download finden sich [hier](https://cppcheck.sourceforge.io).

<img src="CppCheck_01.png" width="600">

*Abbildung* 1: [https://cppcheck.sourceforge.io](https://cppcheck.sourceforge.io) Website.

*Cppcheck* ist sowohl als Open Source ([hier](https://cppcheck.sourceforge.io) als auch als *Cppcheck Premium Paket*
mit erweiterter Funktionalit�t verf�gbar.
Weitere Informationen und Kaufoptionen f�r die kommerzielle Version finden Sie unter [www.cppcheck.com](www.cppcheck.com).


---

## Visual Studio Integration Add-In f�r Cppcheck

Um *Cppcheck* mit Visual Studio zusammen betreiben zu k�nnen,
grift man am besten auf
das [Visual Studio Integration Add-In](https://github.com/VioletGiraffe/cppcheck-vs-addin) f�r Cppcheck zur�ck.

<img src="CppCheck_Extension_01.png" width="700">

*Abbildung* 2: [https://github.com/VioletGiraffe/cppcheck-vs-addin](https://github.com/VioletGiraffe/cppcheck-vs-addin) Website.

*Hinweis*:

Das Add-In stellt keine Installation des *Cppcheck*-Tools -bereit.
Bevor man dieses Add-In installiert, muss eine *Cppcheck*-Installation erfolgt sein.

Das Add-In f�gt an diversen Stellen im Visual Studio neue Men�eintr�ge hinzu,
um das aktuelle Projekt �berpr�fen zu k�nnen:

<img src="CppCheck_Extension_03.png" width="300">

*Abbildung* 3: Erweiterungen im Men� &bdquo;*Tools*&rdquo;

Zum Abschluss finden Sie in *Abbildung* 4 das *Settings*-Dialogfenster vor:

<img src="CppCheck_Extension_04.png" width="600">

*Abbildung* 4: Spezifische Parametrierung des *Cppcheck*-Tools.

In diesem Fenster kann man spezfische Einstellungen vornehmen,
welche Meldungen man haben m�chte &ndash; und welche vielleicht auch nicht.

---

## Literatur <a name="link6"></a>

---

[Zur�ck](../../Markdown/Readme_Performance_Optimization.md)

Die Anregungen zum konzeptionellen Beispiel finden Sie unter

[C++ Performance Optimization: Best Practices](https://hackernoon.com/c-performance-optimization-best-practices)

---
