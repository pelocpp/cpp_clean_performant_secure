# Object Pool

[Zurück](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt
  
  * [Allgemeines](#link1)

---

#### Quellcode


---

## Allgemeines <a name="link1"></a>

In diesem Abschnitt beschreiben wir die Implementierung
einer Object Pool Klasse, die sehr wohl für einen produktiven Einsatz
geeignet ist.

---

## Einige Details <a name="link1"></a>

Die Implementierung verwaltet einen Vektor mit Objektblöcken vom Typ `T`:

```cpp
std::vector<T*>  m_pool;
```

Zusätzlich werden freie Objekte in einem Vektor mit Zeigern auf alle freien Objekte protokolliert.

```cpp
std::vector<T*>  m_free;
```

Wir versuchen, in den beiden folgenden Abbildungen das Szenario im
Objektpool &bdquo;bildlich&rdquo; darzustellen.

Zu Beginn wird im Pool eine bestimmte Menge an freien Objektblöcken eingerichtet.
Da alle diese Blöcke verfügbar sind, sind deren Anfangsadressen 
in der zweiten Liste `m_free` aller verfügbaren Objekte eingetragen.

<img src="cpp_object_pool_01.svg" width="600">

*Abbildung* 1: m_pool und m_free nach dem Start.


Jetzt stellen wir uns vor, dass zwei Objektblöcken der Anwendung
zur Verfügung gestellt wurden. In *Abbildung* 2 erkennen wir, 
dass am Ende von `m_free` zwei Adressen fehlen: Diese Adressen
sind nun in der Obhut der Anwendung, sie können allerdings,
wenn die Anwendung die Objekte nicht mehr benötigt,
in die  `m_free`-Liste zurückgegeben werden.

<img src="cpp_object_pool_02.svg" width="600">

*Abbildung* 2: m_pool und m_free nach Zuteilung zweier Objekte.




Der Pool gibt Objekte über die Memberfunktion `acquireObject()` aus.
Wird `acquireObject()` aufgerufen, sind aber keine freien Objekte mehr vorhanden,
allokiert der Pool einen weiteren Vektor mit Objektblöcken vom Typ `T`.

Die Klasse `ObjectPool` ist ein Klassentemplate.
Die beiden Templateparameter `T` und `TAllocator` parametrisieren
die im Pool zu speichernden Typen und den Typ des Allokators,
der für die Allokation und Freigabe von Speicherblöcken verwendet wird.

```cpp
template <typename T, typename TAllocator = std::allocator<T>>
class ObjectPool final
```

Die Memberfunktion `addChunk()` zum Zuweisen eines neuen Chunks ist wie folgt implementiert.
Der erste Teil von `addChunk()` führt die eigentliche Zuweisung eines neuen Chunks durch.
Ein „Chunk“ ist ein nicht initialisierter Speicherblock, der mithilfe eines Allocators zugewiesen wird und groß genug ist,
um `m_newChunkSize` Instanzen von `T` aufzunehmen.

Durch das Hinzufügen eines Objekt-Chunks werden noch keine Objekte erstellt;
d.h. es werden keine Objektkonstruktoren aufgerufen.
Dies geschieht später in `acquireObject()`, wenn Instanzen vergeben werden.

Der zweite Teil von `addChunk()` erstellt Zeiger auf die neuen Instanzen von `T`. 
Er verwendet den iota()-Algorithmus, definiert in
<numeric>.

Zur Erinnerung: iota() füllt einen durch die ersten beiden Argumente vorgegebenen Bereich mit Werten.

Die Werte beginnen mit dem Wert des dritten Arguments und werden für jeden nachfolgenden Wert um eins erhöht.

Da wir mit T*-Zeigern arbeiten, springt das Erhöhen eines T*-Zeigers um eins zum nächsten T im Speicherblock. Abschließend wird der Wert von m_newChunkSize verdoppelt, sodass der nächste hinzugefügte Block doppelt so groß ist wie der aktuell hinzugefügte Block.

Dies geschieht aus Performancegründen und folgt dem Prinzip von std::vector.


---

[Zurück](Readme_Performance_Optimization_Advanced.md)

---
