# Daten Strukturen und Algorithmen

[Zur�ck](../../Readme.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Hashing in C++ Standard Library](#link1)
  * [XXX](#link1)
  * [Literatur](#link7)

---

#### Quellcode

[*DataStructuresAndAlgorithms.cpp*](DataStructuresAndAlgorithms.cpp)<br />

---

Fehlemeldung


'std::_Uhash_compare<_Kty,_Hasher,_Keyeq>::_Uhash_compare(const std::_Uhash_compare<_Kty,_Hasher,_Keyeq> &)':
attempting to reference a deleted function

---

## Allgemeines <a name="link1"></a>

---

## Metaprogrammierung mit Templates <a name="link2"></a>

*Beispiel*:

```cpp
```

---

ENTFERNEN:

https://www.elektronik-kompendium.de/sites/com/0309291.htm

https://www.howtogeek.com/891526/l1-vs-l2-vs-l3-cache




## CPU-Cache-Speicher <a name="link2"></a>

CPU-Cache-Speicher ist eine Art tempor�rer Datenspeicher, der sich auf dem Prozessor befindet.
Er wird verwendet, um die Verarbeitungseffizienz der CPU zu steigern, indem er kleine, oft angeforderte Datenbytes bereith�lt, die mit hoher Geschwindigkeit abgerufen werden k�nnen.

Cache-Speicher besteht aus verschiedenen Speicherebenen. Diese Ebenen werden �blicherweise als L1, L2, L3 und gelegentlich als L4 bezeichnet und unterscheiden sich in Position, Geschwindigkeit und Gr��e.

Um besser zu verstehen, warum die CPU ihren eigenen Speichercache ben�tigt,
m�ssen wir uns die Entwicklung von Prozessoren und RAM ansehen.
Der Geschwindigkeitsunterschied zwischen Prozessor und RAM war in der Vergangenheit minimal.
Es bestand kaum Besorgnis dar�ber, dass die Verarbeitung durch den Speicher verlangsamt werden k�nnte.
Dies ist heutzutage nicht mehr der Fall: Aktuell �bertrifft die Betriebsgeschwindigkeit von Zentralprozessoren die RAM-Geschwindigkeit bei weitem.
Es bestand folglich eine Notwendigkeit, diese Diskrepanz irgendwie zu verringern.
Die Antwort war Cache-Speicher.

Cache-Speicher ist extrem schnell (oft 10- bis 100-mal schneller als DRAM) und befindet sich physisch nahe an den Prozessorkernen.
Moderne, schnelle Prozessoren werden nicht dadurch ausgebremst, dass sie Daten aus dem relativ langsamen Systemspeicher anfordern m�ssen, da sie die Daten stattdessen aus dem Cache abrufen k�nnen.

In der Regel arbeiten Prozessoren mit mehrstufigen Caches, die unterschiedlich gro� und schnell sind. Je n�her der Cache am Rechenkern ist,
desto kleiner und schneller arbeitet er:

  * L1-Cache (klein, sehr schnell)
  * L2-Cache
  * L3-Cache (gro�, langsam)


### L1-Cache / First-Level-Cache

Der L1-Cache, manchmal auch Prim�rcache genannt, ist die kleinste und schnellste Speicherebene.

Im L1-Cache werden die am h�ufigsten ben�tigten Befehle und Daten zwischengespeichert, damit m�glichst wenige Zugriffe auf den langsamen Arbeitsspeicher erforderlich sind.
Dieser Cache vermeidet Verz�gerungen und hilft dabei den Rechenkern optimal auszulasten.

Ein L1-Cache ist in zwei weitere Ebenen unterteilt:

  * L1-I (*Instructions* &ndash; Anweisungen) und
  * L1-D (*Data* &ndash; Daten).

Der L1-Anweisungscache verarbeitet Informationen, die dem Prozessor zugef�hrt werden, w�hrend der Datencache Informationen enth�lt, die in den Hauptspeicher geschrieben werden sollen.

Jeder Kern des Prozessors verf�gt �ber einen eigenen integrierten L1-Cache.

### L2-Cache / Second-Level-Cache

L2-Cache ist ein sekund�rer Speichercache, der ebenfalls in jeden einzelnen Kern der CPU eingebettet ist.
Er hat fast immer mehr Speicherplatz als ein L1-Cache, arbeitet aber langsamer, aber immer noch viel schneller als die RAM-Geschwindigkeit.

### L3-Cache / Third-Level-Cache 

Anstatt in jeden CPU-Kern eingebettet zu sein, fungiert der Level-3-Cache als gemeinsam genutzter Speicherpool,
auf den der gesamte Prozessor zugreifen kann. Er ist viel langsamer als L1- und L2-Cache, vielleicht nur doppelt so schnell wie RAM,
ist aber die gr��te aller drei Speicherebenen.

In *Abbildung* 1 finden Sie die Cache-Gr��en auf meinem Rechner auf meinem Rechner vor:

<img src="L1_L2_L3_Cache.png" width="350">

*Abbildung* 1: Cache-Gr��en auf meinem Rechner

### Cache-Fehler (*Cache Misses*)

Wenn die CPU die ben�tigten Daten nicht im Cache-Speicher findet, muss sie die Daten stattdessen aus dem langsameren Systemspeicher anfordern.
Dies wird als *Cache-Fehler* (*cache miss*) bezeichnet.
Die Einf�hrung des L3-Cache verringerte die Wahrscheinlichkeit eines Fehlers und trug somit zur Leistungssteigerung bei.

<img src="cpp_l1_l2_l3_cache.svg" width="500">

*Abbildung* 2: �bliche CPU-Architektur mit L1-, L2- und L3-Cache.


### *Cache Lines*

Beim Zugriff auf ein Byte im Speicher holt die CPU nicht nur das angeforderte Byte,
sondern immer eine so genannte *Cache Line*.

H�ufig besteht eine *Cache Line* aus 64 Bytes.
Die verschiedenen Caches zwischen der CPU und dem Hauptspeicher (L1-, L2- und L3-Cache) tauschen 64-Byte-Bl�cke statt einzelner Bytes aus.

Einige wichtige Faktoren, die die Leistung eines Computerprogramms, das den CPU-Cache nutzt, beeintr�chtigen k�nnen, besteht darin,
in einem Programm zwischen verschiedenen *Cache Lines* &bdquo;hin- und her zu springen&rdquo;,
an Stelle die Daten einer einzelnen Cache Line im St�ck auszuwerten (solange das Programm eine derartige Funktionalit�t nat�rlich hergibt).

Die Gr��e einer Cache Line ist in C++ einfach bestimmbar:

*Beispiel*:

```cpp
01: void test() {
02:     constexpr auto cachelineSize = std::hardware_destructive_interference_size;
03:     std::println("Cache Line Size: {}", cachelineSize);
04: }
```

*Ausgabe*: 

```
Cache Line Size: 64
```

Wir berechnen nun die Gr��e des L1 Caches auf meinem Rechner (Windows Betriebssystem).
Dazu kommen einige Win32-API Betriebssystemfunktionen ins Spiel: `GetModuleHandle`, `GetProcAddress` und `GetLogicalProcessorInformation`.

*Beispiel*:

```cpp
01: void test() {
02: 
03:     typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
04: 
05:     auto handle = GetModuleHandle(L"kernel32");
06: 
07:     auto glpi = (LPFN_GLPI) GetProcAddress(handle, "GetLogicalProcessorInformation");
08:     if (glpi == NULL)
09:         return;
10: 
11:     DWORD bufferBytes = 0;
12:     int cacheSize = 0;
13: 
14:     // calculate buffer length
15:     BOOL ret = glpi(0, &bufferBytes);
16: 
17:     std::size_t size = bufferBytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
18:             
19:     SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = new SYSTEM_LOGICAL_PROCESSOR_INFORMATION[size];
20:             
21:     // retrieve array of SYSTEM_LOGICAL_PROCESSOR_INFORMATION structures
22:     ret = glpi(buffer, &bufferBytes);
23: 
24:     for (std::size_t i = 0; i < size; i++)
25:     {
26:         if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
27:         {
28:             cacheSize = (int)buffer[i].Cache.Size;
29:             break;
30:         }
31:     }
32: 
33:     delete[] buffer;
34: 
35:     auto cacheSizeKB = cacheSize / 1024;
36:     std::println("L1 Cache Size: {}", cacheSizeKB);
37: }
```

*Ausgabe*: 

```
L1 Cache Size: 32 kB
```

Wie passt das Ergbnis mit *Abbildung* 1 zusammen? Ganz einfach,
mein Rechner hat 8 Kerne, jeder Kern hat einen L1-Anweisungs- und Datencache.
Macht zusammen 8 * 2 * 32 Kb = 512 kB.


WEITER: Cache Misses
















*Beispiel*:

```cpp
01: constexpr auto cachelineSize = std::hardware_destructive_interference_size;
02: 
03: constexpr auto capacityL1CacheSize = 32768; // L1 data cache size
04:         
05: constexpr auto Size = capacityL1CacheSize / sizeof(int);
06: 
07: using MatrixType = std::array<std::array<size_t, Size>, Size>;
08: 
09: static MatrixType matrix;
10: 
11: static auto initMatrix(MatrixType& matrix) {
12: 
13:     ScopedTimer watch{};
14: 
15:     size_t value{};
16: 
17:     for (size_t i{}; i != Size; ++i) {
18:         for (size_t j{}; j != Size; ++j) {
19:             matrix[i][j] = value++;      // no "cache thrashing" // exchange indices i and j
20:         }
21:     }
22: }
23: 
24: static void test_cache_thrashing() {
25:     initMatrix(matrix);
26: }
```

*Ausgabe*: Ohne &bdquo;Cache Thrashing&bdquo;

```
Elapsed time: 87 [milliseconds]   87  // 612
```

*Ausgabe*: Mit &bdquo;Cache Thrashing&bdquo;

```
Elapsed time: 612 [milliseconds]
```

Beide Ausf�hrungen beziehen sich auf den *Release*-Mode.


---

## Literatur <a name="link6"></a>

Ein interessanter Artikel zum Thema *Demystifying CPU Caches with Examples*
findet sich [hier](https://mecha-mind.medium.com/demystifying-cpu-caches-with-examples-810534628d71).

Eine *Gallery of Processor Cache Effects* beschreibt *Igor Ostrovsky*
in seinem [Blog](https://igoro.com/archive/gallery-of-processor-cache-effects/).




---


Die Anregungen zur Berechnung der L1 Cache Gr��e finden Sie unter dem Github Gist

[Get L1 data cache size on most operating systems](https://gist.github.com/kimwalisch/16c34ae16447b245464a)



---

[Zur�ck](../../Readme.md)

---
