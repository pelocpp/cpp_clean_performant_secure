# Klassische Probleme des C/C++ *Secure Programmings*

[Zur�ck](./Readme_Secure_Programming.md)

---

## Inhalt

### Allgemeines

  * [Einige Eigenschaften der Programmiersprache C](#link1)
  * [Einige Eigenschaften der Programmiersprache C++](#link2)
  * [Was unterscheidet C/C++ von anderen Programmiersprache wie z. B. Java, C# oder Python](#link3)
  * [Exploitability und Vulnerability](#link4)
  * [Sicherheitsl�cken versus *Undefined Behaviour* (*UB*)](#link5)

### Die h�ufigsten *Exploits* und *Vulnerabilities*

  * [Unsigned Integer Wraparound](#link6)
  * [Signed Integer Overflow](#link7)
  * [Numeric Truncation](#link8)
  * [Stack Buffer Overflow](#link9)
  * [Heap Buffer Overflow](#link10)
  * [Buffer Underflow](#link11)
  * [Use after Free](#link12)
  * [Double Free](#link13)
  * [Incorrect Type Conversion / Type Punning](#link14)
  * [Uncontrolled Format String](#link15)

---

#### Quellcode

[*SecureProgrammingIssues.cpp*](SecureProgrammingIssues.cpp)<br />

---

## Allgemeines

### Einige Eigenschaften der Programmiersprache C <a name="link1"></a>

  * Passen C und &bdquo;Secure Programming&rdquo; zusammen? Nein!

  * Ist C als sichere, robuste Programmiersprache konzipiert? Nein!

  * Ist es m�glich, in C sichere und robuste Programme zu schreiben? Ja!

  * Gibt es ein Motto f�r die Programmierung in C? Ja: &bdquo;*A Programmer knows what he does*&rdquo;.

  * Warum ist C so unglaublich erfolgreich? Weil man nahezu jedes erdenkliche Programm in C schreiben kann.

#### Zusammenfassung:

  * How to shoot yourself in the foot using C: *You shoot yourself in the foot*.

---

### Einige Eigenschaften der Programmiersprache C++ <a name="link2"></a>

  * Passen C++ und &bdquo;Secure Programming&rdquo; zusammen? Durchaus!

  * Ist C++ als sichere, robuste Programmiersprache konzipiert? Ja!

  * Ist es m�glich, in C++ sichere und robuste Programme zu schreiben? Ja!

  * Ist C++ eine Sprache f�r Einsteiger oder Experten? F�r Experten! Sie gibt Ihnen die volle Kontrolle �ber Ihren Rechner, die Sie sich w�nschen, aber mit der Macht geht auch Verantwortung einher!

  * Gibt es ein Motto f�r die Programmierung in C++? Ja: &bdquo;*Never trust a programmer who says he knows C++*&rdquo;.

  * Warum ist C++ so unglaublich erfolgreich? Performanz!

#### Zusammenfassung:

  * How to shoot yourself in the foot using C++:<br />*You accidentally create a dozen instances of yourself and shoot them all in the foot. Providing emergency medical care is impossible since you can't tell which are bitwise copies and which are just pointing at others and saying &bdquo;that's me, over there.&rdquo;*.

---

### Was unterscheidet C/C++ von anderen Programmiersprache wie z. B. Java, C# oder Python <a name="link3"></a>

  * Ein C/C++-�bersetzer generiert Maschinencode, so wie dieser vom Entwickler durch den von ihm geschriebenen Quellcode angefordert wird,
  *ohne* Sicherheitspr�fungen hinzuzuf�gen (es gibt Ausnahmen in C++).

  * Bei in C# oder Java geschriebenen Programmen w�rde beispielsweise ein falscher Array-Zugriff zu einer Laufzeitausnahme f�hren (*Exception*),
  w�hrend dies in C++ zu einem falschen Speicherzugriff (Lesen) oder einer Speicherverletzung beim Schreiben f�hrt.

  * Falsches oder unvorsichtiges Programmieren in C/C++ kann zu �berl�ufen (Stack-, Heap- oder globale Datenbereiche) f�hren,
  die leicht f�r einen Angriff verwendet werden k�nnen.

  * Bei konsequentem Einsatz der C++&ndash;Bibliothek (*STL*, *Standard Template Library*) lassen sich all diese Nachteile umgehen.
  Die Verwendung der STL zieht robusten, absturzsicheren und performanten Code nach sich.

---

### Exploitability und Vulnerability <a name="link4"></a>

#### Was versteht man unter einem *Exploit*?

Ein *Exploit* zeigt Sicherheitsl�cken von Software auf und erm�glicht deren Ausnutzung.
Exploits stellen ein Werkzeug f�r Hacker dar, um in Computersysteme einzudringen und sie zu manipulieren.
Sie k�nnen auch zur Beseitigung der Schwachstellen genutzt werden.

Der Begriff *Exploit* kommt vom englischen &bdquo;to exploit&rdquo;, zu deutsch etwa &bdquo;ausnutzen&rdquo;.
Ein Exploit stellt eine systematische M�glichkeit dar, aufgrund von Schwachstellen oder Sicherheitsl�cken
der Software in Computersysteme &ndash; in unserem Fall: in C/C++ Programme &ndash; einzudringen.

##### Welche Arten von Exploits gibt es?

Es existieren eine Vielzahl verschiedener Exploits. Abh�ngig von der verwendeten Angriffsart
und von zeitlichen Aspekten lassen sich Exploits unterschiedlich klassifizieren.
Oft genannte Exploit-Arten sind beispielsweise:

  * Remote ausgef�hrte Exploits
  * Lokale ausgef�hrte Exploits
  * *Denial-of-Service*-Exploits (*DoS*-Exploits)
  * SQL-Injection-Exploits
  * Command-Execution-Exploits
  * Zero-Day-Exploits

##### Der ber�hmteste Exploit

Der ber�hmteste Exploit ist der *Off-by-One Exploit*.

Dieser Exploit beschreibt einen h�ufigen Programmierfehler beim Berechnen von Array-Grenzen.
In Little-Endian-Architekturen kann dies zum �berschreiben des niederwertigsten Bytes f�hren.

#### Was versteht man unter *Vulnerability*?

Unter dem Begriff *Vulnerability* versteht man Schwachstellen,
m�glicherweise etwas drastischer �bersetzt *Hochrisiko-Schwachstellen*.

Wie kommt es dazu?

C und C++ sind sogenannte *Low-Level*-Programmiersprachen.

Das Speichermanagement ist also anders geregelt als bei High-Level-Sprachen wie JavaScript oder Python.
Bei diesen wird die Zuweisung, Nutzung und Bereitstellung implizit verwaltet (zum Beispiel durch *Garbage Collection*).

Bei C/C++ muss dies entwicklerseitig geschehen. 
Performanz und Implementierung lassen sich so zwar �u�erst feinstufig optimieren, zugleich ist dieses Programmierkonzept aber auch mit sehr spezifischen Problematiken verbunden.

*Buffer-Overflows* (CWE-121) und *Out-of-Bounds Write* (CWE-787) sind einige der bekanntesten
*Vulnerabilities* in C/C++.

---

### Sicherheitsl�cken versus *Undefined Behaviour* (*UB*) <a name="link5"></a>

In C/C++ gibt es sowohl Sicherheitsl�cken als auch das sogenannte *Undefined Behaviour* (kurz: *UB*).

Beide Begriffe liegen eng beieinander.

##### Beispiel f�r eine Sicherheitsl�cke: &bdquo;*Use After Free*&rdquo;

Es gibt den Datentyp Pointer in C/C++. Mit der Funktion `malloc` / dem Operator `new`
kann man Speicher reservieren, um ihn anschlie�end zu verwenden.

Der Name &bdquo;*Use After Free*&rdquo; verr�t eigentlich schon alles. Hierbei geht es darum, dass ein Zeiger auf eine Variable
nach seiner Freigabe weiterhin genutzt wird.

Es ist dem Compiler nicht m�glich, zur �bersetzungszeit zu �berpr�fen,
ob Variablen, die nur via Zeiger erreichbar sind (`malloc`, `new`), noch verf�gbar sind (`free`, `delete`).
Dies ist eine Sicherheitsl�cke in der Definition der Programmiersprache.

Die damit verbundene Anf�lligkeit (Sicherheitsl�cke) h�ngt mit einem Fehler im Zusammenhang
mit dem Speichermanagement im Ablauf des Programms zusammen:

Aus der Nutzung der Variable nach ihrer Freigabe resultieren im Programm
unerwartete Aktionen oder andere unerw�nschte Effekte. Dies bezeichnet man als *Undefined Behaviour*.
H�ufig macht man die Beobachtung, dass *Undefined Behaviour* zu einem Absturz des Programms f�hrt.

Andere Programmiersprachen gehen hier einen anderen Weg wie z. B. C# oder Java.
Hier wird vormals reservierter Speicher,
der im Programm keine Verwendung mehr findet, durch die Laufzeitumgebung wieder freigegeben (*Garbage Collection*).

---

## Die h�ufigsten *Exploits* und *Vulnerabilities*

### &bdquo;*Unsigned Integer Wraparound*&rdquo; <a name="link6"></a>

*Beschreibung*:

[CWE-190: Integer Overflow or Wraparound](https://cwe.mitre.org/data/definitions/190.html)

Die Schwachstelle f�hrt eine Berechnung durch, die einen Integer-�berlauf oder Wraparound erzeugen kann.

Dies tritt auf, wenn ein Integer-Wert auf einen Wert erh�ht wird, der zu gro� ist,
um in der zugeh�rigen Darstellung gespeichert werden zu k�nnen.

In diesem Fall kann der Wert eine sehr kleine oder negative Zahl werden.

*Beispiel*:

```cpp
01: // wrap around - well defined behaviour
02: unsigned int n = UINT_MAX - 1;
03: printf("%u\n", n);
04: 
05: n++;
06: printf("%u\n", n);
07: 
08: n++;    /* <- unsigned arithmetic, overflows, wraps around */
09: printf("%u\n", n);
10: 
11: n++;
12: printf("%u\n", n);
```

*Ausgabe*:

```
4294967294
4294967295
0
1
```


*Beispiel*:

```cpp
01: // undefined behavior
02: unsigned short n = USHRT_MAX;
03: printf("%hu\n", n);
04: 
05: // n *= INT_MAX;
06: n *= 12345;
07: printf("%hu\n", n);
```

*Ausgabe*:

```
65535
53191
```


*Bemerkung*:
Es wird bei der �berlauf-Problematik sehr genau zwischen *Wraparound* und *Undefined Behaviour* unterschieden.

Warum?

Eigentlich geht es bei dieser Betrachtung um die Unterscheidung
von vorzeichenlosen und vorzeichenbehafteten Integer-Typen:

  * Vorzeichenlose Integer-Typen:<br />
  Vorzeichenlose Integer-Typen implementieren Modulo-Arithmetik.
  Das Modulo ist gleich 2<sup>*N*</sup>, wobei *N* die Anzahl der Bits in der Bin�rdarstellung des Datentyps ist.
  Aus diesem Grund scheinen vorzeichenlose Integer-Typen bei einem �berlauf tats�chlich umzubrechen.
  Dieses Verhalten ist f�r vorzeichenlose Typen eindeutig definiert.

  * Vorzeichenbehaftete Integer-Typen:<br />
  Vorzeichenbehaftete Integer-Variablen kennen in der Programmiersprache C kein *Wraparound*-Verhalten.
  Ein �berlauf von vorzeichenbehafteten Integer-Typen w�hrend arithmetischer Berechnungen f�hrt zu undefiniertem Verhalten (*Undefined Behaviour*).<br />
  F�r vorzeichenbehaftete Typen ist es eben nicht eindeutig definiert,
  wie eine derartige Zahl intern (Bin�rdarstellung) dargestellt wird.
  Es gibt mehrere Methoden zur Darstellung vorzeichenbehafteter Zahlen im Speicher:
    * Einer-Komplement: der negative Wert ist das bitweise Inverse des positiven Wertes.
    * Zweier-Komplement: der negative Wert ist das bitweise Inverse des positiven Wertes plus 1.


Noch eine abschlie�ende Bemerkung: 

Gibt es eine `short`-Arithmetik?

*Antwort*:<br />
Nein! Der Maschinencode eines C-Programms f�hrt niemal arithmetische Berechnungen in Bereichen durch,
die kleiner sind als die von `signed int`/`unsigned int`.

Eine `short int`-Variable wird in Ausdr�cken typischerweise vor Beginn der Berechnungen in den Typ `int` umgewandelt.
Das bedeutet:

  * Berechnungen mit `unsigned short int` / `signed short int` werden im Bereich von `unsigned int` / `signed int` durchgef�hrt, wobei es zu einem �berlauf kommt, wenn `unsigned int` / `signed int` �berl�uft.
  * Ein �berlauf w�hrend solcher Berechnungen f�hrt zu undefiniertem Verhalten, nicht zu einem Wrap-Around-Verhalten.
  * Das im `unsigned int` / `signed int`-Bereich berechnete Ergebnis wird in eine `unsigned short int` / `signed short int` zur�ckkonvertiert. Hier kann es zu einem Konvertierungsfehler kommen.

### &bdquo;*Signed Integer Overflow*&rdquo; <a name="link7"></a>

*Beschreibung*:

[CWE-190: Integer Overflow or Wraparound](https://cwe.mitre.org/data/definitions/190.html)

Siehe &bdquo;*Unsigned Integer Wraparound*&rdquo;.


### &bdquo;*Numeric Truncation*&rdquo; <a name="link8"></a>

*Beschreibung*:

[CWE-197: Numeric Truncation Error](https://cwe.mitre.org/data/definitions/197.html)

  * Implementierungsdefiniertes Verhalten (*Implementation Defined Behaviour*, *IDB*)
  * Ganzzahlkonvertierungen f�hren zu Datenverlust oder Fehlinterpretationen

Wenn Werte eines elementaren Datentyps in einen Wert eines kleineren elementaren Datentyps umgewandelt werden,
gehen die h�herwertigen Bits des gr��eren Werts bei der Konvertierung verloren,
was m�glicherweise zu einem unerwarteten Wert f�hrt, der nicht dem urspr�nglichen Wert entspricht.

Dieser Wert kann als Index eines Puffers, als Schleifenindex oder einfach als normale Variable erforderlich sein.

In jedem Fall ist der Wert nicht vertrauensw�rdig und das System befindet sich in einem undefinierten Zustand.

*Beispiel*:

```cpp
01: int intPrimitive = 0;
02: short shortPrimitive = 0;
03: intPrimitive = INT_MAX;
04: shortPrimitive = intPrimitive;   // Numeric Truncation Error
05: 
06: printf("Integer MAXINT: %d\n", intPrimitive);
07: printf("Short   MAXINT: %d\n", shortPrimitive);
08: 
09: shortPrimitive = SHRT_MAX;
10: printf("Short   MAXINT: %d\n", shortPrimitive);
```

*Ausgabe*:

```
Integer MAXINT: 2147483647
Short   MAXINT: -1
Short   MAXINT: 32767
```


### &bdquo;*Stack Buffer Overflow*&rdquo; <a name="link9"></a>

*Beschreibung*:

[CWE-121: Stack-based Buffer Overflow](https://cwe.mitre.org/data/definitions/121.html)

&bdquo;*Stack Overflow*&rdquo;, auch als &bdquo;*Stack Exhaustion*&rdquo; bezeichnet, 
ist normalerweise das Ergebnis von �berm��ig vielen geschachtelten Funktionsaufrufen,
bisweilen auch rekursiven Funktionsaufrufen.

Ebenfalls kritisch f�r den Stapel sind zu viele gro�e Datenmengen,
die in Funktionen als &bdquo;*Stack*&rdquo;-Variablen angelegt werden.


*Beschreibung*:

[CWE-242: Use of Inherently Dangerous Function](https://cwe.mitre.org/data/definitions/242.html)

Die Schwachstelle ruft eine Funktion auf, deren sichere Funktion nie garantiert werden kann.


*Beschreibung*:

[CWE-787: Out-of-bounds Write](https://cwe.mitre.org/data/definitions/787.html)

Die Schwachstelle schreibt Daten �ber das Ende oder vor den Anfang des vorgesehenen Puffers hinaus.

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


### &bdquo;*Heap Buffer Overflow*&rdquo; <a name="link10"></a>

*Beschreibung*:

[CWE-122: Heap-based Buffer Overflow](https://cwe.mitre.org/data/definitions/122.html)

Bei einem Heap-�berlauf handelt es sich um einen Puffer�berlauf, bei dem der Puffer, der �berschrieben werden kann,
im Heap-Bereich des Speichers liegt.

Dies bedeutet im Allgemeinen, dass der Puffer mithilfe einer Routine wie `malloc()` / `new` zugewiesen wurde.



### &bdquo;*Buffer Underflow*&rdquo; <a name="link11"></a>

*Beschreibung*:

[CWE-124: Buffer Underwrite ('Buffer Underflow')](https://cwe.mitre.org/data/definitions/124.html)

Die Schwachstelle schreibt in einen Puffer mithilfe eines Index oder Zeigers,
der auf einen Speicherort vor dem Anfang des Puffers verweist.

*Beispiel*:

```cpp
01: char src[12];
02: 
03: strncpy(src, "Hello World", sizeof(src));
04: 
05: size_t length = strlen(src);
06: 
07: int index = (length - 1);
08: while (src[index] != ':') {
09:     src[index] = '\0';
10:     index--;
11: }
```

### &bdquo;*Use after Free*&rdquo; <a name="link12"></a>

*Beschreibung*:

[CWE-416: Use After Free](https://cwe.mitre.org/data/definitions/416.html)

Die Schwachstelle verwendet den Speicher erneut oder verweist auf ihn, nachdem er freigegeben wurde.
Irgendwann danach kann der Speicher erneut zugewiesen und in einem anderen Zeiger gespeichert werden,
w�hrend der urspr�ngliche Zeiger auf einen Speicherort irgendwo innerhalb der neuen Zuweisung verweist.

Alle Vorg�nge, die den urspr�nglichen Zeiger verwenden, sind nicht mehr g�ltig, da der Speicher dem Code &bdquo;geh�rt&rdquo;,
der mit dem neuen Zeiger arbeitet.

*Beispiel*:

```cpp
01: char* buffer = new char[256];
02: memset(buffer, '!', 256);
03: 
04: bool error = true;
05: if (error)
06:     delete[] buffer;
07: 
08: if (error) {
09:     // Use after free when error is true
10:     // printf("%lu\n", strlen(buffer));
11: 
12:     size_t len = strlen(buffer);   // use after free
13: }
```


### &bdquo;*Double Free*&rdquo; <a name="link13"></a>

*Beschreibung*:

[CWE-415: Double Free](https://cwe.mitre.org/data/definitions/415.html)

Die Schwachstelle ruft `free()` / `delete` zweimal f�r dieselbe Speicheradresse auf,
was m�glicherweise zur �nderung unerwarteter Speicherorte f�hrt.

*Beispiel*:

```cpp
01: char* buffer = new char[256];
02: memset(buffer, '!', 256);
03: 
04: bool error = true;
05: if (error)
06:     delete[] buffer;
07: 
08: // ....
09: 
10: delete[] buffer; // second free
```


### &bdquo;*Incorrect Type Conversion*&rdquo; / &bdquo;*Type Punning*&rdquo; <a name="link14"></a>

*Beschreibung*:

[CWE-704: Incorrect Type Conversion or Cast](https://cwe.mitre.org/data/definitions/704.html)

Die Schwachstelle konvertiert ein Objekt, eine Ressource oder eine Struktur nicht ordnungsgem�� von einem Typ in einen anderen Typ.

Was bezeichnet man als *Type Punning*?

&bdquo;Eine Form des Zeigeraliasings, bei der zwei Zeiger auf dieselbe Stelle im Speicher verweisen,
diese Stelle aber aus der Sichtweise unterschiedlicher Datentypen betrachten&rdquo;<br />.

Der Compiler behandelt beide &bdquo;*Type Punnings*&rdquo; als nicht verwandte Zeiger.

*Type Punnings* k�nnen Abh�ngigkeitsprobleme f�r alle Daten verursachen, auf die �ber beide Zeiger zugegriffen wird.&rdquo;

*Beispiel*:

```cpp
01: struct A {};
02: struct B {};
03: 
04: struct A* a = (struct A*) malloc(sizeof(struct A));
05: 
06: // cast to unrelated type
07: struct B* b = (struct B*) a;
```


### &bdquo;*Uncontrolled Format String*&rdquo; <a name="link15"></a>

*Beschreibung*:

[CWE-134: Use of Externally-Controlled Format String](https://cwe.mitre.org/data/definitions/134.html)

Die Schwachstelle verwendet eine Funktion,
die eine Formatzeichenfolge als Argument akzeptiert, die Formatzeichenfolge stammt jedoch aus einer externen Quelle.

---

[Zur�ck](./Readme_Secure_Programming.md)

---
