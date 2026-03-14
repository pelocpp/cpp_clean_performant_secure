# Speicherressourcen / Memory Resources

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---

#### Quellcode

[*PMR_02.cpp*](PMR_02.cpp)<br />
[*PMR_DumpBuffer.cpp*](PMR_DumpBuffer.cpp)<br />

---

### Allgemeines <a name="link1"></a>

---

## Überblick

Ab C++ 17 sind Klassen, die eine Speicherressource repräsentieren möchten,
von der abstrakten Klasse `std::pmr::memory_resource` abzuleiten:
Im Regelfall müssen wir konkrete Spezialisierungen der `std::pmr::memory_resource`-Klasse nicht selbst realisieren,
die Standardbibliothek stellt mehrere konkrete Implementierungen bereit.
Sie sind über die Headerdatei `<memory_resource>` zugänglich.

Man beachte, dass die Klasse `std::pmr::memory_resource` keine Klassenschablone ist,
sondern eine ganz &bdquo;normale Klasse&rdquo;.
Mit einer gewissen Vereinfachung sieht ihre Definition so aus:

```cpp
01: class memory_resource
02: {
03: private:
04:     virtual void* do_allocate(std::size_t bytes, std::size_t align) = 0;
05:     virtual void  do_deallocate(void* ptr, std::size_t bytes, std::size_t align) = 0;
06:     virtual bool  do_is_equal(const memory_resource& other) const = 0;
07: 
08: public:
09:     void* allocate(std::size_t bytes, std::size_t align) {
10:         return do_allocate(bytes, align);
11:     }
12: 
13:     void deallocate(void* ptr, std::size_t bytes, std::size_t align) {
14:         return do_deallocate(ptr, bytes, align);
15:     }
16: 
17:     bool is_equal(const memory_resource& other) const {
18:         return do_is_equal(other);
19:     }
20: };
```

### Eine Besonderheit in der Definition der Klasse `std::pmr::memory_resource`

Vielleicht ist es Ihnen schon aufgefallen, dass die Klasse `std::pmr::memory_resource` 
nicht ganz wie eine &bdquo;gewöhnliche&rdquo; Schnittstellenklasse in C++ aussieht
(Klasse, die nur aus öffentlichen virtuellen Methoden ohne Realisierung besteht).
Im Gegensatz zu so einer Klasse hat man bei der Klasse `std::pmr::memory_resource` den Weg in der Definition einer Basisklasse 
mit drei fest vorgegebenen Methoden `allocate`, `deallocate` und `do_is_equal` gewählt.

Bei genauem Hinsehen erkennt man hier das *Template-Method* Entwurfsmusters,
an manch anderen Stellen in der einschlägigen Literatur spricht man auch von einer
Umsetzung des &bdquo;*Non-Virtual-Interface*&rdquo;-Patterns (*NVI*-Pattern).
Dies bedeutet konkret, dass in der Klasse drei nicht-virtuelle Methoden `allocate`, `deallocate` und `is_equal`
vorhanden sind, deren Funktionalität durch das Überschreiben der drei virtuellen Methoden `do_allocate`, `do_deallocate` und `do_is_equal`
beeinflusst werden kann.

Die Anwendung dieses Entwurfsmusters bietet gegenüber einer rein öffentlichen virtuellen Schnittstelle mehrere Vorteile:

  * Kontrolle und Invarianten:<br />Die öffentlichen Methoden (&bdquo;Template Methoden&rdquo;) fungieren als stabile &bdquo;Hülle&rdquo;.
    Man kann dort Anweisungen platzieren,
    die für alle Unterklassen zu gelten haben (z. B. die Überprüfung der Parameter, Logging oder das Setzen von Performance-Zählern),
    ohne dass jede einzelne Implementierung daran denken muss.

  * Schnittstellenstabilität:<br />Man kann die öffentliche Schnittstelle (öffentliches API) ändern &ndash; z. B. einen Standardwert
    für das *Alignment* hinzufügen oder das Rückgabeformat anpassen &ndash;,
    ohne die virtuelle Schnittstelle der abgeleiteten Klassen anzutasten. Die Basisklasse behält die volle Kontrolle darüber, wie die `do_`-Methoden
    aufzurufen sind.

  * Klarheit der Verantwortlichkeiten:<br />
    * Öffentliche Schnittstelle (*Public API*): Definiert, was der Nutzer sieht und wie er die Klasse verwenden kann.
    * Private Schnittstelle (*Privates virtuelles API*): Definiert, wie spezialisierte Unterklassen das Verhalten implementieren.

  * *Fragile Base Class* Problem:<br />Es verhindert, dass Nutzer der Klasse die virtuellen Methoden direkt aufrufen.
     Dies kann dann unerwünscht sein, wenn Vor- oder Nachbereitungen nötig sind. 

