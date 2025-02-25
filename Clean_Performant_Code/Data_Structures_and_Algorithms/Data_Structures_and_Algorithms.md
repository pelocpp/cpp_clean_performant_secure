# Daten Strukturen und Algorithmen

[Zurück](../../Readme.md)

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

CPU-Cache-Speicher ist eine Art temporärer Datenspeicher, der sich auf dem Prozessor befindet.
Er wird verwendet, um die Verarbeitungseffizienz der CPU zu steigern, indem er kleine, oft angeforderte Datenbits bereithält, die mit hoher Geschwindigkeit abgerufen werden können.
Cache-Speicher besteht aus verschiedenen Speicherebenen. Diese Ebenen werden üblicherweise als L1, L2, L3 und gelegentlich L4 bezeichnet und unterscheiden sich in Position, Geschwindigkeit und Größe.

Um besser zu verstehen, warum die CPU ihren eigenen Speichercache benötigt,
müssen wir uns die Entwicklung von Prozessoren und RAM ansehen.
Der Geschwindigkeitsunterschied zwischen Prozessor und RAM war in der Vergangenheit minimal.
Es bestand kaum Besorgnis darüber, dass die Verarbeitung durch den Speicher verlangsamt werden könnte.
Mit der Zeit übertrifft die Betriebsgeschwindigkeit von Zentralprozessoren die RAM-Geschwindigkeit bei weitem.

Es bestand folglich eine Notwendigkeit, diese Diskrepanz irgendwie zu verringern.
Die Antwort war Cache-Speicher.

Cache-Speicher ist extrem schnell (oft 10- bis 100-mal schneller als DRAM) und befindet sich physisch nahe an den Prozessorkernen.
Moderne, schnelle Prozessoren werden nicht dadurch ausgebremst, dass sie Daten aus dem relativ langsamen Systemspeicher anfordern müssen, da sie die Daten stattdessen aus dem Cache abrufen können.

In der Regel arbeiten Prozessoren mit mehrstufigen Caches, die unterschiedlich groß und schnell sind. Je näher der Cache am Rechenkern ist, desto kleiner und schneller arbeitet er.

  * L1-Cache (klein, sehr schnell)
  * L2-Cache
  * L3-Cache (groß, langsam)


### L1-Cache / First-Level-Cache <a name="link2"></a>

Der L1-Cache, manchmal auch Primärcache genannt, ist die kleinste und schnellste Speicherebene.

Im L1-Cache werden die am häufigsten benötigten Befehle und Daten zwischengespeichert, damit möglichst wenige Zugriffe auf den langsamen Arbeitsspeicher erforderlich sind.
Dieser Cache vermeidet Verzögerungen und hilft dabei den Rechenkern optimal auszulasten.

Die Bedeutung des L1-Caches wächst mit höherer Geschwindigkeit eines Prozessors.

Ein L1-Cache ist in zwei weitere Ebenen unterteilt:

  * L1-I (*Instructions* &ndash; Anweisungen) und
  * L1-D (*Data* &ndash; Daten).

Der L1-Anweisungscache verarbeitet Informationen, die dem Prozessor zugeführt werden, während der Datencache Informationen enthält, die in den Hauptspeicher geschrieben werden sollen.

### L2-Cache / Second-Level-Cache <a name="link2"></a>

L2-Cache ist ein sekundärer Speichercache, der ebenfalls in jeden einzelnen Kern der CPU eingebettet ist.
Er hat fast immer mehr Speicherplatz als ein L1-Cache, arbeitet aber langsamer, aber immer noch viel schneller als die RAM-Geschwindigkeit.

Jeder Kern des Prozessors verfügt über einen eigenen integrierten L1-Cache.


### L3-Cache / Third-Level-Cache <a name="link2"></a>

Anstatt in jeden CPU-Kern eingebettet zu sein, fungiert der Level-3-Cache als gemeinsam genutzter Speicherpool,
auf den der gesamte Prozessor zugreifen kann. Er ist viel langsamer als L1- und L2-Cache, vielleicht nur doppelt so schnell wie RAM,
ist aber die größte aller drei Speicherebenen.

Wenn die CPU die benötigten Daten nicht im Cache-Speicher findet, muss sie die Daten stattdessen aus dem langsameren Systemspeicher anfordern.
Dies wird als *Cache-Fehler* (*cache miss*) bezeichnet.
Die Einführung des L3-Cache verringerte die Wahrscheinlichkeit eines Fehlers und trug somit zur Leistungssteigerung bei.

<img src="L1_L2_L3_Cache.png" width="350">

*Abbildung* 1: Cache-Größen auf meinem Rechner

<img src="cpp_l1_l2_l3_cache.svg" width="500">

*Abbildung* 2: Übliche CPU-Architektur mit L1-, L2- und L3-Cache.




### Auswirkungen von *Cache Lines*

Beim Zugriff auf ein Byte im Speicher holt die CPU nicht nur das angeforderte Byte,
sondern immer eine so genannte *Cache Lines*.

Häufig besteht eine *Cache Lines* aus 64 Bytes.
Die verschiedenen Caches zwischen der CPU und dem Hauptspeicher (L1-, L2- und L3-Cache) tauschen 64-Byte-Blöcke statt einzelner Bytes aus.




Einige wichtige Faktoren, die die Leistung eines Computerprogramms, das den CPU-Cache nutzt, beeinträchtigen können, besteht darin,
in einem Programm zwischenm verschiedenen *Cache Lines* &bdquo;hin- und her zu springen&rdquo;,
an Stelle die Daten einer einzelnen Cache Line im Stück auszuwerten (solange das Programm eine derartige Funktionalität natürlich hergibt)


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

Beide Ausführungen beziehen sich auf den *Release*-Mode.


---

## Literatur <a name="link6"></a>

Ein interessanter Artikel zum Thema *Demystifying CPU Caches with Examples*
findet sich [hier](https://mecha-mind.medium.com/demystifying-cpu-caches-with-examples-810534628d71).

Eine *Gallery of Processor Cache Effects* beschreibt *Igor Ostrovsky*
in seinem [Blog](https://igoro.com/archive/gallery-of-processor-cache-effects/).




---



Die Anregungen zum Beispiel finden Sie unter

[Unlocking the Power of `std::hash` in C++ Programming](https://johnfarrier.com/unlocking-the-power-of-stdhash-in-c-programming/)


---

[Zurück](../../Readme.md)

---
