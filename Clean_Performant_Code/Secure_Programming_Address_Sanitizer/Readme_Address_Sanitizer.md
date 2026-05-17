# Visual Studio Address Sanitizer

[Zurück](../Secure_Programming/Readme_Secure_Programming.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Interne Funktionsweise eines Address Sanitizers](#link2)
  * [Debug- und Release-Build](#link3)
  * [Installation des Address Sanitizers](#link4)
  * [Ein erstes Beispiel](#link5)
  * [Fehler: *alloc-dealloc-mismatch*](#link6)
  * [Fehler: *stack-buffer-underflow*](#link7)
  * [Fehler: *double-free*](#link8)
  * [Fehler: *invalid-allocation-alignment*](#link9)
  * [Fehler: *new-delete-type-mismatch*](#link10)
  * [Fehler: *stack-use-after-scope*](#link11)
  * [Fehler: *stack-use-after-return*](#link12)
  * [Fehler: *global-buffer-overflow*](#link13)
  * [Literatur](#link14)

---

#### Quellcode

[*01_Basic_Global_Buffer_Overflow.cpp*](01_Basic_Global_Buffer_Overflow.cpp)<br/>
[*02_Alloc_Dealloc_Mismatch.cpp*](02_Alloc_Dealloc_Mismatch.cpp)<br/>
[*03_Stack_Buffer_Underflow.cpp*](03_Stack_Buffer_Underflow.cpp)<br/>
[*04_Double_Free.cpp*](04_Double_Free.cpp)<br/>
[*05_Memory_Leak_Detection.cpp*](05_Memory_Leak_Detection.cpp)<br/>
[*06_Invalid_Allocation_Alignment.cpp*](06_Invalid_Allocation_Alignment.cpp)<br/>
[*07_Virtual_Base_Class_Destructor.cpp*](07_Virtual_Base_Class_Destructor.cpp)<br/>
[*08_Stack_Use_After_Scope.cpp*](08_Stack_Use_After_Scope.cpp)<br/>
[*09_Stack_Use_After_Return.cpp*](09_Stack_Use_After_Return.cpp)<br/>
[*10_Global_Buffer_Overflow.cpp*](10_Global_Buffer_Overflow.cpp)<br/>
[*Program.cpp*](Program.cpp)<br />

---

## Allgemeines <a name="link1"></a>

So genannte *Address Sanitizer* sind eine Compiler- und Laufzeittechnologie,
die schwer zu findende Fehler aufdecken.

Address Sanitizer wurde ursprünglich von Google eingeführt
und bieten Technologien zur Laufzeitfehlersuche,
die das vorhandene Build-System und die vorhandenen Testressourcen direkt nutzen.

Der Visual C++ Sanitizer kann folgende Fehlerursachen aufspüren:

 * Alloc/dealloc mismatches and new/delete type mismatches.
 * Allocations too large for the heap.
 * `calloc` overflow and `alloca` overflow.
 * *Double free* and use after free.
 * Global variable overflow.
 * Heap buffer overflow.
 * Invalid alignment of aligned values.
 * `memcpy` and `strncat` parameter overlap.
 * Stack buffer overflow and underflow.
 * Stack use after return and use after scope.
 * Memory use after it's poisoned.

---

## Interne Funktionsweise eines Address Sanitizers <a name="link2"></a>

Ein Address Sanitizer (ASan) arbeitet, indem es Übersetungszeit-Zeit Instrumentierungen
mit einer Laufzeitbibliothek kombiniert,
um jedes einzelne Byte des Speichers zu überwachen, auf das ein Programm zugreift.

### Der Kernmechanismus: Shadow Memory.

ASan reserviert einen dedizierten Bereich des virtuellen Adressraums, der als &bdquo;*Shadow Memory*&rdquo; (Schatten-Speicher) bezeichnet wird.

  * 1:8-Verhältnis: Jeweils acht Bytes des Anwendungsspeichers werden durch genau ein Byte im Shadow Memory überwacht.
  * Mapping: Das Tool verwendet eine einfache mathematische Formel, um für jede beliebige Speicheradresse schnell das entsprechende Shadow-Byte zu ermitteln: `ShadowAddr = (Addr >> 3) + Offset`.
  * Poisoning: Der Wert eines Shadow-Bytes gibt an, ob die zugehörigen 8 Bytes &bdquo;clean&rdquo; (sicher zugreifbar) oder &bdquo;poisoned&rdquo; (ungültig) sind.

### Wie wird der Maschinencode modifiziert

Wird in Visual Studio die Option `/fsanitize=address` aktiviert, &bdquo;bestückt&rdquo; der Compiler den Maschinencode tatsächlich mit zusätzlichen Instruktionen:

  * Instruktions-Injektion: Vor jedem Lese- oder Schreibzugriff auf den Speicher fügt der Compiler eine &bdquo;Prüfung&rdquo; ein. Er berechnet die Shadow-Adresse, liest das entsprechende Shadow-Byte aus und überprüft, ob der Speicherbereich als &bdquo;poisoned&rdquo; markiert ist.
  * Redzones: Der Compiler fügt &bdquo;Padding&rdquo; (Füllbereiche) um globale Variablen sowie Stack-Variablen herum ein. Diese &bdquo;Redzones&rdquo; werden im Shadow Memory als &bdquo;poisoned&rdquo; markiert; somit trifft jeder &bdquo;Off-by-one&rdquo;-Fehler (ein um eins verschobener Zugriff) unmittelbar auf ein markiertes Byte und löst einen Fehler aus.
  * Funktions-Interzeption: Die Laufzeitbibliothek ersetzt Standardfunktionen wie `malloc` und `free`.
    * `malloc` fügt um den zugewiesenen Speicherblock herum &bdquo;poisoned&rdquo; Redzones ein.
    * `free` markiert den gesamten freigegebenen Block als &bdquo;poisoned&rdquo; und verschiebt ihn in eine &bdquo;Quarantäne&rdquo;, sodass er nicht unmittelbar wiederverwendet werden kann &ndash; dies hilft dabei, &bdquo;Use-after-free&rdquo;-Fehler zu erkennen.
  
### Leistung und Visual Studio-Integration

  * 2-fache Verlangsamung: Aufgrund der zusätzlichen Instruktionen bei jedem Speicherzugriff läuft Ihr Programm typischerweise etwa doppelt so langsam.
  * Speicher-Overhead: Der Speicherverbrauch steigt aufgrund des Shadow Memory und der Redzones häufig auf das 2- bis 4-fache an.
  * IDE-Feedback: In Visual Studio 2019 (ab Version 16.9) und 2022 fängt die IDE im Fehlerfall die Meldung ab und zeigt einen detaillierten Bericht direkt im Editor an; dabei werden die exakte Codezeile sowie die Zuweisungshistorie des betroffenen Speicherbereichs präzise identifiziert.
  
*Fazit*:<br />
ASan stellt keine bloßen Vermutungen an; Es validiert aktiv den Speicherzustand bei jeder einzelnen Lese- oder Schreiboperation auf der Ebene der Hardware-Instruktionen.

---

## Debug- und Release-Build <a name="link3"></a>

In Visual Studio ist der Address Sanitizer sowohl im Debug- als auch im Release-Builds einsetzbar.

Die Wahl hängt jedoch von Ihrem spezifischen Ziel für die jeweilige Sitzung ab.

Verwende den Debug-Mode für die anfängliche Fehlersuche.

Dies ist der Standardansatz für die meisten Entwickler während der Programmierphase.

 * Voller Kontext: Man erhält präzise Zeilennummern und Variablennamen in den Fehlerberichten.
 * Keine Störung durch Optimierungen: Compiler &bdquo;optimieren&rdquo; manchmal fehlerhaften Code weg, wodurch es für ASan (Address Sanitizer) im Release-Modus schwieriger wird, diese Fehler zu erkennen.
 * Einfachere Einrichtung: ASan lässt sich im Debug-Modus oft leichter konfigurieren, da andere Debugging-Funktionen wie Assertions (`assert`) ergänzt werden.

Verwende den Release-Mode für &bdquo;versteckte&rdquo; oder leistungsintensive Fehler:

Wechseln Sie in diesen spezifischen Szenarien zu einem Release-Build mit ASan:

 * Fehler durch Optimierung: Manche Speicherprobleme treten nur dann auf, wenn der Compiler den Code in Bezg auf die Optimierung umstrukturiert hat.
 * Leistungsintensive Anwendungen: ASan verursacht einen erheblichen Mehraufwand (2- bis 3-mal langsamer). Wenn Ihre Anwendung im Debug-Modus zu langsam für Tests ist, kann ein Release-Build sie wieder nutzbar machen.
 * Continuous Integration (CI): Viele Teams erstellen eine spezielle &bdquo;Release-ASan&rdquo;-Konfiguration für automatisierte Tests, um sicherzustellen, dass der produktionsnahe Code-Pfad stabil ist.


---

## Installation des Address Sanitizers <a name="link4"></a>

Zur Installation des Address Sanitizers finden sich [hier](https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170) Hinweise.

Grundlegende Voraussetzung ist natürlich, dass der Sanitizer bei der Visual Studio Installation mit berücksichtigt wurde:

<img src="VisualStudio_AddressSanitizer_02.png" width="300">

*Abbildung* 1: Installation des *Address Sanitizers* in den Einstellungen des *Visual Studio Installers*.

Dann muss man den Sanitizer pro Projekt in den *Projekt Eigenschaften* aktivieren:

<img src="VisualStudio_AddressSanitizer_01.png" width="700">

*Abbildung* 2: *Enable Address Sanitizer*-Einstellung in den Einstellungen des Projekts.


*Hinweis*:<br />
Bei Ausführung des Sanitizers auf meinem Rechner kommt es bei
den Ausgaben des Sanitizers zu einer Fehlermeldung:

*Visual Studio 22 - Asan - Failed to use and restart external symbolizer*

In *Stackoverflow* kann man
[hier](https://stackoverflow.com/questions/76781556/visual-studio-22-asan-failed-to-use-and-restart-external-symbolizer) nachlesen,
wie man den Fehler behebt.

Es muss &ndash; und das ist etwas schlecht in *SO* beschrieben &ndash;,
der zweite Pfad entfernt werden:

```
PATH=$(VC_ExecutablePath_x64);%PATH%
ASAN_SYMBOLIZER_PATH=$(VC_ExecutablePath_x64)
```
   
---

## Ein erstes Beispiel <a name="link5"></a>

Es folgt ein Beispiel, um den Address Sanitizer zu testen:

```cpp
01: int buffer[20];
02: 
03: void test_01_basic_global_buffer_overflow()
04: {
05:     std::println("Hello Global Buffer Overflow:");
06: 
07:     std::size_t n{ 20 };
08:     buffer[n] = 123; // Boom!
09: }
```

Die Ausgaben in der Konsole sehen nun so aus:


```
Hello Global Buffer Overflow:
=================================================================
==17760==ERROR: AddressSanitizer: global-buffer-overflow on address 0x7ff6391966b0 at pc 0x7ff6390b4f63 bp 0x000d5c6ffca0 sp 0x000d5c6ffca8
WRITE of size 4 at 0x7ff6391966b0 thread T0
    #0 0x7ff6390b4f62 in test_01_basic_global_buffer_overflow(void) C:\01_Basic_Global_Buffer_Overflow.cpp:15
    #1 0x7ff6390b4ea4 in main C:\Program.cpp:19
    #2 0x7ff639132628 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #3 0x7ff639132571 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #4 0x7ff63913242d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #5 0x7ff63913269d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #6 0x7ff98b69e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #7 0x7ff98d36427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

0x7ff6391966b0 is located 0 bytes after global variable 'buffer' defined in '01_Basic_Global_Buffer_Overflow.cpp:7:4' (0x7ff639196660) of size 80
SUMMARY: AddressSanitizer: global-buffer-overflow C:\01_Basic_Global_Buffer_Overflow.cpp:15 in test_01_basic_global_buffer_overflow(void)
Shadow bytes around the buggy address:
  0x7ff639196400: 04 f9 f9 f9 00 f9 f9 f9 00 f9 f9 f9 00 f9 f9 f9
  0x7ff639196480: 00 f9 f9 f9 01 f9 f9 f9 00 00 00 00 f9 f9 f9 f9
  0x7ff639196500: 00 f9 f9 f9 00 f9 f9 f9 00 00 00 00 00 f9 f9 f9
  0x7ff639196580: f9 f9 f9 f9 00 00 00 00 00 f9 f9 f9 f9 f9 f9 f9
  0x7ff639196600: 00 00 00 00 00 f9 f9 f9 f9 f9 f9 f9 00 00 00 00
=>0x7ff639196680: 00 00 00 00 00 00[f9]f9 f9 f9 f9 f9 00 00 00 00
  0x7ff639196700: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff639196780: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff639196800: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff639196880: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff639196900: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==17760==ABORTING
```

---

## Fehler: *alloc-dealloc-mismatch* <a name="link6"></a>

Ermöglicht die Laufzeiterkennung von nicht übereinstimmenden Speicheroperationen,
die zu undefiniertem Verhalten führen können &ndash; wie zum Beispiel:

  * `malloc` muss mit `free` gepaart werden, nicht mit `delete` oder `delete[]`.
  * `new` muss mit `delete` gepaart werden, nicht mit `free` oder `delete[]`.
  * `new[]` muss mit `delete[]` gepaart werden, nicht mit `delete` oder `free`.

Unter Windows ist die Fehlererkennung für `alloc-dealloc-mismatch` standardmäßig deaktiviert.
Um sie zu aktivieren, ist die Umgebungsvariable `ASAN_OPTIONS=alloc_dealloc_mismatch=1` vor der Programmausführung zu setzen.

*Hinweis*:<br />
Mit der Eingabe von *env* im *Developer Command Prompt* erhält man den Zugang zu den Umgebungsvariablen.

*Hinweis*:<br />
Nach dem Setzen der Umgebungsvariablen ist Visual Studio neu zu starten, damit die Änderungen wirksam werden.


*Beispiel*:

```cpp
01: void test_02_alloc_dealloc_mismatch()
02: {
03:     std::println("Allocate / Deallocate Mismatch:");
04: 
05:     int ready{ 1 };
06:     int* adr{ nullptr };
07: 
08:     while (ready != 0) {
09: 
10:         std::println("Choose action:");
11:         std::println("(1) = Allocate memory with new");
12:         std::println("(2) = Allocate memory with new[]");
13:         std::println("(3) = Deallocate memory with delete");
14:         std::println("(4) = Deallocate memory with delete[]");
15:         std::println("(9) = Exit");
16:         std::println();
17: 
18:         int action { - 1 };
19: 
20:         scanf_s("%d", &action);
21: 
22:         switch (action)
23:         {
24:         case 1:
25:             adr = new int{ 123 };
26:             break;
27:         case 2:
28:             adr = new int [20] {};
29:             break;
30:         case 3:
31:             delete adr;
32:             break;
33:         case 4:
34:             delete[] adr;
35:             break;
36:         case 9:
37:             ready = 0;
38:             break;
39:         default:
40:             printf("Unknown command: %d", action);
41:             break;
42:         }
43:     }
44: }
```

*Ausgabe*:

```
Allocate / Deallocate Mismatch:
Choose action:
(1) = Allocate memory with new
(2) = Allocate memory with new[]
(3) = Deallocate memory with delete
(4) = Deallocate memory with delete[]
(9) = Exit

2
Choose action:
(1) = Allocate memory with new
(2) = Allocate memory with new[]
(3) = Deallocate memory with delete
(4) = Deallocate memory with delete[]
(9) = Exit

3
=================================================================
==5860==ERROR: AddressSanitizer: alloc-dealloc-mismatch (operator new [] vs operator delete) on 0x119b6f1a0020
    #0 0x7ff753c511f3 in operator delete(void *, unsigned __int64) D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_delete_scalar_size_thunk.cpp:41
    #1 0x7ff753b6159d in test_02_alloc_dealloc_mismatch(void) C:\02_Alloc_Dealloc_Mismatch.cpp:37
    #2 0x7ff753bd4ea4 in main C:\Program.cpp:20
    #3 0x7ff753c52628 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff753c52571 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff753c5242d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff753c5269d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

0x119b6f1a0020 is located 0 bytes inside of 80-byte region [0x119b6f1a0020,0x119b6f1a0070)
allocated by thread T0 here:
    #0 0x7ff753c5129e in operator new[](unsigned __int64) D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_new_array_thunk.cpp:41
    #1 0x7ff753b6151e in test_02_alloc_dealloc_mismatch(void) C:\02_Alloc_Dealloc_Mismatch.cpp:34
    #2 0x7ff753bd4ea4 in main C:\Program.cpp:20
    #3 0x7ff753c52628 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff753c52571 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff753c5242d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff753c5269d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

SUMMARY: AddressSanitizer: alloc-dealloc-mismatch C:\02_Alloc_Dealloc_Mismatch.cpp:37 in test_02_alloc_dealloc_mismatch(void)
==5860==HINT: if you don't care about these errors you may set ASAN_OPTIONS=alloc_dealloc_mismatch=0
==5860==ABORTING
```

---

## Fehler: *stack-buffer-underflow* <a name="link7"></a>

Diese Fehlermeldungen weisen auf einen Speicherzugriff vor dem Beginn einer Stack-Variablen hin.

*Beispiel*:

```cpp
01: void test_03_stack_buffer_underflow_01()
02: {
03:     int subscript = -1;
04:     char buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
05:     buffer[subscript] = 123; // Boom!
06: }
07: 
08: void threadProcedure()
09: {
10:     int subscript = -1;
11:     char buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
12:     buffer[subscript] = 123; // Boom!
13: }
14: 
15: void test_03_stack_buffer_underflow_02()
16: {
17:     std::thread t{ threadProcedure };
18:     t.join();
19: }
20: 
21: void test_03_stack_buffer_underflow()
22: {
23:    test_03_stack_buffer_underflow_01();
24:    test_03_stack_buffer_underflow_02();
25: }
```

*Ausgabe*:

```
=================================================================
==12748==ERROR: AddressSanitizer: stack-buffer-underflow on address 0x001838bff51f at pc 0x7ff6abf1ccc5 bp 0x001838bff4e0 sp 0x001838bff4e8
WRITE of size 1 at 0x001838bff51f thread T1
    #0 0x7ff6abf1ccc4 in threadProcedure C:\03_Stack_Buffer_Underflow.cpp:19
    #1 0x7ff6abf1d77b in std::invoke<void (__cdecl *)(void)>(void (__cdecl *&&)(void)) C:\Tools\MSVC\14.51.36231\include\type_traits:1801
    #2 0x7ff6abf1d26b in std::thread::_Invoke<class std::tuple<void (__cdecl *)(void)>, 0>(void *) C:\Tools\MSVC\14.51.36231\include\thread:60
    #3 0x7ffd70ba2ec4  (C:\Windows\SYSTEM32\ucrtbased.dll+0x1800a2ec4)
    #4 0x7ffd4e2db71c in asan_thread_start D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win.cpp:372
    #5 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #6 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

Address 0x001838bff51f is located in stack of thread T1 at offset 31 in frame
    #0 0x7ff6abf1c87f in threadProcedure C:\03_Stack_Buffer_Underflow.cpp:16

  This frame has 1 object(s):
    [32, 42) 'buffer' <== Memory access at offset 31 underflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp, SEH and C++ exceptions *are* supported)
Thread T1 created by T0 here:
    #0 0x7ffd4e2d6567 in CreateThread D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win.cpp:430
    #1 0x7ffd70ba3616  (C:\Windows\SYSTEM32\ucrtbased.dll+0x1800a3616)
    #2 0x7ff6abf1d419 in std::thread::_Start<void (__cdecl &)(void)>(void (__cdecl &)(void)) C:\Tools\MSVC\14.51.36231\include\thread:78
    #3 0x7ff6abf1ce88 in std::thread::thread<void (__cdecl &)(void), 0>(void (__cdecl &)(void)) C:\Tools\MSVC\14.51.36231\include\thread:93
    #4 0x7ff6abf1cdd4 in test_03_stack_buffer_underflow_02 C:\03_Stack_Buffer_Underflow.cpp:24
    #5 0x7ff6abf1ce54 in test_03_stack_buffer_underflow(void) C:\03_Stack_Buffer_Underflow.cpp:31
    #6 0x7ff6abf849f4 in main C:\Program.cpp:21
    #7 0x7ff6ac002178 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #8 0x7ff6ac0020c1 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #9 0x7ff6ac001f7d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #10 0x7ff6ac0021ed in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #11 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #12 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

SUMMARY: AddressSanitizer: stack-buffer-underflow C:\03_Stack_Buffer_Underflow.cpp:19 in threadProcedure
Shadow bytes around the buggy address:
  0x001838bff280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff300: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff380: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff400: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x001838bff500: f1 f1 f1[f1]00 02 f3 f3 f3 f3 00 00 00 00 00 00
  0x001838bff580: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff600: 00 00 00 00 00 00 00 00 f1 f1 f1 f1 00 f3 f3 f3
  0x001838bff680: f3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff700: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x001838bff780: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==12748==ABORTING
```

---

## Fehler: *double-free* <a name="link8"></a>

Es geht um die Freigabe eines bereits freigegebenen Speicherbereichs.

  * In C kann man `free` fehlerhaft aufrufen.
  * In C++ kann man `delete` mehr als einmal aufrufen.


*Beispiel*:

```cpp
01: void test()
02: {
03:     int* buffer = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04:     std::println("buffer: {:#X}", reinterpret_cast<intptr_t>(buffer));
05: 
06:     // ... some complex body of code
07: 
08:     delete[] buffer;
09: 
10:     // ... some complex body of code
11: 
12:     delete[] buffer;
13: }
```

*Ausgabe*:

```
buffer: 0X11E3510A0010
=================================================================
==9956==ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed: 0x000000008123 in thread T0
    #0 0x7ff611a812ee in operator delete[](void *) D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_delete_array_thunk.cpp:43
    #1 0x7ff61199eb64 in test_04_double_free_02 C:\Secure_Programming_Address_Sanitizer\04_Double_Free.cpp:35
    #2 0x7ff61199ebf4 in test_04_double_free(void) C:\Secure_Programming_Address_Sanitizer\04_Double_Free.cpp:41
    #3 0x7ff611a04e44 in main C:\Secure_Programming_Address_Sanitizer\Program.cpp:22
    #4 0x7ff611a825c8 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #5 0x7ff611a82511 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #6 0x7ff611a823cd in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #7 0x7ff611a8263d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #8 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #9 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

Address 0x000000008123 is a wild pointer inside of access range of size 0x000000000001.
SUMMARY: AddressSanitizer: bad-free C:\Secure_Programming_Address_Sanitizer\04_Double_Free.cpp:35 in test_04_double_free_02
==9956==ABORTING
```


---

## Fehler: *invalid-allocation-alignment* <a name="link9"></a>

Die Funktion `_aligned_malloc` erfordert eine Zweierpotenz zur Angabe der Ausrichtung.
Wir simulieren die &bdquo;externe&bdquo; Berechnung eines Alignment-Faktors mithilfe einer unoptimierten globalen Variable.


*Beispiel*:

```cpp
01: int ExternalAlign = 5;
02: 
03: void test() {
04: 
05:     // this externally calculated alignment of 5 isn't valid.
06: 
07:     void* ptr = _aligned_malloc(8, ExternalAlign);
08: 
09:     bool result = (ptr == nullptr && errno == EINVAL) ? 0 : -1;
10: }
```

*Ausgabe*:

```
=================================================================
==13164==ERROR: AddressSanitizer: invalid allocation alignment: 5, alignment must be a power of two (thread T0)
    #0 0x7ffd665c2ae9 in _aligned_malloc D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_malloc_win.cpp:531
    #1 0x7ff755b9f3c0 in test_06_invalid_allocation_alignment(void) C:\Secure_Programming_Address_Sanitizer\06_Invalid_Allocation_Alignment.cpp:13
    #2 0x7ff755ba4e44 in main C:\Secure_Programming_Address_Sanitizer\Program.cpp:24
    #3 0x7ff755c225c8 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff755c22511 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff755c223cd in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff755c2263d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

==13164==HINT: if you don't care about these errors you may set allocator_may_return_null=1
SUMMARY: AddressSanitizer: invalid-allocation-alignment C:\Secure_Programming_Address_Sanitizer\06_Invalid_Allocation_Alignment.cpp:13 in test_06_invalid_allocation_alignment(void)
==13164==ABORTING
```

---

## Fehler: *new-delete-type-mismatch* <a name="link10"></a>

Im Beispiel zu diesem Fehler wird von einer Klasse `Base` wird nur der Konstruktor `~Base` aufgerufen, nicht jedoch `~Derived`.

Der Compiler generiert einen Aufruf von `~Base()`, da der Destruktor der Basisklasse nicht virtuell ist.
Beim Aufruf von `delete b` wird der Destruktor des Objekts an die Standarddefinition gebunden.

Ein fehlendes Schlüsselwort `virtual` bei der Deklaration des Destruktors ist ein häufiger Fehler in C++ bei der Verwendung von Vererbung.


*Beispiel*:

```cpp
01: struct T
02: {
03:     T() : v(100) {}
04:     std::vector<int> v;
05: };
06: 
07: struct Base {
08:   //  virtual ~Base() = default;     // remove comment
09: };
10: 
11: struct Derived : public Base
12: {
13:     T t;
14: };
15: 
16: void test_07_virtual_base_class_destructor()
17: {
18:     Base* b = new Derived;
19:     delete b;  // Boom! 
20: 
21:     // oder
22: 
23:     std::unique_ptr<Base> b1 = std::make_unique<Derived>();
24: }
```

*Ausgabe*:

```
=================================================================
==26284==ERROR: AddressSanitizer: new-delete-type-mismatch on 0x11d2240a0010 in thread T0:
  object passed to delete has wrong type:
  size of the allocated type:   32 bytes;
  size of the deallocated type: 1 bytes.
    #0 0x7ff61b461193 in operator delete(void *, unsigned __int64) D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_delete_scalar_size_thunk.cpp:41
    #1 0x7ff61b3df59a in test_07_virtual_base_class_destructor(void) C:\Secure_Programming_Address_Sanitizer\07_Virtual_Base_Class_Destructor.cpp:26
    #2 0x7ff61b3e4e44 in main C:\Secure_Programming_Address_Sanitizer\Program.cpp:25
    #3 0x7ff61b4625c8 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff61b462511 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff61b4623cd in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff61b46263d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

0x11d2240a0010 is located 0 bytes inside of 32-byte region [0x11d2240a0010,0x11d2240a0030)
allocated by thread T0 here:
    #0 0x7ff61b461075 in operator new(unsigned __int64) D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_win_new_scalar_thunk.cpp:40
    #1 0x7ff61b3df522 in test_07_virtual_base_class_destructor(void) C:\Secure_Programming_Address_Sanitizer\07_Virtual_Base_Class_Destructor.cpp:25
    #2 0x7ff61b3e4e44 in main C:\Secure_Programming_Address_Sanitizer\Program.cpp:25
    #3 0x7ff61b4625c8 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff61b462511 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff61b4623cd in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff61b46263d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

SUMMARY: AddressSanitizer: new-delete-type-mismatch C:\Secure_Programming_Address_Sanitizer\07_Virtual_Base_Class_Destructor.cpp:26 in test_07_virtual_base_class_destructor(void)
==26284==HINT: if you don't care about these errors you may set ASAN_OPTIONS=new_delete_type_mismatch=0
==26284==ABORTING
```


---

## Fehler: *stack-use-after-scope* <a name="link11"></a>

Die Verwendung einer Stack-Adresse außerhalb des lexikalischen Gültigkeitsbereichs der Lebensdauer einer Variablen
kann in C oder C++ auf vielfältige Weise erfolgen:

### Beispiel 1 – Einfache verschachtelte lokale Variable

*Beispiel*:

```cpp
01: static int* gp = nullptr;
02: static bool b = true;
03: 
04: int test() {
05: 
06:     if (b) {
07:         int x[5] = { };
08:         gp = x + 1;
09:     }
10: 
11:     return *gp;  // Boom!
12: }
```

*Ausgabe*:

```
=================================================================
==10380==ERROR: AddressSanitizer: stack-use-after-scope on address 0x00b1099afcd4 at pc 0x7ff6df9e28cb bp 0x00b1099afc90 sp 0x00b1099afc98
READ of size 4 at 0x00b1099afcd4 thread T0
    #0 0x7ff6df9e28ca in test_09_stack_use_after_scope_01 C:\08_Stack_Use_After_Scope.cpp:22
    #1 0x7ff6df9e2954 in test_08_stack_use_after_scope(void) C:\08_Stack_Use_After_Scope.cpp:78
    #2 0x7ff6df9e34b4 in main C:\Program.cpp:25
    #3 0x7ff6dfa60c38 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff6dfa60b81 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff6dfa60a3d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff6dfa60cad in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

Address 0x00b1099afcd4 is located in stack of thread T0 at offset 36 in frame
    #0 0x7ff6df9e26ff in test_09_stack_use_after_scope_01 C:\08_Stack_Use_After_Scope.cpp:15

  This frame has 1 object(s):
    [32, 52) 'x' <== Memory access at offset 36 is inside this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp, SEH and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-use-after-scope C:\08_Stack_Use_After_Scope.cpp:22 in test_09_stack_use_after_scope_01
Shadow bytes around the buggy address:
  0x00b1099afa00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afa80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afb00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afb80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afc00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x00b1099afc80: 00 00 00 00 00 00 f1 f1 f1 f1[f8]f8 f8 f3 f3 f3
  0x00b1099afd00: f3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099afe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x00b1099aff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==10380==ABORTING
```


### Beispiel 2 – Lambda-Capture

*Beispiel*:

```cpp
01: auto test() {
02: 
03:     std::function<int()> f;
04: 
05:     {
06:         int x = 0;
07:         f = [&x]() {
08:             return x;
09:         };
10:     }
11: 
12:     return f();  // Boom!
13: }
```

*Ausgabe*:

```
=================================================================
==22200==ERROR: AddressSanitizer: stack-use-after-scope on address 0x0087106ffcc0 at pc 0x7ff6a58c2a49 bp 0x0087106ffb00 sp 0x0087106ffb08
READ of size 4 at 0x0087106ffcc0 thread T0
    #0 0x7ff6a58c2a48 in `test_09_stack_use_after_scope_02'::`3'::<lambda_1>::operator() C:\08_Stack_Use_After_Scope.cpp:35
    #1 0x7ff6a58c325e in std::invoke<`test_09_stack_use_after_scope_02'::`3'::<lambda_1> &> C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\include\type_traits:1801
    #2 0x7ff6a58c2bf6 in std::_Func_impl_no_alloc<`test_09_stack_use_after_scope_02'::`3'::<lambda_1>,int>::_Do_call C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\include\functional:1007
    #3 0x7ff6a58c33e2 in std::_Func_class<int>::operator()(void) const C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\include\functional:1057
    #4 0x7ff6a58c2908 in test_09_stack_use_after_scope_02 C:\08_Stack_Use_After_Scope.cpp:39
    #5 0x7ff6a58c2b04 in test_08_stack_use_after_scope(void) C:\08_Stack_Use_After_Scope.cpp:81
    #6 0x7ff6a58c4604 in main C:\Program.cpp:25
    #7 0x7ff6a5941d88 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #8 0x7ff6a5941cd1 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #9 0x7ff6a5941b8d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #10 0x7ff6a5941dfd in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #11 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #12 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

Address 0x0087106ffcc0 is located in stack of thread T0 at offset 128 in frame
    #0 0x7ff6a58c26ff in test_09_stack_use_after_scope_02 C:\08_Stack_Use_After_Scope.cpp:28

  This frame has 3 object(s):
    [32, 96) 'f'
    [48, 52) 'x'
    [64, 72) 'compiler temporary' <== Memory access at offset 128 overflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp, SEH and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-use-after-scope C:\08_Stack_Use_After_Scope.cpp:35 in `test_09_stack_use_after_scope_02'::`3'::<lambda_1>::operator()
Shadow bytes around the buggy address:
  0x0087106ffa00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffa80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffb00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffb80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffc00: 00 00 00 00 00 00 00 00 f1 f1 f1 f1 00 00 00 00
=>0x0087106ffc80: 00 00 00 00 f2 f2 f2 f2[f8]f2 f8 f3 f3 f3 f3 00
  0x0087106ffd00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106ffe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0087106fff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==22200==ABORTING
```

---

## Fehler: *stack-use-after-return* <a name="link12"></a>

Verwendung von Stack-Speicher nach der Rückkehr.

Unter Windows ist die Fehlererkennung für `stack-use-after-return` standardmäßig deaktiviert.
Um sie zu aktivieren, ist die Umgebungsvariable `ASAN_OPTIONS=detect_stack_use_after_return=1` vor der Programmausführung zu setzen.

*Hinweis*:<br />
Mit der Eingabe von *env* im *Developer Command Prompt* erhält man den Zugang zu den Umgebungsvariablen.

*Hinweis*:<br />
Diese Überprüfung erfordert eine Codegenerierung, die durch eine zusätzliche Compiler-Option aktiviert wird: `/fsanitize-address-use-after-return`.
Geben Sie `/fsanitize-address-use-after-return` unter &bdquo;Command Line&rdquo; unten in &bdquo;Additional Options&rdquo; ein.

*Hinweis*:<br />
Nach dem Setzen der Umgebungsvariablen ist Visual Studio neu zu starten, damit die Änderungen wirksam werden.


*Beispiel*:

```cpp
01: static volatile char* x = nullptr;
02: 
03: static void foo() {
04:     char stack_buffer[100]{};
05:     x = &stack_buffer[50];
06: }
07: 
08: static int test_09_stack_use_after_return_01() {
09: 
10:     foo();
11: 
12:     *x = 123; // Boom!
13: 
14:     return (*x == 123);
15: }
```

*Ausgabe*:

```
=================================================================
==22700==ERROR: AddressSanitizer: stack-buffer-underflow on address 0x017080a10000 at pc 0x7ffd44ebc3a4 bp 0x0036164ff880 sp 0x0036164ff020
WRITE of size 100 at 0x017080a10000 thread T0
    #0 0x7ffd44ebc3a3 in __asan_memset D:\a\_work\1\s\src\vctools\asan\llvm\compiler-rt\lib\asan\asan_interceptors_memintrinsics.cpp:70
    #1 0x7ff7189c6dec in foo C:\09_Stack_Use_After_Return.cpp:11
    #2 0x7ff7189c6b50 in test_09_stack_use_after_return_01 C:\09_Stack_Use_After_Return.cpp:17
    #3 0x7ff7189c6c44 in test_09_stack_use_after_return(void) C:\09_Stack_Use_After_Return.cpp:26
    #4 0x7ff7189c70c4 in main C:\Program.cpp:26
    #5 0x7ff718a44878 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #6 0x7ff718a447c1 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #7 0x7ff718a4467d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #8 0x7ff718a448ed in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #9 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #10 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

Address 0x017080a10000 is located in stack of thread T0 at offset 0 in frame
    #0 0x7ff7189c6c5f in foo C:\09_Stack_Use_After_Return.cpp:10

  This frame has 1 object(s):
    [32, 132) 'stack_buffer' <== Memory access at offset 0 partially underflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp, SEH and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-underflow C:\09_Stack_Use_After_Return.cpp:11 in foo
Shadow bytes around the buggy address:
  0x017080a0fd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a0fe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a0fe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a0ff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a0ff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x017080a10000:[f1]f1 f1 f1 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a10080: 04 f2 f3 f3 f3 f3 f3 f3 f3 f3 f3 f3 f3 f3 f3 f3
  0x017080a10100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a10180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a10200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x017080a10280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==22700==ABORTING
```


---

## Fehler: *global-buffer-overflow* <a name="link13"></a>

Globaler Pufferüberlauf.

*Beispiel*:

```cpp
01: struct C {
02:     static int array[10];
03: };
04: 
05: // normal global
06: static int global[10];
07: 
08: // class static
09: int C::array[10];
10: 
11: static int test_10_global_buffer_overflow_01()
12: { 
13:     int first = 1;
14: 
15:     // choose one of these options: 'g', 'c', 'f', 'l'
16:     char mode = { 'l' };
17: 
18:     switch (mode)
19:     {
20:     case 'g': return global[first * 11];         // Boom! simple global
21:     case 'c': return C::array[first * 11];       // Boom! class static
22:     case 'f':
23:     {
24:         static int array[10] = {};
25:         return array[first * 11];                // Boom! function static
26:     }
27:     case 'l':
28:         // literal global ptr created by compiler
29:         const char* str = "0123456789";
30:         return str[first * 11];                  // Boom! .rdata string literal allocated by compiler
31:     }
32:     return 0;
33: }
```

Wir testen das Beispiel an Hand der Zeichenkette:

*Ausgabe*:

```
=================================================================
==20024==ERROR: AddressSanitizer: global-buffer-overflow on address 0x7ff6a48a552b at pc 0x7ff6a4803f72 bp 0x0081fe8ffc40 sp 0x0081fe8ffc48
READ of size 1 at 0x7ff6a48a552b thread T0
    #0 0x7ff6a4803f71 in test_10_global_buffer_overflow_01 C:\10_Global_Buffer_Overflow.cpp:41
    #1 0x7ff6a4803d74 in test_10_global_buffer_overflow(void) C:\10_Global_Buffer_Overflow.cpp:48
    #2 0x7ff6a4803fa4 in main C:\Program.cpp:27
    #3 0x7ff6a4881728 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff6a4881671 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff6a488152d in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff6a488179d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffe8343e956  (C:\Windows\System32\KERNEL32.DLL+0x18002e956)
    #8 0x7ffe843c427b  (C:\Windows\SYSTEM32\ntdll.dll+0x18000427b)

0x7ff6a48a552b is located 0 bytes after global variable '<C++ string literal>' defined in '10_Global_Buffer_Overflow.cpp:40:26' (0x7ff6a48a5520) of size 11
  '<C++ string literal>' is ascii string '0123456789'
SUMMARY: AddressSanitizer: global-buffer-overflow C:\10_Global_Buffer_Overflow.cpp:41 in test_10_global_buffer_overflow_01
Shadow bytes around the buggy address:
  0x7ff6a48a5280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5300: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5380: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5400: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x7ff6a48a5500: 00 00 00 00 00[03]f9 f9 00 00 00 00 00 00 00 00
  0x7ff6a48a5580: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5600: 00 00 00 00 00 00 00 00 00 00 00 05 f9 f9 f9 f9
  0x7ff6a48a5680: 03 f9 f9 f9 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff6a48a5700: 01 f9 f9 f9 f9 f9 f9 f9 04 f9 f9 f9 00 01 f9 f9
  0x7ff6a48a5780: 04 f9 f9 f9 00 02 f9 f9 00 00 00 05 f9 f9 f9 f9
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==20024==ABORTING
```

---

## Literatur <a name="link14"></a>

Eine genaue Beschreibung des Visual Studio Address Sanitizers findet sich in der offiziellen Dokumentation unter

[AddressSanitizer](https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170).

---

[Zurück](./Readme_Secure_Programming.md)

---
