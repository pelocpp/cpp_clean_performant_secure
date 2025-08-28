# Realisierung eines Objektpools / *Object Pool*

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Ein Objektpool mit statischer Gr��e](#link2)
  * [Ein Objektpool mit dynamischer Gr��e](#link3)
  * [Ein Objektpool mit statischer Gr��e Multi-Threading sicher](#link4)
  * [Literatur](#link5)

---

## Voraussetzungen

  * *Placement New*
  * &bdquo;*Zeigerw�sche*&rdquo;: `std::launder`
  * Speicher-Allokatoren (`std::allocator`)
  
---

#### Quellcode

[*ObjectPool.h*](ObjectPool.h)<br />
[*ObjectPool.cpp*](ObjectPool.cpp)<br />
[*ObjectPool_Test.cpp*](ObjectPool_Test.cpp)

---

## Allgemeines <a name="link1"></a>

In diesem Abschnitt beschreiben wir die Implementierung
mehrerer *Object Pool* Klassen, die sehr wohl f�r einen produktiven Einsatz
geeignet sind:

* Objektpool mit statischer Gr��e (feste Anzahl von Objekten).
* Objektpool mit dynamischer Gr��e (die Anzahl der Objekte kann sich zur Laufzeit vergr��ern).
* Objektpool mit statischer Gr��e Multi-Threading sicher.


Es gibt verschiedene Arten von Objektpool-Klassen.
Dieser Abschnitt beschreibt mehrere Objektpool-Realisierungen, die einen gro�en Speicherblock auf einmal allokieren
und kleinere Objekte direkt &bdquo;*in-place*&rdquo; erstellen.

Diese Objekte k�nnen an Clients ausgeh�ndigt und wiederverwendet werden,
wenn diese sie nicht mehr ben�tigen.

Dies erfordert *keine* zus�tzlichen Aufrufe des Speichermanagers,
um Speicher f�r einzelne Objekte zuzuweisen (`new`) oder freizugeben (`delete`).

---

## Ein Objektpool mit statischer Gr��e <a name="link2"></a>

---

## Ein Objektpool mit dynamischer Gr��e <a name="link3"></a>

Die Implementierung des Objektpools verwaltet einen Vektor mit Objektbl�cken vom Typ `T`.
Nach dem Start gibt es in diesem Vektor nur ein Element (Zeiger).
Dieser Zeiger verweist auf einen Speicherbereich,
der eine bestimmte Anzahl von Objekten (Typ `T`) aufnehmen kann, 
die dieser Pool verwaltet:

```cpp
std::vector<T*>  m_pool;
```

Zus�tzlich werden freie Objekte in einem Vektor mit Zeigern auf alle freien Objekte protokolliert:

```cpp
std::vector<T*>  m_free;
```

Da zu Beginn alle Objekte im Pool verf�gbar sind,
sind hier alle Adressen verf�gbar, die durch den ersten Block
im Pool angesprochen werden k�nnen.


Wir versuchen, in den folgenden Abbildungen das Szenario im
Objektpool &bdquo;bildlich&rdquo; darzustellen.

Zu Beginn wird im Pool eine bestimmte Menge an freien Objektbl�cken eingerichtet.
Da alle diese Bl�cke verf�gbar sind, sind deren Anfangsadressen 
in der zweiten Liste `m_free` aller verf�gbaren Objekte eingetragen.

<img src="cpp_object_pool_01.svg" width="600">

*Abbildung* 1: `m_pool` und `m_free` nach dem Start.

Jetzt stellen wir uns vor, dass zwei Objektbl�cke der Anwendung
zur Verf�gung gestellt wurden. In *Abbildung* 2 erkennen wir, 
dass am Ende von `m_free` zwei Adressen fehlen: Diese Adressen
sind nun in der Obhut der Anwendung, sie k�nnen allerdings,
wenn die Anwendung die Objekte nicht mehr ben�tigt,
in der `m_free`-Liste wieder eingereiht werden.

<img src="cpp_object_pool_02.svg" width="600">

*Abbildung* 2: `m_pool` und `m_free` nach Zuteilung zweier Objekte.

Sind alle Objekte des Pools zugeteilt, erkennen wir in
Abbildung 3, dass die Liste  `m_free` leer ist.
Es gibt keine freien Bl�cke mehr.

<img src="cpp_object_pool_03.svg" width="600">

*Abbildung* 3: Ein Pool ohne freie Objekte.

Wird jetzt ein neues Objekt angefordert,
muss zun�chst der Pool an sich wieder nachladen.

Die grafische Darstellung wird nun ein wenig komplexer.
In *Abbildung* 4 erkennen wir, dass zun�chst der Pool
neuen Speicher bekommen hat. Dieser wird komplett in die Liste
der freien Bl�cke aufgenommen und neue Bl�cke sind wieder verf�gbar.

<img src="cpp_object_pool_04.svg" width="800">

*Abbildung* 4: Ein Pool mit neuen Chunks.


Der Pool stellt Objekte �ber die Memberfunktion `acquireObject()` zur Verf�gung.
Wird `acquireObject()` aufgerufen und sind aber keine freien Objekte mehr vorhanden,
allokiert der Pool (durch die Methode `addChunk`) einen weiteren Vektor mit Objektbl�cken vom Typ `T`.

Die Klasse `ObjectPool` ist ein Klassentemplate.
Die beiden Templateparameter `T` und `TAllocator` parametrisieren
die im Pool zu speichernden Typen und den Typ des Allokators,
der f�r die Allokation und Freigabe von Speicherbl�cken verwendet wird.

```cpp
template <typename T, typename TAllocator = std::allocator<T>>
class ObjectPool final
```

---

### Die Memberfunktion `addChunk()`

Die Memberfunktion `addChunk()` zum Zuweisen eines neuen Chunks ist wie folgt implementiert.
Der erste Teil von `addChunk()` f�hrt die eigentliche Zuweisung eines neuen Chunks durch.
Ein &bdquo;Chunk&rdquo; ist ein nicht initialisierter Speicherblock, der mithilfe eines Allocators zugewiesen wird und gro� genug ist,
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

Durch das Hinzuf�gen eines Objekt-Chunks werden noch keine Objekte erstellt;
d.h. es werden keine Objektkonstruktoren aufgerufen.
Dies geschieht sp�ter in `acquireObject()`, wenn Instanzen vergeben werden.

Der zweite Teil von `addChunk()` erstellt Zeiger auf die neuen Instanzen von `T`. 
Er verwendet den `iota()`-Algorithmus, definiert in der Datei `<numeric>`.

Zur Erinnerung: `iota()` f�llt einen durch die ersten beiden Argumente vorgegebenen Bereich mit Werten.
Die Werte beginnen mit dem Wert des dritten Arguments und werden f�r jeden nachfolgenden Wert um Eins erh�ht.

Da wir mit `T*`-Zeigern arbeiten, springt das Erh�hen eines `T*`-Zeigers um Eins
zum n�chsten `T` im Speicherblock.

Abschlie�end wird der Wert von `m_newChunkSize` verdoppelt,
sodass der n�chste hinzugef�gte Block doppelt so gro� ist wie der aktuelle Block.
Dies geschieht aus Performancegr�nden und folgt dem Prinzip von `std::vector`.

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
gibt ein freies Objekt aus dem Pool zur�ck und allokiert einen neuen Block,
wenn keine freien Objekte mehr verf�gbar sind.

Wie bereits erl�utert, allokiert das Hinzuf�gen eines neuen Blocks lediglich einen Block nicht initialisierten Speichers.

`acquireObject()` ist daf�r verantwortlich, eine neue Instanz von `T` an der richtigen Stelle im Speicher zu erstellen.
Dies geschieht mithilfe der *Placement-New* Technik.

Alle an `acquireObject()` �bergebenen Argumente werden &bdquo;perfekt&rdquo;
an einen Konstruktor vom Typ `T` weitergeleitet.

*Hinweis*:<br />
`acquireObject()` verwendet die *Placement-New* Technik,
um eine neue Instanz eines Objekts vom Typ `T` an einem explizit vorgegebenen Speicherort zu erstellen.

Enth�lt Typ `T` Konstanten- oder Referenzen, f�hrt der Zugriff auf das neu erstellte Objekt
�ber den urspr�nglichen Zeiger zu undefiniertem Verhalten.

Um dieses Verhalten in definiertes Verhalten umzuwandeln, muss der Speicher mit `std::launder()`,
definiert in `<new>`, &bdquo;gewaschen&rdquo; werden.

Abschlie�end wird der &bdquo;gewaschene&rdquo; `T*`-Zeiger in einen `std::shared_ptr`-Objekt
mit einem benutzerdefinierten *Deleter* eingebettet.
Dieser *Deleter* gibt keinen Speicher frei, sondern ruft den Destruktor manuell mit `std::destroy_at()` auf und f�gt den Zeiger anschlie�end wieder in die Liste der verf�gbaren Objekte ein (`m_freeObjects`).

---

## Ein Objektpool mit statischer Gr��e Multi-Threading sicher <a name="link4"></a>

Wir orientieren uns bei der Realisierung eines thread-sicheren Objektpools 
an dem Objektpool aus Abschnitt
&bdquo;[Ein Objektpool mit statischer Gr��e](#link2)&rdquo;.
Da wir  die einzelnen Bl�cke quasi im Stile einer einfach verketteten Liste verwalten,
k�nnen wir das &bdquo;Compare-Exchange&rdquo;-Idiom anwenden.
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

Die Realisierung des Object Pools mit einer statischen Gr��e kann man einem Youtube-Tutorial entnehmen:<br />
&bdquo;[Efficient Object Pool - How-To](https://www.youtube.com/watch?v=37VhkrOiE8M)&rdquo;.

Die Realisierung des Object Pools mit dynamischer Gr��e wurde in dem Buch 
&bdquo;[*Professional C++*](https://www.amazon.de/Professional-C-Marc-Gregoire/dp/1394193173)&rdquo; von Marc Gregoire
vorgefunden.

Die multi-threading sichere Variante wiederum ist eine Weiterarbeit des Beispiels aus Youtube:<br />
&bdquo;[Compare and Swap Loop - Learn Modern C++](https://www.youtube.com/watch?v=56Q4Ty4hfMM)&rdquo;.


---

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---