In der C++ Standardbibliothek wird dieses Muster bei der Klasse `std::pmr::memory_resource` gezielt eingesetzt,
um sicherzustellen, dass die Speicherverwaltung strikten Regeln folgt, während die konkreten Allokatoren (z. B. die Klasse `std::pmr::monotonic_buffer_resource`)
nur die Kernlogik implementieren müssen.


## Globale Speicherressourcen

Dies sind Singleton-ähnliche Ressourcen, auf die über freie Funktionen zugegriffen werden kann:

  * `std::pmr::new_delete_resource`: Verwendet die globalen Operatoren `new` und `delete` zur Speicherallokation und -freigabe.
  * `std::pmr::null_memory_resource`: Eine &bdquo;nicht-allokierende&rdquo; Ressource, bei der jeder Aufruf von `allocate()` eine Ausnahme
    vom Typ `std::bad_alloc` auslöst. Sie wird häufig als &bdquo;terminale&rdquo; *Upstream*-Ressource verwendet, um versehentliche Heap-Fallbacks zu verhindern.


## Spezialisierte Ressourcenklassen

Die STL enthält mehrere klassenbasierte Implementierungen für gängige Allokationsstrategien:

  * `std::pmr::monotonic_buffer_resource`: Ein schneller Allokator im Stile einer &bdquo;Arena&rdquo;. Der Speicher wird nur freigegeben, wenn die Ressource selbst zerstört wird. Er eignet sich ideal für kurzlebige, große Allokationen, ist aber nicht threadsicher.
  * `std::pmr::synchronized_pool_resource`: Ein threadsicherer, universeller Pool-Allokator. Er verwaltet eine Sammlung von Pools für verschiedene Blockgrößen, um die Fragmentierung zu reduzieren, und kann von mehreren Threads ohne externe Sperren verwendet werden.
  * `std::pmr::unsynchronized_pool_resource`: Ähnlich dem synchronisierten Pool, jedoch ohne den Overhead der Threadsicherheit. Verwenden Sie diese Klasse, wenn die Ressource nur von einem einzigen Thread verwendet wird.


## Die vordefinierten Ressourcenklassen im Vergleich

| Ressource | Thread-sicher? | Allokationsstrategie | Bester Anwendungsfall |
|:-|:-|:-|:-|
| `std::pmr::new_delete_resource` | Ja | Globaler Heap | Standard-/Fallback-Verhalten |
| `std::pmr::null_memory_resource` | Ja | Schlägt immer fehl | Debugging / Verhinderung der Heap-Nutzung |
| `std::pmr::monotonic_buffer_resource` | Nein | Linear / Arena | Schnelle Batch-Allokationen |
| `std::pmr::synchronized_pool_resource` | Ja | Pools mit Blöcken | Multithreading Szenario / Blöcke mit fester Größe |
| `std::pmr::unsynchronized_pool_resource` | Nein | Pools mit Blöcken | Singlethreading Szenario / Blöcke mit fester Größe |

Tabelle 1: Hauptmerkmale der einzelnen Ressourcenklassen.


## Ein Beispiel zur Klasse `std::pmr::monotonic_buffer_resource`

Im ersten Beispiel betrachten wir ein Beispiel zur Klasse `std::pmr::monotonic_buffer_resource`.
Diese benötigt einen Speicherbereich, der von der Resource verwaltet wird.
Dies kann ein Byte-Array im C-Stil sein, oder in C++ ein `std::array`-Objekt.

Das Thema der so genannten *Upstream*-Speicherressource wollen wir auf das nächste Beispiel vertagen, 
deshalb tritt im Konstruktorenaufruf an der dritten Stelle aktuell nur ein `nullptr` in Erscheinung.

