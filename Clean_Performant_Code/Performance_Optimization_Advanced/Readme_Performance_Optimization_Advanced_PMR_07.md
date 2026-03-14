# PMR STL-Container mit unterlagerten PMR Objekten

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Erstes Beispiel: `std::pmr::vector<std::pmr::string>` mit &bdquo;kurzen&rdquo; Zeichenketten](#link2)
  * [Zweites Beispiel: `std::pmr::vector<std::pmr::string>` mit &bdquo;langen&rdquo; Zeichenketten](#link3)
  * [Erste Stolperfalle: Klasse `std::pmr::vector<std::pmr::string>` und Methode `push_back`](#link4)
  * [Zweite Stolperfalle: Klasse `std::pmr::vector<std::pmr::string>` und Initialisierungslisten](#link5)
  * [Literatur](#link6)

---

#### Quellcode

[*PMR_07.cpp*](PMR_07.cpp)<br />

---

### Allgemeines <a name="link1"></a>

STL-Container mit polymorphen Ressourcen haben wir bereits angesprochen,
z. B. Klassen der Gestalt `std::pmr::vector<std::uint8_t>`. Wie sieht es aber aus,
wenn die Elemente der Vektor-Klasse ebenfalls dynamischen Speicher belegen?
Ich möchte im Folgenden ein `std::vector`-Objekt betrachten, das Zeichenketten (`std::string`) beherbergt.
Ein erster Ansatz der Gestalt

```cpp
std::pmr::vector<std::string> strings;
```

ist mit dieser Definition falsch, da dann die Zeichenketten auf ihren eigenen Allokator zurückgreifen würden.
Damit sind wir bei folgender Definition angekommen:

```cpp
std::pmr::vector<std::pmr::string> strings;
```

Auf diese Weise kann man erreichen, dass der polymorphe Allokator von `std::pmr::vector` an die von ihm
verwalteten `std::pmr::string`-Elemente durchgereicht wird. Das wollen wir nun näher betrachten.

## Erstes Beispiel: `std::pmr::vector<std::pmr::string>` mit &bdquo;kurzen&rdquo; Zeichenketten <a name="link2"></a>

Die Klasse `std::string` hat a priori nicht nur ihren eigenen Allokator, für &bdquo;kurze&rdquo; Zeichenketten 
hält sie auch noch eine Überraschung bereit: Die Zeichen kurzer Zeichenketten werden in einem internen Puffer der Klasse `std::string` selbst abgelegt,
ein zeitaufwändiger Zugriff auf den Heap entfällt dadurch.

*Bemerkung*:<br />
Die Frage, was wir unter &bdquo;kurzen Zeichenketten&rdquo; verstehen, kann man unter
dem Schlagwort SSO (&bdquo;*Short String Optimization*&rdquo;) nachlesen.

Das Ganze wollen wir nun an einem Beispiel näher betrachten


```cpp
01: void test()
02: {
03:     constexpr std::size_t NumBytes = 160;
04: 
05:     std::array<std::uint8_t, NumBytes> memory{};
06: 
07:     std::pmr::monotonic_buffer_resource resource{
08:         memory.data(), memory.size(), std::pmr::null_memory_resource()
09:     };
10: 
11:     std::fill(memory.begin(), memory.end(), std::numeric_limits<std::uint8_t>::max());
12: 
13:     std::pmr::vector<std::pmr::string> strings{ &resource };
14: 
15:     strings.reserve(2);
16: 
17:     strings.emplace_back("A lazy dog");
18:     strings.emplace_back("Another dog");
19:     dumpBuffer(std::span{ memory });
20: }
```

*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X2704AFF540
Bytes:  160
---------------------------------------------------------------------------
0000 | 50 f6 af 04 27 00 00 00  00 00 00 00 00 00 00 00 | P...'... ........
0010 | f8 f5 af 04 27 00 00 00  b0 f5 af 04 27 00 00 00 | ....'... ....'...
0020 | 41 20 6c 61 7a 79 20 64  6f 67 00 00 00 00 00 00 | A lazy d og......
0030 | 0a 00 00 00 00 00 00 00  0f 00 00 00 00 00 00 00 | ........ ........
0040 | f8 f5 af 04 27 00 00 00  c0 f5 af 04 27 00 00 00 | ....'... ....'...
0050 | 41 6e 6f 74 68 65 72 20  64 6f 67 00 00 00 00 00 | Another  dog.....
0060 | 0b 00 00 00 00 00 00 00  0f 00 00 00 00 00 00 00 | ........ ........
0070 | 58 f5 af 04 27 00 00 00  00 00 00 00 00 00 00 00 | X...'... ........
0080 | 88 f5 af 04 27 00 00 00  00 00 00 00 00 00 00 00 | ....'... ........
0090 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
```

Die beiden Zeichenketten `A lazy dog` und `Another dog` sind &bdquo;vergleichsweise&rdquo; kurz,
sie werden im `std::string`-Objekt direkt abgelegt. Das können wir anhand der Ausgabe
erkennen. Außerdem finden wir die beiden Längen `0a` (10 dezimal) und `0b` (11 dezimal) im Speicher vor.

## Zweites Beispiel: `std::pmr::vector<std::pmr::string>` mit &bdquo;langen&rdquo; Zeichenketten <a name="link3"></a>

Wie sieht es mit längeren Zeichenketten aus? Dazu tauschen wir die beiden Argumente der `emplace_back`-Methode wie folgt aus:

```cpp
strings.emplace_back("The quick brown fox jumps over the lazy dog");               // 43 chars
strings.emplace_back("Lorem ipsum dolor sit amet, consectetur adipiscing elit");   // 55 chars 
```


*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X96FBF6F140
Bytes:  256
---------------------------------------------------------------------------
0000 | b0 f2 f6 fb 96 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
0010 | 58 f2 f6 fb 96 00 00 00  b0 f1 f6 fb 96 00 00 00 | X....... ........
0020 | c0 f1 f6 fb 96 00 00 00  00 00 00 00 00 00 00 00 | ........ ........  <== Anfangsadresse 0x96 FB F6 F1 C0
0030 | 2b 00 00 00 00 00 00 00  2f 00 00 00 00 00 00 00 | +....... /.......  <== 0x2b entspricht 43 dezimal
0040 | 58 f2 f6 fb 96 00 00 00  f0 f1 f6 fb 96 00 00 00 | X....... ........
0050 | 00 f2 f6 fb 96 00 00 00  00 00 00 00 00 00 00 00 | ........ ........  <== Anfangsadresse 0x96 FB F6 F1 C0
0060 | 37 00 00 00 00 00 00 00  3f 00 00 00 00 00 00 00 | 7....... ?.......  <== 0x37 entspricht 55 dezimal 
0070 | 58 f1 f6 fb 96 00 00 00  00 00 00 00 00 00 00 00 | X....... ........
0080 | 54 68 65 20 71 75 69 63  6b 20 62 72 6f 77 6e 20 | The quic k brown
0090 | 66 6f 78 20 6a 75 6d 70  73 20 6f 76 65 72 20 74 | fox jump s over t
00a0 | 68 65 20 6c 61 7a 79 20  64 6f 67 00 ff ff ff ff | he lazy  dog.....
00b0 | 88 f1 f6 fb 96 00 00 00  00 00 00 00 00 00 00 00 | ........ ........
00c0 | 4c 6f 72 65 6d 20 69 70  73 75 6d 20 64 6f 6c 6f | Lorem ip sum dolo
00d0 | 72 20 73 69 74 20 61 6d  65 74 2c 20 63 6f 6e 73 | r sit am et, cons
00e0 | 65 63 74 65 74 75 72 20  61 64 69 70 69 73 63 69 | ectetur  adipisci
00f0 | 6e 67 20 65 6c 69 74 00  ff ff ff ff ff ff ff ff | ng elit. ........
---------------------------------------------------------------------------
```

Jetzt können wir etwas Schulmathematik betreiben. Wir beginnen damit,
die Anfangsadressen der beiden Zeichenketten an Hand der Ausgabe zu berechnen:

```
0X96FBF6F140 + 0x80 = 0x96 FB F6 F1 C0
```

und

```
0X96FBF6F140 + 0xC0 = 0x96 FB F6 F2 00
```

Tatsächlich finden sich beide Adressen im Dump wieder, siehe dazu die Anmerkungen auf der rechten Seite oben.
Auch können wir die Längenangaben nachspüren. 

Was haben wir damit erreicht? Es ist mit Hilfe der beiden Klassen `std::pmr::vector<>` und `std::pmr::string` möglich,
jegliche Allokationen auf dem Heap zu umgehen!

## Erste Stolperfalle: Klasse `std::pmr::vector<std::pmr::string>` und Methode `push_back` <a name="link4"></a>

Wir bleiben bei dem Thema &bdquo;Einsatz der `std::pmr::vector<std::pmr::string>`-Klasse&rdquo;
unter Verwendung einer `std::pmr::monotonic_buffer_resource`-SpeicherRessource.

Dieses Szenario ist dann interessant, wenn man jegliche Heap-Allokationen vermeiden möchte (Umfeld des *Embedded Programming*)
und die Daten kontrolliert in einem globalen Speicherbereich unterbringen möchte.
Sicherlich kann man hier einwenden, dass dieser Speicherbereich eine feste Größe hat und damit in einem gewissen Widerspruch
zur dynamischen Allokation von Objekten steht. Nicht unbedingt, in manchen Anwendungen kommt man nicht umhin,
eine Vorstellung von der maximalen Anzahl der Objekte zu haben, die zur Laufzeit vorhanden sein können bzw. müssen. 
Und dann macht die Abbildung von Allokator-bezogenen  Containerobjekten auf einen globalen Speicherbereich sehr wohl Sinn.

Bevor wir auf das nächste Beispiel eingehen, benötigen wir einen Speichertracker für den globalen Heap.
Ja, richtig gelesen, dieser Tracker soll eigentlich überhaupt nicht anspringen, wenn die Objekte alle in einer `std::pmr::monotonic_buffer_resource`-Speicherresource 
abgelegt werden, aber genau dies gilt es ja zu verifizieren.

Unser Speichertracker ist ebenfalls eine polymorphe SpeicherRessource:


```cpp
01: class NewDelete_ResourceTracker : public std::pmr::memory_resource
02: {
03: public:
04:     NewDelete_ResourceTracker() {
05:         std::println("New Delete Resource Tracker:");
06:     }
07: 
08:     ~NewDelete_ResourceTracker() {
09:         std::println("New Delete Resource Tracker: Done.");
10:     }
11: 
12: private:
13:     void* do_allocate(std::size_t bytes, std::size_t alignment) override {
14:         std::println("## allocating {} bytes", bytes);
15:         return std::pmr::new_delete_resource()->allocate(bytes, alignment);
16:     }
17: 
18:     void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
19:         std::println("## deallocating {} bytes", bytes);
20:         std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
21:     }
22: 
23:     bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
24:         std::println("## do_is_equal");
25:         return this == &other;
26:     }
27: };
```

Damit geht es mit unserem Testprogramm weiter. In Zeile 14 finden wir ein `std::pmr::vector<std::pmr::string>`-Objekt vor,
es werden mit `emplace_back` zwei Zeichenketten im STL-Container des Typs `std::pmr::vector` aufgenommen:

```cpp
01: void test()
02: {
03:     NewDelete_ResourceTracker tracker;
04:     std::pmr::set_default_resource(&tracker);
05: 
06:     constexpr std::size_t NumBytes = 512;
07: 
08:     std::array<std::uint8_t, NumBytes> memory{};
09: 
10:     std::pmr::monotonic_buffer_resource resource{
11:         memory.data(), memory.size(), std::pmr::null_memory_resource()
12:     };
13: 
14:     std::pmr::vector<std::pmr::string> strings{ &resource };
15: 
16:     strings.emplace_back("A lazy dog");
17:     strings.emplace_back("Another dog");
18: }
```

*Ausgabe*:

```
New Delete Resource Tracker:
New Delete Resource Tracker: Done.
```

Der Tracker ist nicht angesprungen, wir haben bislang unser Ziel erreicht.
Wie sieht es mit längeren Zeichenketten aus?

```
strings.emplace_back("The quick brown fox jumps over the lazy dog");
strings.emplace_back("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
```

*Ausgabe*:

```
New Delete Resource Tracker:
New Delete Resource Tracker: Done.
```

Okay, auch das funktioniert so, wie es sein soll. Da wir bislang ausschließlich die Methode `emplace_back` verwendet haben,
kann man sagen, dass der Allokator des `std::pmr::vector<std::pmr::string>`-Objekts mehr oder minder direkt angesprochen wurde.

Könnten wir auch die `push_back`-Methode auch verwenden?


```
strings.push_back("A lazy dog");
strings.push_back("Another dog");
```

*Ausgabe*:

```
New Delete Resource Tracker:
## allocating 16 bytes
## deallocating 16 bytes
## allocating 16 bytes
## deallocating 16 bytes
New Delete Resource Tracker: Done.
```

Hmm, das ist nicht die beabsichtigte Vorgehensweise. Warum wird für die Parameter nicht ebenfalls ein `std::pmr::string`-Objekt
mit dem vom übergeordneten PMR-Container delegierten Speicherallokator aufgerufen?

Dazu müssen wir einen Blick auf die exakte Definition der `std::pmr::vector`-Klasse werfen:

```cpp
namespace pmr {
    template <class T>
    using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
}
```

Jetzt sind wir aber nicht an dem Allokator interessiert, sondern an der `push_back`-Methode.
Diese ist so definiert:

```cpp
void push_back( const T& value );
void push_back( T&& value );
```

Der Template Parameter `T` wird damit auf `std::string` umgesetzt, es findet keine Abbildung auf die Klasse `std::pmr::string` statt!
Damit haben wir zumindest die Antwort auf die zuvor gestellte Frage gefunden.

Wir verweilen dennoch bei der `push_back`-Methode. Es müssen nur die übergebenen Objekte vom korrekten Typ sein:

```cpp
std::pmr::string s1{ "A lazy dog", &resource };
std::pmr::string s2{ "Another dog", &resource };
```

oder

```cpp
std::pmr::string s3{ "The quick brown fox jumps over the lazy dog", &resource };
std::pmr::string s4{ "Lorem ipsum dolor sit amet, consectetur adipiscing elit", &resource };
```

Beides funktioniert, man beachte jeweils den zweiten Parameter: 
Es wird eine Adresse desselben `std::pmr::monotonic_buffer_resource`-Objekts an die Zeichenkettenobjekte übergeben,
dass zuvor schon dem Containerobjekt vom Typ `std::pmr::vector<std::pmr::string>` zugewiesen worden ist.

*Fazit*:<br />
Es ist sehr wohl möglich, dass STL-Container und die Elemente dasselbe Speicherressourcenobjekt haben!
Man muss nur etwas aufpassen, das nicht versehentlich die dynamischen Speicherverwaltung mit `new` / `delete` dazwischen funkt.

## Zweite Stolperfalle: Klasse `std::pmr::vector<std::pmr::string>` und Initialisierungslisten <a name="link5"></a>

Wir wenden uns polymorphen STL-Containern wie z. B. der Klasse `std::pmr::vector<std::pmr::string>` noch einmal zu,
dieses Mal syntaktisch gesehen mit dem Sprachmittel einer Initialisierungsliste (`std::initializer_list<T>`).
Natürlich soll auch in diesem Abschnitt die polymorphe Speicherressource `std::pmr::monotonic_buffer_resource`
zu Grunde gelegt werden:

```cpp
01: void test()
02: {
03:     NewDelete_ResourceTracker tracker;
04:     std::pmr::set_default_resource(&tracker);
05: 
06:     constexpr std::size_t NumBytes = 512;
07: 
08:     std::array<std::uint8_t, NumBytes> memory{};
09: 
10:     std::pmr::monotonic_buffer_resource resource{
11:         memory.data(), memory.size(), std::pmr::null_memory_resource()
12:     };
13: 
14:     std::pmr::vector<std::pmr::string> strings{ "Hello", "World" };
15: }
```


*Ausgabe*:

```
New Delete Resource Tracker:
## allocating 16 bytes
## allocating 16 bytes
## allocating 16 bytes
## allocating 96 bytes
## allocating 16 bytes
## allocating 16 bytes
## deallocating 16 bytes
## deallocating 16 bytes
## deallocating 16 bytes
## deallocating 16 bytes
## deallocating 96 bytes
## deallocating 16 bytes
New Delete Resource Tracker: Done.
```

Okay, Sie werden sagen, dass kann so nicht funktionieren, da das `std::pmr::monotonic_buffer_resource`-Objekt
überhaupt nicht ins Spiel gekommen ist. Damit unternehmen wir einen zweiten Versuch:
Wir tauschen Zeile 14 des letzten Listings wie folgt aus:

```cpp
std::pmr::vector<std::pmr::string> strings{ {"Hello", "World" }, &resource };
```


*Ausgabe*:

```
New Delete Resource Tracker:
## allocating 16 bytes
## allocating 16 bytes
## deallocating 16 bytes
## deallocating 16 bytes
New Delete Resource Tracker: Done.
```

Zumindest wurde die dynamische Speicherverwaltung etwas weniger in Anspruch genommen, aber das ist nicht das Ziel.
Wir wollen keinen einzigen Aufruf an `new` bzw. `delete` vorfinden!

Auch mit dieser Anweisung erreichen wir das Ziel nicht:

```cpp
std::pmr::vector<std::pmr::string> strings{ { std::pmr::string {"Hello"},  std::pmr::string{"World"} }, & resource };
```

Wir könnten auch versuchen, ein `std::initializer_list<std::pmr::string>`-Objekt explizit anzulegen.
Das würde sogar gehen, sogar ohne `new` bzw. `delete`-Aufrufe!
Aber beim Umkopieren der Liste in den Container gelangen die Objekte wieder auf den Heap:

```cpp
std::initializer_list<std::pmr::string> list{
    std::pmr::string {"Hello", &resource},
    std::pmr::string{"World", &resource}
};

std::pmr::vector<std::pmr::string> strings{ list.begin(), list.end() };
```

*Ausgabe*:

```
New Delete Resource Tracker:
## allocating 16 bytes
## allocating 96 bytes
## allocating 16 bytes
## allocating 16 bytes
## deallocating 16 bytes
## deallocating 16 bytes
## deallocating 96 bytes
## deallocating 16 bytes
New Delete Resource Tracker: Done.
```

Wir sind nun dem Problem auf die Spur gekommen: Wenn wir ein `std::pmr::vector<std::pmr::string>`-Objekt
mit einer Initialisierungsliste vorbelegen, kommen Heap-Zugriffe ins Spiel, da temporäre `std::string`-Objekte entstehen,
die nicht mit der gewünschten Speicherresource arbeiten.

Wie lässt sich dieses Problem beheben? Einfache Antwort: **Überhaupt nicht**!

Es wäre möglich, mit einer Utility-Funktion einen gewissen Touch einer Initialisierungsliste nachzuahmen,
aber mit dem eigentlichen Thema (Vorbelegung eines `std::pmr::vector<std::pmr::string>`-Objekts mit einer Initialisierungsliste)
hat dieser Workaround dann nichts mehr zu tun!

Gut, dann lassen Sie mich einen Blick auf diesen Workaround werfen: Wir schreiben eine Utility-Funktion,
die ein vorgegebenes `std::pmr::vector<std::pmr::string>`-Objekt sukzessive mit `emplace_back`-Methodenaufrufen befüllt.
Wir sind gewissermaßen wieder am Anfang unseres kleinen Ausflugs in die Niederungen der `std::pmr::vector<std::pmr::string>`-Klasse angekommen.
Die Utility-Funktion, sie heißt `createContainer`, könnte so aussehen:

```cpp
template <typename TContainer, typename ... TValues>
auto createContainer(auto* resource, TValues&&... values)
{
    TContainer result{ resource };
    result.reserve(sizeof... (values));
    (result.emplace_back(std::forward<TValues>(values)), ...);
    return result;
};
```

Beim näheren Betrachten von `createContainer` kann man feststellen, dass wir gewissermaßen ein &bdquo;All-In&rdquo; der
modernen Sprachkonzepte von C++ vorfinden, als da wären:

  * Variadische Templates (Versorgung der Funktion mit einer variabel langen Liste von Zeichenketten)
  * Uniforme Referenzen (`TValues&&... values`)
  * Perfektes Weiterleiten (`std::forward<TValues>(values)`)
  * Folding über einem Komma (`(result.emplace_back(std::forward<TValues>(values)) , ...);`)


Damit kommen wir zu einem Testrahmen:

```cpp
01: void test()
02: {
03:     NewDelete_ResourceTracker tracker;
04:     std::pmr::set_default_resource(&tracker);
05: 
06:     constexpr std::size_t NumBytes = 1024;
07: 
08:     std::array<std::uint8_t, NumBytes> memory{};
09: 
10:     std::pmr::monotonic_buffer_resource resource{
11:         memory.data(), memory.size(), std::pmr::null_memory_resource()
12:     };
13: 
14:     std::pmr::vector<std::pmr::string> vec{ 
15:         createContainer<std::pmr::vector<std::pmr::string>>(
16:             &resource,
17:             "Hello",
18:             "World",
19:             "The quick brown fox jumps over the lazy dog",
20:             "Lorem ipsum dolor sit amet, consectetur adipiscing elit"
21:         )
22:     };
23: 
24:     for (std::size_t i{};  const auto& s : vec) {
25:         std::println("{}: {}", i, s);
26:         ++i;
27:     }
28: }
```

*Ausgabe*:

```
New Delete Resource Tracker:
0: Hello
1: World
2: The quick brown fox jumps over the lazy dog
3: Lorem ipsum dolor sit amet, consectetur adipiscing elit
New Delete Resource Tracker: Done.
```

Wir haben es wieder geschafft, den Heap außer Acht zu lassen. Um es aber noch einmal deutlich zu sagen:
`std::pmr::vector<std::pmr::string>`-Objekte und Initialisierungslisten sind keine Freunde, 
wir sollten einfach beim Befüllen eines solchen Objekts auf eine Initialisierungsliste verzichten.


## Literatur <a name="link6"></a>

Die Idee zu der Funktion `createContainer` stammt von Jason Turner und wurde [hier](https://www.youtube.com/watch?v=6BLlIj2QoT8) gefunden.

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---
