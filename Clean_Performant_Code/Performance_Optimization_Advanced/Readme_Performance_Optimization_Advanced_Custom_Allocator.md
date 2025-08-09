# Benutzerdefinierte Speicher-Allokatoren / *Custom Allocator*

[Zurück](Readme_Performance_Optimization_Advanced.md)

---

## Inhalt
  
  * [Allgemeines](#link1)
  * [Erste Schritte](#link2)
  * [ `allocate`, `deallocate`, `construct` und `destroy`](#link3)
  * [Ein erstes Beispiel](#link4)
  * [Realisierung eines benutzerdefinierten Speicher-Allokators](#link5)
  * [Literatur](#link6)

---

#### Quellcode

[*CustomAllocator.h*](CustomAllocator.h)<br />
[*CustomAllocator.cpp*](CustomAllocator.cpp)<br />
[*CustomAllocator_Test.cpp*](CustomAllocator_Test.cpp)

---

## Allgemeines <a name="link1"></a>

Warum verwendet man benutzerdefinierte Allokatoren?

Dafür gibt es mehrere Gründen:

  * Leistungsoptimierung:<br />Durch die Anpassung der Speicherzuweisung in bestimmten Situationen können Sie die Fragmentierung reduzieren und die Cache-Lokalität verbessern.
  * Speicherpooling:<br />Allokatoren können einen Speicherpool für eine schnelle Zuweisung und Freigabe verwalten, was in Echtzeitsystemen nützlich ist.
  * Debugging und Profiling:<br />Benutzerdefinierte Allokatoren helfen, die Speichernutzung zu verfolgen und Speicherlecks zu erkennen.
  * Spezialisierte Hardware:<br />Allokatoren können für die Interaktion mit nicht standardmäßigem Speicher wie GPU-Speicher oder *Shared Memory* entwickelt werden.

---

## Erste Schritte <a name="link2"></a>

Jeder Container der Standardbibliothek verwendet einen Allocator-Typ als *Template Type Parameter*,
wobei der Standardwert in der Regel ausreicht.

Die Definition des Klassentemplate `std::vector`
sieht beispielsweise so aus:

```cpp
template <class T, class Allocator = allocator<T>>
class vector;
```

Die Containerkonstruktoren ermöglichen deshalb die Übergabe
eines Objekts vom Typ `Allocator`.

Dadurch kann die Art und Weise,
wie ein Container Speicher reserviert, anpassen oder sogar
selbst in die Hand nehmen.

Jede Speicherzuweisung eines Containers erfolgt durch einen Aufruf der Memberfunktion `allocate()` des `Allocator`-Objekts.

Umgekehrt erfolgt jede Freigabe durch einen Aufruf der Memberfunktion `deallocate()` des `Allocator`-Objekts.

Wenn ein Container der Standardbibliothek einen `Allocator`-Parameter verwendet, wird dieser standardmäßig immer auf `std::allocator<T>` gesetzt, sofern nicht angegeben.

Die Memberfunktionen `allocate()` und `deallocate()` von `std::allocator<T>` sind einfache Wrapper für `new` und `delete`.

---

## `allocate`, `deallocate`, `construct` und `destroy` <a name="link3"></a>

`allocate`:<br />
Zweck: Diese Methode ist für die Zuweisung von rohem (uninitialisiertem) Speicher zuständig.<br />
Aufruf: STL-Container rufen `allocate` auf, wenn sie Speicher zum Speichern von Elementen benötigen.

`deallocate`:<br />
Zweck: Diese Methode ist für die Freigabe von zuvor zugewiesenem rohem Speicher zuständig.<br />
Aufruf: STL-Container rufen `deallocate` auf, wenn sie zuvor zugewiesenen Speicher freigeben müssen.

`construct`:<br />
Zweck: Diese Methode ist für die Erstellung eines Objekts im zugewiesenen Speicher zuständig.<br />
Aufruf: STL-Container rufen `construct` auf, wenn sie ein Objekt im zugewiesenen Speicher erstellen müssen.

`destroy`:<br />
Zweck: Diese Methode ist für die Zerstörung eines Objekts zuständig.<br />
Aufruf: STL-Container rufen `destroy` auf, wenn sie ein Objekt zerstören und seine Ressourcen freigeben müssen.

---

## Ein erstes Beispiel <a name="link4"></a>

Beachten Sie, dass `allocate()` lediglich einen ausreichend großen Block nicht initialisierten Speichers allokiert und keinen Objektkonstruktor aufruft.

Ebenso gibt `deallocate()` den Speicherblock einfach frei und ruft keinen Destruktor auf.

Sobald ein Speicherblock allokiert ist, kann ein &bdquo;Placement new&rdquo; Operatoraufruf verwendet werden, um ein Objekt an Ort und Stelle zu erstellen.

Der folgende Codeausschnitt zeigt ein etwas konstruiertes Beispiel.

```cpp
01: void test()
02: {
03:     // create an allocator to use
04:     std::allocator<Person> alloc;
05: 
06:     // allocate an uninitialized memory block for 1 instance of 'Person'
07:     auto* memory{ alloc.allocate(1) };
08: 
09:     // use 'placement new' operator to construct a 'Person' object in place
10:     ::new (memory) Person{};
11: 
12:     // destroy 'Person' instance
13:     std::destroy_at(memory);
14: 
15:     // deallocate memory block
16:     alloc.deallocate(memory, 1);
17: 
18:     memory = nullptr;
19: }
```

---

## Realisierung eines benutzerdefinierten Speicher-Allokators <a name="link5"></a>

```cpp
01: template<typename T>
02: class SimpleAllocator {
03: public:
04:     using value_type = T;
05: 
06:     SimpleAllocator() = default;
07: 
08:     template<typename U>
09:     constexpr SimpleAllocator(const SimpleAllocator<U>&) noexcept {}
10: 
11:     T* allocate(std::size_t n)
12:     {
13:         if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
14:             throw std::bad_array_new_length();
15: 
16:         if (auto p = static_cast<T*>(std::malloc(numBytes)))
17:         {
18:             return p;
19:         }
20: 
21:         throw std::bad_alloc();
22:     }
23: 
24:     void deallocate(T* p, std::size_t) noexcept
25:     {
26:         std::free(p);
27:     }
28: 
29:     template<typename U, typename... TArgs>
30:     void construct(U* p, TArgs&&... args)
31:     {
32:         new(p) U(std::forward<TArgs>(args)...);
33:     }
34: 
35:     template<typename U>
36:     void destroy(U* p) noexcept
37:     {
38:         p->~U();
39:     }
40: 
41:     friend bool operator==(const SimpleAllocator&, const SimpleAllocator&) { return true; }
42:     friend bool operator!=(const SimpleAllocator&, const SimpleAllocator&) { return false; }
43: };
```

Wir setzen diesen Allokator im folgenden Beispiel ein:

```cpp
01: void test()
02: {
03:     SimpleAllocator<int> alloc;
04:     std::vector<int, SimpleAllocator<int>> vec(alloc);
05: 
06:     vec.push_back(1);
07:     vec.push_back(2);
08:     vec.push_back(3);
09: 
10:     for (int i : vec) {
11:         std::println("{} ", i);
12:     }
13: }
```

*Ausgabe*:

```
Allocating 16 bytes
Allocating 4 bytes
Constructing element
Allocating 8 bytes
Constructing element
Constructing element
Destroying element
Deallocating memory
Allocating 12 bytes
Constructing element
Constructing element
Constructing element
Destroying element
Destroying element
Deallocating memory
1
2
3
Destroying element
Destroying element
Destroying element
Deallocating memory
Deallocating memory
```

---

## Literatur <a name="link6"></a>

Die Anregungen zu den Beispielen aus diesem Abschnitt stammen
aus dem Artikel
[Custom Allocators in C++: High Performance Memory Management](https://johnfarrier.com/custom-allocators-in-c-high-performance-memory-management/)
von John Farrier.

---

[Zurück](Readme_Performance_Optimization_Advanced.md)

---