Die `allocate`-Methode wird zweimal mit den Werten 4 (`sizeof(std::uint32_t)`) und 4 (`alignof (std::uint32_t)`) aufgerufen.
Damit soll pro Aufruf Speicher für eine `std::uint32_t`-Variable mit entsprechender Ausrichtung reserviert werden:

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 8;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::println("Address of current buffer:    {:#X}", reinterpret_cast<intptr_t>(g_memory.data()));
08: 
09:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
10: 
11:     std::pmr::monotonic_buffer_resource res { g_memory.data(), g_memory.size(), nullptr };
12: 
13:     std::uint32_t* ptr{ static_cast<std::uint32_t*> (res.allocate(sizeof(std::uint32_t), alignof (std::uint32_t))) };
14: 
15:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
16:     *ptr = 123;
17:     res.deallocate(ptr, alignof (std::uint32_t));
18: 
19:     ptr = static_cast<std::uint32_t*> (res.allocate(sizeof(std::uint32_t), alignof (std::uint32_t)));
20:     std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
21:     *ptr = 456;
22:     res.deallocate(ptr, alignof (std::uint32_t));
23: 
24:     std::span span{ g_memory };
25:     dumpBuffer(span);
26: }
```

*Ausgabe*:

```
Address of current buffer:    0X7FF717E843B8
Pointer to allocated storage: 0X7FF717E843B8
Pointer to allocated storage: 0X7FF717E843BC
===========================================================================
Memory Dump
Buffer: 0X7FF717E843B8
Bytes:  8
---------------------------------------------------------------------------
0000 | 7b 00 00 00 c8 01 00 00                          | {.......
---------------------------------------------------------------------------
```

Man kann an den Adressen erkennen, dass der `allocate`-Aufruf am `std::pmr::monotonic_buffer_resource`-Objekt
jedes Mal 4 Bytes reserviert. Ebenfalls gut erkennbar ist, dass der `deallocate`-Aufruf keine Auswirkungen
auf das Speicherressourcenobjekt hat &ndash; der Speicherbereich an der Adresse `ptr` wird *nicht* wieder in den Freispeicher eingeordnet.
So gesehen kann man sich bei `std::pmr::monotonic_buffer_resource`-Objekten diese Aufrufe auch schenken,
sie sind umsonst und kosten nur unnütze Laufzeit.

Was man gut an diesem Beispiel erkennen kann: Ein  `std::pmr::monotonic_buffer_resource`-Objekt ist eine *Speicherressource*,
seine Methoden sind nicht für reale Datentypen oder gar Instanzen von Klassen ausgelegt.
Die `allocate`-Methode erwartet Längenangaben in Bytes, der Rückgabewert ist ein `void*`-Zeiger. 
Um mit diesen Adressen typ-konform arbeiten zu können, benötigt man `reinterpret_cast`-Typwandlungen.
Auf dieser Ebene will man nicht wirklich arbeiten, deshalb gibt es die *Allokatorenobjekte*, hierauf kommen wir im nächsten Abschnitt zu sprechen.


## Die &bdquo;*Upstream*&rdquo; Speicherressource

Die Ressource `std::pmr::monotonic_buffer_resource` kann mit einem initialen Puffer erstellt werden.
Ist kein initialer Puffer vorhanden oder ist dieser erschöpft,
kann zusätzlicher Speicher aus einer so genannten *Upstream*-Speicherressource bezogen werden.
Die *Upstream*-Speicherressource ist bei der Konstruktion eines `std::pmr::monotonic_buffer_resource`-Objekts mit anzugeben.

Wird kein derartiges Objekt in einem der Konstruktoraufrufe von `std::pmr::monotonic_buffer_resource` übergeben,
gibt es eine Default *Upstream*-Speicherressource. Diese kann mit den beiden freien Funktionen
`std::pmr::get_default_resource()` bzw. `std::pmr::set_default_resource()` gelesen oder geschrieben werden:

```cpp
std::pmr::memory_resource* get_default_resource () noexcept;
std::pmr::memory_resource* set_default_resource (std::pmr::memory_resource* res) noexcept;
```

Im Beispiel aus dem letzten Abschnitt wurde die *Upstream*-Speicherressource mit `nullptr` vorbelegt.
Das ist auf der einen Seite einigermaßen gefährlich, wenn der Speicher, der anfänglich an das Speicherressourcenobjekt übergeben worden ist,
nicht mehr ausreicht. Auf der anderen Seite ist dies die einzige Möglichkeit, wenn man garantieren möchte,
dass das `std::pmr::monotonic_buffer_resource`-Objekt den ihm zugewiesenen Speicherbereich nicht verlässt.


## Zweites Beispiel zur Klasse `std::pmr::monotonic_buffer_resource`

Wir gehen in diesem Beispiel auf die *Upstream*-Speicherressource eines Speicherressourcenobjekts näher ein:


*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 8;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::pmr::monotonic_buffer_resource res{ g_memory.data(), g_memory.size() };
08: 
09:     std::pmr::memory_resource* upstream{ res.upstream_resource() };
10: 
11:     std::pmr::memory_resource* resource{ std::pmr::new_delete_resource() };
12: 
13:     if (upstream == resource) {
14:         std::println("Default resource equals std::pmr::new_delete_resource");
15:     }
16: }
```

*Ausgabe*:

```
Default resource equals std::pmr::new_delete_resource
```

Wenn wir ein `std::pmr::monotonic_buffer_resource`-Objekt ohne expliziten dritten Parameter anlegen (siehe Zeile 7 des letzten Listings),
dann wird dem Objekt automatisch das *Default*-Speicherressourcenobjekt zugeordnet.
Dieses ist vom Typ `std::pmr::new_delete_resource`, solange wir mit einem Aufruf von `std::pmr::set_default_resource` kein anderes Objekt zugeordnet haben.


# Ausgabe eines Speicherbereichs in der Konsole (*Dump*)

Da wir öfters den Speicherbereich, der von einem Speicherressourcenobjekt verwaltet wird, näher betrachten wollen,
habe ich eine Utility-Funktion `dumpBuffer` geschrieben:


```cpp
01: void dumpBuffer(std::span<std::uint8_t> buffer)
02: {
03:     std::println("===========================================================================");
04:     std::println("Memory Dump");
05:     std::println("Buffer: {:#X}", reinterpret_cast<intptr_t>(buffer.data()));
06:     std::println("Bytes:  {}", buffer.size());
07:     std::println("---------------------------------------------------------------------------");
08: 
09:     constexpr auto elementsPerLine{ static_cast<std::size_t>(16) };
10: 
11:     std::size_t idx{};
12: 
13:     while (idx < buffer.size()) {
14: 
15:         std::size_t first{ idx };
16:         std::size_t last{ idx + std::min(elementsPerLine, buffer.size() - first) };
17: 
18:         std::print("{:04x} |", idx);
19: 
20:         for (std::size_t k{ first }; k != last; ++k) {
21: 
22:             if (k % 8 == 0) {
23:                 std::print(" "); 
24:             }
25: 
26:             std::print("{:02x} ", buffer[k]);
27:         }
28: 
29:         // complete last line with blanks, if necessary
30:         if (last != first + elementsPerLine) {
31: 
32:             std::size_t blanksNeeded{ first + elementsPerLine - last };
33: 
34:             if (blanksNeeded >= 8) {
35:                 std::print(" ");
36:             }
37: 
38:             for (std::size_t k{ }; k != blanksNeeded; ++k) {
39: 
40:                 std::print("   ");
41:             }
42:         }
43: 
44:         std::print("|");
45: 
46:         for (std::size_t k{ first }; k != last; ++k) {
47: 
48:             if (k % 8 == 0) {
49:                 std::print(" ");
50:             }
51: 
52:             if (std::isprint(buffer[k])) {
53:                 std::print("{}", static_cast<char> (buffer[k]));
54:             }
55:             else {
56:                 std::print(".");
57:             }
58:         }
59: 
60:         idx += elementsPerLine;
61: 
62:         std::println();
63:     }
64: 
65:     std::println("---------------------------------------------------------------------------");
66: }
```

*Beispiel*:

```cpp
01: constexpr std::size_t NumBytes = 32;
02: 
03: std::array<std::uint8_t, NumBytes> g_memory{};
04: 
05: void test()
06: {
07:     std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());
08: 
09:     std::span span{ g_memory };
10: 
11:     dumpBuffer(span);
12: }
```


*Ausgabe*:

```
===========================================================================
Memory Dump
Buffer: 0X7FF6EC4CE260
Bytes:  32
---------------------------------------------------------------------------
0000 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
0010 | ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff | ........ ........
---------------------------------------------------------------------------
Done.
```

---

[Zurück](Readme_Performance_Optimization_Advanced_PMR.md)

---
