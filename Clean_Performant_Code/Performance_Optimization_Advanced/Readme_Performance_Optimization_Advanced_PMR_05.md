# Eine Stolperfalle im Umfeld der polymorphen Speicherverwaltung

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

#### Quellcode

[*PMR_05.cpp*](PMR_05.cpp)<br />

---

### Allgemeines <a name="link1"></a>

In diesem Abschnitt wollen wir ein Anwendungsbeispiel für den Einsatz eines PMR-Containers betrachten.

## Ein Anwendungsfall

Schreiben Sie eine Funktion `splitToDigits`, die eine ganze Zahl in ihre einzelnen Ziffern zerlegt.
Die Ziffern der Zahl sind der Reihe nach in einem dynamisch allokierten Speicherbereich abzulegen:

```cpp
std::pmr::vector<std::uint8_t> splitToDigits(std::size_t number);
```

Offensichtlich kennt die Funktion die Anzahl der Ziffern nicht von vorneherein,
ein STL-Container mit einer variablen Anzahl von Elementen wie ein Objekt der Klasse `std::vector` bietet sich an.

## Erster Lösungsansatz &ndash; mit Absturz

Die Aufgabenstellung ist prädestiniert für einen STL Container,
dessen Daten in einem `std::pmr::monotonic_buffer_resource`-Objekt abgelegt sind.

Während des Zerlegens der Zahl in ihre einzelnen Ziffern benötigt man eine Datenstruktur, deren Umfang dynamisch erweiterbar ist.

Das Freigeben von Daten während der Berechnung hingegen spielt keine Rolle. Dies ist ein Pluspunkt des `std::pmr::monotonic_buffer_resource`-Objekts.

Der erste Lösungsansatz in der Realisierung einer `splitToDigits`-Funktion könnte so aussehen:

```cpp
01: std::pmr::vector<std::uint8_t> splitToDigits(std::size_t number)
02: {
03:     constexpr std::size_t NumBytes{ 32 };
04: 
05:     std::array<std::uint8_t, NumBytes> memory{};
06: 
07:     std::pmr::monotonic_buffer_resource resource{
08:         memory.data(), memory.size(), std::pmr::null_memory_resource()
09:     };
10:   
11:     std::pmr::vector<std::uint8_t> digits{ &resource };
12: 
13:     do {
14:         std::uint8_t digit = number % 10;
15:         number /= 10;
16: 
17:         digits.push_back(digit);   
18: 
19:     } while (number != 0);
20: 
21:     std::reverse(digits.begin(), digits.end());
22: 
23:     return digits;
24: }
```

Interessanterweise funktioniert ein Aufruf der Funktion `splitToDigits` noch &bdquo;prinzipiell&rdquo;, zum Beispiel bei folgendem Testrahmen:

```cpp
01: void test()
02: {
03:     try {
04:         std::size_t number{ 12345 };
05: 
06:         std::pmr::vector<std::uint8_t> digits{ splitToDigits(number) };
07: 
08:         for (std::size_t i{}; auto digit : digits) {
09:             std::println("{}: {}", i, digit);
10:             ++i;
11:         }
12:     }
13:     catch (std::exception ex) {
14:         std::println("std::exception: {}", ex.what());
15:     }
16: }
```

Die Ausgabe offenbart bereits erste Schwächen des Ergebnisses, sprich es werden falsche Ziffern ausgegeben:

```
0: 1
1: 0
2: 0
3: 0
4: 0
```

Das Grauen offenbart sich erst zu einem späteren Zeitpunkt, wenn das Programm mit einem *Access-Violation* Fehlercode abstürzt:

```
C:\Development\Cpp_PMR.exe (process 26128) exited with code -1073741819 (0xc0000005).
```

Erkennen Sie das Problem in der Realisierung?

## Zweiter Lösungsansatz &ndash; mit Fehlerkorrektur

Eigentlich muss man zugeben, dass es sich bei dem Absturz um einen höchst trivialen Fehler handelt,
man sieht nur vor lauter Bäumen, sprich vor lauter &bdquo;*Ressourcen*&rdquo;- und &bdquo;*Allokatoren*&rdquo;-Objekten, den Wald nicht:
Das Speicherressourcenobjekt `resource` (vom Typ `std::pmr::monotonic_buffer_resource`) ist ein *lokales* Objekt der Funktion `splitToDigits`,
dessen Adresse aber in einem zweiten Objekt `digits` (vom Typ `std::pmr::vector`) abgelegt wird.

Bis hierher stellt das alles kein Problem dar, aber das Kopieren (oder Verschieben) dieses `digits`-Objekts hin zum Aufrufer
ist problematisch, da dieses dann einen *Dangling*-Zeiger im Bauch hat.

Dadurch haben wir die Erklärung des &bdquo;*verspäteten*&rdquo; Absturzes im Programm gefunden:
Erst der Zugriff auf diesen ungültigen Zeiger führt schließlich zum Absturz.

Wie kann man das Problem beheben: Man muss zunächst einmal konstatieren, dass das Arbeiten mit beispielsweise `std::pmr::vector`-Objekten
in einer realen Anwendung möglicherweise nicht ganz so trivial ist, da neben dem Container-Objekt auch das Speicherressourcenobjekt *gültig* sein muss.
So gesehen müssen wir uns jetzt um zwei Objekte kümmern, die bis zum Abschluss eines Funktionsaufrufs gültig sein müssen.


```cpp
01: std::pmr::vector<std::uint8_t> splitToDigits(std::size_t number, std::pmr::memory_resource* res)
02: {
03:     std::pmr::vector<std::uint8_t> digits{ res };
04: 
05:     do {
06:         std::uint8_t digit = number % 10;
07:         number /= 10;
08: 
09:         digits.push_back(digit);
10:     } while (number != 0);
11: 
12:     std::reverse(digits.begin(), digits.end());
13: 
14:     return digits;
15: }
```

Diese zweite Realisierung einer `splitToDigits`-Funktion umgeht zunächst das zuvor geschilderte Problem,
es wird einfach ein `std::pmr::memory_resource`-Zeiger an die Funktion durchgeschleust.
Ein passendes Objekt muss nun beim Aufruf der `splitToDigits`-Funktion bereitstehen &ndash; verbunden mit einer Lebenszeit,
die mindestens so lang gültig ist wie das korrespondierende Ergebnisobjekt vom Typ `std::pmr::vector<std::uint8_t>`:


```cpp
01: void test()
02: {
03:     constexpr std::size_t NumBytes{ 32 };
04: 
05:     std::array<std::uint8_t, NumBytes> memory{};
06: 
07:     std::pmr::monotonic_buffer_resource resource{
08:         memory.data(), memory.size(), std::pmr::null_memory_resource()
09:     };
10: 
11:     std::size_t number{ 12345 };
12: 
13:     std::pmr::vector<std::uint8_t> digits{ splitToDigitsCorrect(number, &resource) };
14: 
15:     for (std::size_t i{}; auto digit : digits) {
16:         std::println("{}: {}", i, digit);
17:         ++i;
18:     }
19: }
```

Das Programm funktioniert nun wie erwartet, auch das Ergebnis ist korrekt:

```
0: 1
1: 2
2: 3
3: 4
4: 5
```

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

