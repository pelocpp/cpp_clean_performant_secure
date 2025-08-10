# Realisierung eines Objektpools / *Object Pool*

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Zu den Details](#link2)
  * [Literatur](#link3)

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
einer Object Pool Klasse, die sehr wohl f�r einen produktiven Einsatz
geeignet ist.

---

## Zu den Details <a name="link2"></a>

Die Implementierung des Objektpools verwaltet einen Vektor mit Objektbl�cken vom Typ `T`.
Nach dem Start gibt es in diesem Vektor nur ein Element (Zeiger).
Dieser Zeiger verweist auf einen Speicherbereich,
der eine bestimmte Anzahl von Objekten (Typ `T`) aufnehmen kann, 
die dieser Pool verwaltet:

```cpp
std::vector<T*>  m_pool;
```

Zus�tzlich werden freie Objekte in einem Vektor mit Zeigern auf alle freien Objekte protokolliert.

```cpp
std::vector<T*>  m_free;
```

Da zu Beginn alle Objekte im Pool verf�gbar sind,
sind hier alle Adressen verf�gbar, die durch den ersten Block
im Pool angesprochen werden k�nnen.


Wir versuchen, in den beiden folgenden Abbildungen das Szenario im
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

---

Der Pool stellt Objekte �ber die Memberfunktion `acquireObject()` zur Verf�gung.
Wird `acquireObject()` aufgerufen und sind aber keine freien Objekte mehr vorhanden,
allokiert der Pool einen weiteren Vektor mit Objektbl�cken vom Typ `T`.

Die Klasse `ObjectPool` ist ein Klassentemplate.
Die beiden Templateparameter `T` und `TAllocator` parametrisieren
die im Pool zu speichernden Typen und den Typ des Allokators,
der f�r die Allokation und Freigabe von Speicherbl�cken verwendet wird.

```cpp
template <typename T, typename TAllocator = std::allocator<T>>
class ObjectPool final
```

Die Memberfunktion `addChunk()` zum Zuweisen eines neuen Chunks ist wie folgt implementiert.
Der erste Teil von `addChunk()` f�hrt die eigentliche Zuweisung eines neuen Chunks durch.
Ein �Chunk� ist ein nicht initialisierter Speicherblock, der mithilfe eines Allocators zugewiesen wird und gro� genug ist,
um `m_newChunkSize` Instanzen von `T` aufzunehmen.

Durch das Hinzuf�gen eines Objekt-Chunks werden noch keine Objekte erstellt;
d.h. es werden keine Objektkonstruktoren aufgerufen.
Dies geschieht sp�ter in `acquireObject()`, wenn Instanzen vergeben werden.

Der zweite Teil von `addChunk()` erstellt Zeiger auf die neuen Instanzen von `T`. 
Er verwendet den `iota()`-Algorithmus, definiert in der Datei `<numeric>`.

Zur Erinnerung: `iota()` f�llt einen durch die ersten beiden Argumente vorgegebenen Bereich mit Werten.

Die Werte beginnen mit dem Wert des dritten Arguments und werden f�r jeden nachfolgenden Wert um Eins erh�ht.

Da wir mit `T*`-Zeigern arbeiten, springt das Erh�hen eines `T*`-Zeigers um Eins
zum n�chsten `T` im Speicherblock. Abschlie�end wird der Wert von `m_newChunkSize` verdoppelt,
sodass der n�chste hinzugef�gte Block doppelt so gro� ist wie der aktuelle Block.

Dies geschieht aus Performancegr�nden und folgt dem Prinzip von `std::vector`.

---

## Literatur <a name="link3"></a>

Die Realisierung des Object Pools wurde in dem Buch 
&bdquo;[*Professional C++*](https://www.amazon.de/Professional-C-Marc-Gregoire/dp/1394193173)&rdquo; von Marc Gregoire
vorgefunden.

---

[Zur�ck](Readme_Performance_Optimization_Advanced.md)

---
