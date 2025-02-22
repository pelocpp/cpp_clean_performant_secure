# Klassische Probleme des C/C++ *Secure Programmings*

[Zur�ck](./Readme_Secure_Programming.md)

---

## Inhalt

### Allgemeines

  * [Einige Eigenschaften der Programmiersprache C](#link)
  * [Einige Eigenschaften der Programmiersprache C++](#link)
  * [Was unterscheidet C/C++ von anderen Programmiersprache wie z. B. Java oder C#](#link)
  * [Exploitability und Vulnerability](#link)
  * [Sicherheitsl�cken versus *Undefined Behaviour* (*UB*)](#link)

### Die h�ufigsten *Exploits* und *Vulnerabilities*

  * [Unsigned Integer Wraparound](#link)
  * [Signed Integer Overflow](#link)
  * [Numeric Truncation](#link)
  * [Stack Buffer Overflow](#link)
  * [Heap Buffer Overflow](#link)
  * [Buffer Underflow](#link)
  * [Use after Free](#link)
  * [Double Free](#link)
  * [Incorrect Type Conversion / Type Punning](#link)
  * [Uncontrolled Format String](#link)

---

#### Quellcode

[*SecureProgrammingIssues.cpp*](SecureProgrammingIssues.cpp)<br />

---

### Einige Eigenschaften der Programmiersprache C

  * Passen C und &bdquo;Secure Programming&rdquo; zusammen? Nein!

  * Ist C als sichere, robuste Programmiersprache konzipert: Nein!

  * Ist es m�glich, in C sichere und robuste Programme zu schreiben: Ja!

  * Gibt es ein Motto f�r C Programmierung: Ja: &bdquo;A Programmer knows what he does&rdquo;.

  * Warum ist C so unglaublich erfolgreich: Weil man nahezu jedes erdenkliche Programm in C schreiben kann.

##### Zusammenfassung:

  * How to Shoot Yourself In the Foot using C:<br />*You shoot yourself in the foot*.

---

### Einige Eigenschaften der Programmiersprache C++

  * Passen C++ und &bdquo;Secure Programming&rdquo; zusammen? Durchaus!

  * Ist C++ als sichere, robuste Programmiersprache konzipert: Ja!

  * Ist es m�glich, in C++ sichere und robuste Programme zu schreiben: Ja!

  * Gibt es ein Motto f�r C++ Programmierung: Ja: &bdquo;Never trust a programmer who says he knows C++&rdquo;.

  * Warum ist C++ so unglaublich erfolgreich: Performanz!

##### Zusammenfassung:

  * How to Shoot Yourself In the Foot using C:<br />*You accidentally create a dozen instances of yourself and shoot them all in the foot. Providing emergency medical care is impossible since you can't tell which are bitwise copies and which are just pointing at others and saying &bdquo;that's me, over there.&rdquo;*.

---

### Was unterscheidet C/C++ von anderen Programmiersprache wie z. B. Java oder C#

  * Ein C/C++-�bersetzer generiert Maschinencode, so wie dieser vom Entwickler durch den vom ihm geschriebenen Quellcode angefordert wird,
  *ohne* Sicherheitspr�fungen hinzuzuf�gen.

  * In C# oder Java geschriebenen Programmen w�rde beispielsweise ein falscher Array-Zugriff zu einer Laufzeitausnahme (*Exception*) f�hren,
  w�hrend dies in C++ zu einem falschen Speicherzugriff (Lesen) oder einer Speicherverletzung beim Schreiben f�hrt.

  * Falsches oder unvorsichtiges Programmieren in C/C++ kann zu �berl�ufen (Stack-, Heap- und Puffer�berl�ufen) f�hren,
  die leicht f�r einen Angriff verwendet werden k�nnen.

---

### Exploitability und Vulnerability

##### Was versteht man unter einem *Exploit*?

Ein Exploit zeigt Sicherheitsl�cken von Software auf und erm�glicht deren Ausnutzung.
Exploits stellen ein Werkzeug f�r Hacker dar, um in Computersysteme einzudringen und sie zu manipulieren.
Sie k�nnen auch zur Beseitigung der Schwachstellen genutzt werden.

Der Begriff Exploit kommt vom englischen &bdquo;to exploit&rdquo;, zu Deutsch &bdquo;ausnutzen&rdquo;.
Ein Exploit stellt eine systematische M�glichkeit dar, aufgrund von Schwachstellen oder Sicherheitsl�cken
der Software in Computersysteme &ndash; in unserem Fall: in C/C++ Programme &ndash; einzudringen.


Welche Arten von Exploits gibt es?

Es existiert eine Vielzahl verschiedener Exploits. Abh�ngig von der verwendeten Angriffsart
und zeitlichen Aspekten lassen sich die Exploits unterschiedlich klassifizieren.
Oft genannte Exploit-Arten sind beispielsweise:

  * Remote ausgef�hrte Exploits
  * Lokale ausgef�hrte Exploits
  * Denial-of-Service-Exploits (DoS-Exploits)
  * SQL-Injection-Exploits
  * Command-Execution-Exploits
  * Zero-Day-Exploits


Der ber�hmteste Exploit: *Off-by-one Exploit*

Ein h�ufiger Programmierfehler beim Berechnen von Array-Grenzen.
In Little-Endian-Architekturen kann dies zum �berschreiben des niederwertigsten Bytes f�hren.

##### Was versteht man unter *Vulnerability*?

Unter dem Begriff *Vulnerability* versteht man Schwachstellen,
m�glicherweise etwas drastischer �bersetzt *Hochrisiko-Schwachstellen*.

Wie kommt es dazu?

C/C++ sind so genannte *Low-Level*-Programmiersprachen.

Das Speichermanagement ist also anders geregelt als bei High-Level-Sprachen wie JavaScript oder Python.
Bei diesen wird die Zuweisung, Nutzung und Bereitstellung implizit verwaltet (zum Beispiel mit *Garbage Collection*)

Bei C/C++ muss dies entwicklerseitig geschehen. 
Performance und Implementierung lassen sich so zwar �u�erst feinstufig optimieren, zugleich ist dieses Programmierkonzept aber auch mit sehr spezifischen Problematiken verbunden.

*Buffer-Overflows* (CWE-121) und *Out-of-Bounds Write* (CWE-787) sind einige der bekanntesten
*Vulnerabilities* in C/C++.

---

### Sicherheitsl�cken versus *Undefined Behaviour* (*UB*)

In C/C++ gibt es sowohl Sicherheitsl�cken als auch das sogenannte *Undefined Behaviour* (kurz: *UB*).

Beide Begriffe liegen eng beieinander.

##### Beispiel f�r eine Sicherheitsl�cke: &bdquo;Use After Free&rdquo;

Es gibt den Datentyp Pointer in C/C++. Mit der Funktion `malloc` / dem Operator `new`
kann man Speicher reservieren, um ihn anschlie�end verwenden zu k�nnen.

Der Name &bdquo;Use After Free&rdquo; verr�t eigentlich schon alles. Hierbei geht es darum, dass ein Zeiger auf eine Variable
nach seiner Freigabe weiterhin genutzt wird.

Es ist dem Compiler nicht m�glich, zur �bersetzungszeit zu �berpr�fen,
ob Variablen, die nur via Zeiger erreichbar sind (`malloc`, `new`), noch verf�gbar sind (`free`, `delete`).
Dies ist eine Sicherheitsl�cke in der Definition der Programmiersprache.

Andere Programmiersprachen gehen hier einen anderen Weg wie z. B. C# oder Java.
Hier wird vormals reservierter Speicher,
der im Programm keine Verwendung mehr findet, durch die Laufzeitumgtebung wieder freigegeben (*Garbage Collection*)

Die damit verbundene Anf�lligkeit (Sicherheitsl�cke) h�ngt mit einem Fehler im Zusammenhang
mit dem Speichermanagement im Ablauf des Programms zusammen:

Aus der Nutzung der Variable nach ihrer Freigabe resultieren im Programm
unerwartete Aktionen oder andere unerw�nschte Effekte. Dies bezeichnet man als *Undefined Behaviour*.
H�ufig macht man die Beobachtung, dass *Undefined Behaviour* zu einem Absturz des Programms f�hrt.

---

## Die h�ufigsten *Exploits* und *Vulnerabilities*

### &bdquo;*Unsigned Integer Wraparound*&rdquo;

*Beschreibung*:

[CWE-190: Integer Overflow or Wraparound](https://cwe.mitre.org/data/definitions/190.html)

Das Produkt f�hrt eine Berechnung durch, die einen Integer-�berlauf oder Wraparound erzeugen kann.

Dies tritt auf, wenn ein Integer-Wert auf einen Wert erh�ht wird, der zu gro� ist,
um in der zugeh�rigen Darstellung gespeichert zu werden.

In diesem Fall kann der Wert eine sehr kleine oder negative Zahl werden.



### &bdquo;*Signed Integer Overflow*&rdquo;

*Beschreibung*:

[CWE-190: Integer Overflow or Wraparound](https://cwe.mitre.org/data/definitions/190.html)

Siehe &bdquo;*Unsigned Integer Wraparound*&rdquo;.

### &bdquo;*Numeric Truncation*&rdquo;

*Beschreibung*:

[CWE-197: Numeric Truncation Error](https://cwe.mitre.org/data/definitions/197.html)

  * Implementation Defined Behaviour (IDB)
  * Integer Conversions do result in lost or misinterpreted data

Wenn Werte eines elementaren Datentyps in einen Wert eines kleineren elementaren Datentyps umgewandelt werden,
gehen die h�herwertigen Bits des gr��eren Werts bei der Konvertierung verloren,
was m�glicherweise zu einem unerwarteten Wert f�hrt, der nicht dem urspr�nglichen Wert entspricht.

Dieser Wert kann als Index in einem Puffer, als Schleifenindex oder einfach als normale Variable erforderlich sein.

In jedem Fall ist der Wert nicht vertrauensw�rdig und das System befindet sich in einem undefinierten Zustand.



### &bdquo;*Stack Buffer Overflow*&rdquo;

*Beschreibung*:

[CWE-121: Stack-based Buffer Overflow](https://cwe.mitre.org/data/definitions/121.html)

&bdquo;*Stack Overflow*&bdquo;, auch als &bdquo;*Stack Exhaustion*&bdquo; bezeichnet, 
ist normalerweise das Ergebnis von �berm��ig vielen geschachtelten Funktionsaufrufen,
bisweilen auch rekursiven Funktionsaufrufen.

Ebenfalls kritisch f�r den Stapel sind zu viele gro�e Datenmengen,
die in Funktionen als &bdquo;*Stack*&bdquo;-Variablen angelegt werden.


*Beschreibung*:

[CWE-242: Use of Inherently Dangerous Function](https://cwe.mitre.org/data/definitions/242.html)

Das Produkt ruft eine Funktion auf, deren sichere Funktion nie garantiert werden kann.


*Beschreibung*:

[CWE-787: Out-of-bounds Write](https://cwe.mitre.org/data/definitions/787.html)

Das Produkt schreibt Daten �ber das Ende oder vor den Anfang des vorgesehenen Puffers hinaus.

*Beispiel*:

```cpp
01: void test() {
02: 
03:     const int Size = 32;
04:     char password[Size];
05:     // gets(password); // <- Write outside  // Deprecated in C++ 11 // Removed in C++ 14
06: }
07: 
08: void test() {
09: 
10:     const int Size = 32;
11:     char password[Size];
12:     std::println("Please enter Password: ");
13:     fgets(password, Size, stdin);
14: 
15:     auto result = strncmp(password, "12345", 5);
16:     if (result == 0) {
17:         std::println("Login successful!");
18:     }
19: }
```


### &bdquo;*Heap Buffer Overflow*&rdquo;

*Beschreibung*:

[CWE-122: Heap-based Buffer Overflow](https://cwe.mitre.org/data/definitions/122.html)

Bei einem Heap-�berlauf handelt es sich um einen Puffer�berlauf, bei dem der Puffer, der �berschrieben werden kann,
im Heap-Bereich des Speichers zugewiesen wird.

Dies bedeutet im Allgemeinen, dass der Puffer mithilfe einer Routine wie `malloc()` / `new` zugewiesen wurde.



### &bdquo;*Buffer Underflow*&rdquo;

*Beschreibung*:

[CWE-124: Buffer Underwrite ('Buffer Underflow')](https://cwe.mitre.org/data/definitions/124.html)

Das Problem schreibt in einen Puffer mithilfe eines Index oder Zeigers,
der auf einen Speicherort vor dem Anfang des Puffers verweist.


### &bdquo;*Use after Free*&rdquo;

*Beschreibung*:

[CWE-416: Use After Free](https://cwe.mitre.org/data/definitions/416.html)

Das Problem verwendet den Speicher erneut oder verweist auf ihn, nachdem er freigegeben wurde.
Irgendwann danach kann der Speicher erneut zugewiesen und in einem anderen Zeiger gespeichert werden,
w�hrend der urspr�ngliche Zeiger auf einen Speicherort irgendwo innerhalb der neuen Zuweisung verweist.

Alle Vorg�nge, die den urspr�nglichen Zeiger verwenden, sind nicht mehr g�ltig, da der Speicher dem Code &bdquo;geh�rt&rdquo;,
der mit dem neuen Zeiger arbeitet.


### &bdquo;*Double Free*&rdquo;

*Beschreibung*:

[CWE-415: Double Free](https://cwe.mitre.org/data/definitions/415.html)


Das Problem ruft `free()` / `delete` zweimal f�r dieselbe Speicheradresse auf,
was m�glicherweise zur �nderung unerwarteter Speicherorte f�hrt.


### &bdquo;*Incorrect Type Conversion*&rdquo; / &bdquo;*Type Punning*&rdquo;

*Beschreibung*:

[CWE-704: Incorrect Type Conversion or Cast](https://cwe.mitre.org/data/definitions/704.html)

Das Problem konvertiert ein Objekt, eine Ressource oder eine Struktur nicht ordnungsgem�� von einem Typ in einen anderen Typ.

Was bezeichnet man als *Type Punning*?

&bdquo;Eine Form des Zeigeraliasings, bei der zwei Zeiger auf dieselbe Stelle im Speicher verweisen,
diese Stelle aber aus der Sichtweise unterschiedlicher Datentypen betrachten.<br />
Der Compiler behandelt beide &bdquo;Punnings&rdquo; als nicht verwandte Zeiger.

*Type Punnings* k�nnen Abh�ngigkeitsprobleme f�r alle Daten verursachen, auf die �ber beide Zeiger zugegriffen wird.&rdquo;

### &bdquo;*Uncontrolled Format String*&rdquo;

*Beschreibung*:

[CWE-134: Use of Externally-Controlled Format String](https://cwe.mitre.org/data/definitions/134.html)

Das Problem verwendet eine Funktion,
die eine Formatzeichenfolge als Argument akzeptiert, die Formatzeichenfolge stammt jedoch aus einer externen Quelle.

---

[Zur�ck](./Readme_Secure_Programming.md)

---
