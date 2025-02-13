# Secure Programming

[Zurück](../../Markdown/Readme_Performance_Optimization.md)

---

Incorrect type conversion


## Inhalt

  * [Puffer Über-/Unterlauf (*Buffer Overflows/Underflows*)](#link)
  * [Speicherlecks (*Memory Leaks*)](#link)
  * [Race Conditions](#link)
  * [Arithmetischer Über-/Unterlauf (*Integer overflow/underflow*)](#link)
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

*Type Punnings* können Abhängigkeitsprobleme für alle Daten verursachen, auf die über beide Zeiger zugegriffen wird.&rdquo;




---

#### Quellcode

[*BestPractices.cpp*](BestPractices.cpp)<br />

---

## Tool *Cppcheck*

*Cppcheck* ist ein statisches Analysetool für C/C++-Quellcode.
Es bietet eine einzigartige Codeanalyse zum Erkennen von Fehlern und konzentriert sich auf das Erkennen undefinierten Verhaltens
und gefährlicher Codekonstrukte.

Das Ziel besteht darin, möglichst wenige Fehlalarme zu haben.
Cppcheck ist so konzipiert, dass es C/C++-Code analysieren kann,
auch wenn er eine nicht standardmäßige Syntax aufweist (was häufig bei Embedded C++ Projekten der Fall ist).

Mehrere Möglichkeiten zum Download finden sich [hier](https://cppcheck.sourceforge.io).

<img src="CppCheck_01.png" width="600">

*Abbildung* 1: [https://cppcheck.sourceforge.io](https://cppcheck.sourceforge.io) Website.

*Cppcheck* ist sowohl als Open Source ([hier](https://cppcheck.sourceforge.io) als auch als *Cppcheck Premium Paket*
mit erweiterter Funktionalität verfügbar.
Weitere Informationen und Kaufoptionen für die kommerzielle Version finden Sie unter [www.cppcheck.com](www.cppcheck.com).


---

## Visual Studio Integration Add-In für Cppcheck

Um *Cppcheck* mit Visual Studio zusammen betreiben zu können,
grift man am besten auf
das [Visual Studio Integration Add-In](https://github.com/VioletGiraffe/cppcheck-vs-addin) für Cppcheck zurück.

<img src="CppCheck_Extension_01.png" width="700">

*Abbildung* 2: [https://github.com/VioletGiraffe/cppcheck-vs-addin](https://github.com/VioletGiraffe/cppcheck-vs-addin) Website.

*Hinweis*:

Das Add-In stellt keine Installation des *Cppcheck*-Tools -bereit.
Bevor man dieses Add-In installiert, muss eine *Cppcheck*-Installation erfolgt sein.

Das Add-In fügt an diversen Stellen im Visual Studio neue Menüeinträge hinzu,
um das aktuelle Projekt überprüfen zu können:

<img src="CppCheck_Extension_03.png" width="300">

*Abbildung* 3: Erweiterungen im Menü &bdquo;*Tools*&rdquo;

Zum Abschluss finden Sie in *Abbildung* 4 das *Settings*-Dialogfenster vor:

<img src="CppCheck_Extension_04.png" width="600">

*Abbildung* 4: Spezifische Parametrierung des *Cppcheck*-Tools.

In diesem Fenster kann man spezfische Einstellungen vornehmen,
welche Meldungen man haben möchte &ndash; und welche vielleicht auch nicht.

---

## Visual Studio Address Sanitizer

Beschreibung:

[hier](https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170).

Fehlermeldung;

https://stackoverflow.com/questions/76781556/visual-studio-22-asan-failed-to-use-and-restart-external-symbolizer


Visual Studio 22 - Asan - Failed to use and restart external symbolizer

Aufheben:


```
Hello!
=================================================================
==16452==ERROR: AddressSanitizer: global-buffer-overflow on address 0x7ff604745af0 at pc 0x7ff60472111d bp 0x00576738fa10 sp 0x00576738fa18
WRITE of size 4 at 0x7ff604745af0 thread T0
    #0 0x7ff60472111c in SecureProgrammingPractices::UsingAddressSanitizer::test_address_sanitizer C:\Development\GitRepositoryCPlusPlus\Cpp_Clean_Performant_Code\Clean_Performant_Code\Secure_Programming\SecureProgramming.cpp:90
    #1 0x7ff604721184 in secure_programming_practices(void) C:\Development\GitRepositoryCPlusPlus\Cpp_Clean_Performant_Code\Clean_Performant_Code\Secure_Programming\SecureProgramming.cpp:107
    #2 0x7ff604721054 in main C:\Development\GitRepositoryCPlusPlus\Cpp_Clean_Performant_Code\Clean_Performant_Code\Secure_Programming\Program.cpp:9
    #3 0x7ff60472dcc8 in invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78
    #4 0x7ff60472dc11 in __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288
    #5 0x7ff60472dacd in __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:330
    #6 0x7ff60472dd3d in mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:16
    #7 0x7ffce42d7373  (C:\WINDOWS\System32\KERNEL32.DLL+0x180017373)
    #8 0x7ffce521cc90  (C:\WINDOWS\SYSTEM32\ntdll.dll+0x18004cc90)

0x7ff604745af0 is located 0 bytes after global variable 'SecureProgrammingPractices::x' defined in 'SecureProgramming.cpp:83:15' (0x7ff604745960) of size 400
SUMMARY: AddressSanitizer: global-buffer-overflow C:\Development\GitRepositoryCPlusPlus\Cpp_Clean_Performant_Code\Clean_Performant_Code\Secure_Programming\SecureProgramming.cpp:90 in SecureProgrammingPractices::UsingAddressSanitizer::test_address_sanitizer
Shadow bytes around the buggy address:
  0x7ff604745800: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745880: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745900: 00 00 00 00 00 00 00 00 04 f9 f9 f9 00 00 00 00
  0x7ff604745980: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745a00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x7ff604745a80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00[f9]f9
  0x7ff604745b00: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 00 00 00 00
  0x7ff604745b80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745c00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745c80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7ff604745d00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
==16452==ABORTING

C:\Development\GitRepositoryCPlusPlus\Cpp_Clean_Performant_Code\x64\Debug\Secure_Programming.exe (process 16452) exited with code 1 (0x1).
Press any key to close this window . . .
```

---

## Literatur <a name="link6"></a>

---

[Zurück](../../Markdown/Readme_Performance_Optimization.md)

Die Anregungen zum konzeptionellen Beispiel finden Sie unter

[C++ Performance Optimization: Best Practices](https://hackernoon.com/c-performance-optimization-best-practices)

---
