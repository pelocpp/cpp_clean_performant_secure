# Realisierung eines Objektpools / *Object Pool*

[Zurück](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Ein Objektpool mit statischer Größe](#link2)
  * [Ein Objektpool mit dynamischer Größe](#link3)
  * [Ein Objektpool mit statischer Größe Multi-Threading sicher](#link4)
  * [Literatur](#link5)

---

## Voraussetzungen

  * *Placement New*
  * &bdquo;*Zeigerwäsche*&rdquo;: `std::launder`
  * Speicher-Allokatoren (`std::allocator`)
  
---

#### Quellcode

[*ObjectPool.h*](ObjectPool.h)<br />
[*ObjectPool.cpp*](ObjectPool.cpp)<br />
[*ObjectPool_Test.cpp*](ObjectPool_Test.cpp)

---

## Allgemeines <a name="link1"></a>

In diesem Abschnitt beschreiben wir die Implementierung
mehrerer *Object Pool* Klassen, die sehr wohl für einen produktiven Einsatz
geeignet sind:

* Objektpool mit statischer Größe (feste Anzahl von Objekten).
* Objektpool mit dynamischer Größe (die Anzahl der Objekte kann sich zur Laufzeit vergrößern).
* Objektpool mit statischer Größe Multi-Threading sicher.


Es gibt verschiedene Arten von Objektpool-Klassen.
Dieser Abschnitt beschreibt mehrere Objektpool-Realisierungen, die einen großen Speicherblock auf einmal allokieren
und kleinere Objekte direkt &bdquo;*in-place*&rdquo; erstellen.

Diese Objekte können an Clients ausgehändigt und wiederverwendet werden,
wenn diese sie nicht mehr benötigen.

Dies erfordert *keine* zusätzlichen Aufrufe des Speichermanagers,
um Speicher für einzelne Objekte zuzuweisen (`new`) oder freizugeben (`delete`).

---

## Ein Objektpool mit statischer Größe <a name="link2"></a>

---

## Ein Objektpool mit dynamischer Größe <a name="link3"></a>

Die Implementierung des Objektpools verwaltet einen Vektor mit Objektblöcken vom Typ `T`.
Nach dem Start gibt es in diesem Vektor nur ein Element (Zeiger).
Dieser Zeiger verweist auf einen Speicherbereich,
der eine bestimmte Anzahl von Objekten (Typ `T`) aufnehmen kann, 
die dieser Pool verwaltet:

```cpp
std::vector<T*>  m_pool;
```

Zusätzlich werden freie Objekte in einem Vektor mit Zeigern auf alle freien Objekte protokolliert:

```cpp
std::vector<T*>  m_free;
```

Da zu Beginn alle Objekte im Pool verfügbar sind,
sind hier alle Adressen verfügbar, die durch den ersten Block
im Pool angesprochen werden können.


Wir versuchen, in den folgenden Abbildungen das Szenario im
Objektpool &bdquo;bildlich&rdquo; darzustellen.

Zu Beginn wird im Pool eine bestimmte Menge an freien Objektblöcken eingerichtet.
Da alle diese Blöcke verfügbar sind, sind deren Anfangsadressen 
in der zweiten Liste `m_free` aller verfügbaren Objekte eingetragen.

<img src="cpp_object_pool_01.svg" width="600">

*Abbildung* 1: `m_pool` und `m_free` nach dem Start.

Jetzt stellen wir uns vor, dass zwei Objektblöcke der Anwendung
zur Verfügung gestellt wurden. In *Abbildung* 2 erkennen wir, 
dass am Ende von `m_free` zwei Adressen fehlen: Diese Adressen
sind nun in der Obhut der Anwendung, sie können allerdings,
wenn die Anwendung die Objekte nicht mehr benötigt,
in der `m_free`-Liste wieder eingereiht werden.

<img src="cpp_object_pool_02.svg" width="600">

*Abbildung* 2: `m_pool` und `m_free` nach Zuteilung zweier Objekte.

Sind alle Objekte des Pools zugeteilt, erkennen wir in
Abbildung 3, dass die Liste  `m_free` leer ist.
Es gibt keine freien Blöcke mehr.

<img src="cpp_object_pool_03.svg" width="600">

*Abbildung* 3: Ein Pool ohne freie Objekte.

Wird jetzt ein neues Objekt angefordert,
muss zunächst der Pool an sich wieder nachladen.

Die grafische Darstellung wird nun ein wenig komplexer.
In *Abbildung* 4 erkennen wir, dass zunächst der Pool
neuen Speicher bekommen hat. Dieser wird komplett in die Liste
der freien Blöcke aufgenommen und neue Blöcke sind wieder verfügbar.

<img src="cpp_object_pool_04.svg" width="800">

*Abbildung* 4: Ein Pool mit neuen Chunks.


Der Pool stellt Objekte über die Memberfunktion `acquireObject()` zur Verfügung.
Wird `acquireObject()` aufgerufen und sind aber keine freien Objekte mehr vorhanden,
allokiert der Pool (durch die Methode `addChunk`) einen weiteren Vektor mit Objektblöcken vom Typ `T`.

Die Klasse `ObjectPool` ist ein Klassentemplate.
Die beiden Templateparameter `T` und `TAllocator` parametrisieren
die im Pool zu speichernden Typen und den Typ des Allokators,
der für die Allokation und Freigabe von Speicherblöcken verwendet wird.

```cpp
template <typename T, typename TAllocator = std::allocator<T>>
class ObjectPool final
```

---

### Die Memberfunktion `addChunk()`

Die Memberfunktion `addChunk()` zum Zuweisen eines neuen Chunks ist wie folgt implementiert.
Der erste Teil von `addChunk()` führt die eigentliche Zuweisung eines neuen Chunks durch.
Ein &bdquo;Chunk&rdquo; ist ein nicht initialisierter Speicherblock, der mithilfe eines Allocators zugewiesen wird und groß genug ist,
um `m_newChunkSize` Instanzen von `T` aufzunehmen:

```cpp
01: template <typename T, typename TAllocator>
02: inline void ObjectPool<T, TAllocator>::addChunk()
03: {
04:     std::println("allocating new chunk ...");
05: 
06:     m_pool.push_back(nullptr);
07:     try {
08:         m_pool.back() = m_allocator.allocate(m_currentChunkSize);
09:     }
10:     catch (...) {
11:         m_pool.pop_back();
12:         throw;
13:     }
14: 
15:     auto oldFreeObjectsSize{ m_freeObjects.size() };
16: 
17:     m_freeObjects.resize(oldFreeObjectsSize + m_currentChunkSize);
18: 
19:     // create pointers to the new instances of T (using std::iota)
20:     std::iota(
21:         std::begin(m_freeObjects) + oldFreeObjectsSize, 
22:         std::end(m_freeObjects), 
23:         m_pool.back()
24:     );
25: 
26:     m_currentChunkSize *= 2;
27: }
```

Durch das Hinzufügen eines Objekt-Chunks werden noch keine Objekte erstellt;
d.h. es werden keine Objektkonstruktoren aufgerufen.
Dies geschieht später in `acquireObject()`, wenn Instanzen vergeben werden.

Der zweite Teil von `addChunk()` erstellt Zeiger auf die neuen Instanzen von `T`. 
Er verwendet den `iota()`-Algorithmus, definiert in der Datei `<numeric>`.

Zur Erinnerung: `iota()` füllt einen durch die ersten beiden Argumente vorgegebenen Bereich mit Werten.
Die Werte beginnen mit dem Wert des dritten Arguments und werden für jeden nachfolgenden Wert um Eins erhöht.

Da wir mit `T*`-Zeigern arbeiten, springt das Erhöhen eines `T*`-Zeigers um Eins
zum nächsten `T` im Speicherblock.

Abschließend wird der Wert von `m_newChunkSize` verdoppelt,
sodass der nächste hinzugefügte Block doppelt so groß ist wie der aktuelle Block.
Dies geschieht aus Performancegründen und folgt dem Prinzip von `std::vector`.

---

### Die Memberfunktion `acquireObject()`

```cpp
01: template <typename T, typename TAllocator>
02: template <typename... TArgs>
03: inline std::shared_ptr<T> ObjectPool::acquireObject(TArgs&& ... args)
04: {
05:     // if there are no free objects, need to allocate a new chunk
06:     if (m_freeObjects.empty()) { 
07:         addChunk();
08:     }
09: 
10:     // get a free object
11:     T* object{ m_freeObjects.back() };
12: 
13:     // construct an instance of T in this block using placement new
14:     ::new(object) T{ std::forward<TArgs>(args)... };
15: 
16:     // launder the object pointer.
17:     T* constructedObject{ std::launder(object) };
18: 
19:     // remove the object from the list of free objects
20:     m_freeObjects.pop_back();
21: 
22:     // wrap the constructed object and return it
23:     return std::shared_ptr<T>{
24:         constructedObject,
25:         [this] (T* object) {
26:             std::destroy_at(object);          // destroy object
27:             m_freeObjects.push_back(object);  // put object back in the list of free objects.
28:         }
29:     };
30: }
```

Methode `acquireObject()`, eine variadische *Member Template Function*,
gibt ein freies Objekt aus dem Pool zurück und allokiert einen neuen Block,
wenn keine freien Objekte mehr verfügbar sind.

Wie bereits erläutert, allokiert das Hinzufügen eines neuen Blocks lediglich einen Block nicht initialisierten Speichers.

`acquireObject()` ist dafür verantwortlich, eine neue Instanz von `T` an der richtigen Stelle im Speicher zu erstellen.
Dies geschieht mithilfe der *Placement-New* Technik.

Alle an `acquireObject()` übergebenen Argumente werden &bdquo;perfekt&rdquo;
an einen Konstruktor vom Typ `T` weitergeleitet.

*Hinweis*:<br />
`acquireObject()` verwendet die *Placement-New* Technik,
um eine neue Instanz eines Objekts vom Typ `T` an einem explizit vorgegebenen Speicherort zu erstellen.

Enthält Typ `T` Konstanten- oder Referenzen, führt der Zugriff auf das neu erstellte Objekt
über den ursprünglichen Zeiger zu undefiniertem Verhalten.

Um dieses Verhalten in definiertes Verhalten umzuwandeln, muss der Speicher mit `std::launder()`,
definiert in `<new>`, &bdquo;gewaschen&rdquo; werden.

Abschließend wird der &bdquo;gewaschene&rdquo; `T*`-Zeiger in einen `std::shared_ptr`-Objekt
mit einem benutzerdefinierten *Deleter* eingebettet.
Dieser *Deleter* gibt keinen Speicher frei, sondern ruft den Destruktor manuell mit `std::destroy_at()` auf und fügt den Zeiger anschließend wieder in die Liste der verfügbaren Objekte ein (`m_freeObjects`).

---

## Ein Objektpool mit statischer Größe Multi-Threading sicher <a name="link4"></a>

Wir orientieren uns bei der Realisierung eines thread-sicheren Objektpools 
an dem Objektpool aus Abschnitt
&bdquo;[Ein Objektpool mit statischer Größe](#link2)&rdquo;.
Da wir  die einzelnen Blöcke quasi im Stile einer einfach verketteten Liste verwalten,
können wir das &bdquo;Compare-Exchange&rdquo;-Idiom anwenden.
Dieses kommt in den beiden Methoden `allocate` und `deallocate` zur Anwendung:

```cpp
01: template <typename T, size_t Size>
02: [[nodiscard]] inline T* ObjectPool<T, Size>::allocate()
03: {
04:     auto item = m_nextFree.load();
05:     while (item != nullptr && !m_nextFree.compare_exchange_weak(item, item->m_next))
06:     {
07:     }
08: 
09:     if (item == nullptr) {
10:         throw std::bad_alloc{};
11:     }
12:         
13:     return reinterpret_cast<T*>(item);
14: }
```

und

```cpp
01: template <typename T, size_t Size>
02: inline void ObjectPool<T, Size>::deallocate(T* ptr) noexcept
03: {
04:     const auto item = reinterpret_cast<FreeList*>(ptr);
05: 
06:     item->m_next = m_nextFree;
07:     while (!m_nextFree.compare_exchange_weak(item->m_next, item))
08:     {
09:     }
10: }
```

Dabei wurde zu Grunde gelegt, dass die Variable `m_nextFree` atomar ist:

```
std::atomic<FreeList*> m_nextFree;
```

---

## Literatur <a name="link5"></a>

Die Realisierung des Object Pools mit einer statischen Größe kann man einem Youtube-Tutorial entnehmen:<br />
&bdquo;[Efficient Object Pool - How-To](https://www.youtube.com/watch?v=37VhkrOiE8M)&rdquo;.

Die Realisierung des Object Pools mit dynamischer Größe wurde in dem Buch 
&bdquo;[*Professional C++*](https://www.amazon.de/Professional-C-Marc-Gregoire/dp/1394193173)&rdquo; von Marc Gregoire
vorgefunden.

Die multi-threading sichere Variante wiederum ist eine Weiterarbeit des Beispiels aus Youtube:<br />
&bdquo;[Compare and Swap Loop - Learn Modern C++](https://www.youtube.com/watch?v=56Q4Ty4hfMM)&rdquo;.


---

[Zurück](Readme_Performance_Optimization_Advanced.md)

---
