# *SafeArray*: Hilfsbibliotheken für Arrays zum Schutz vor falschen Indizes

[Zurück](../Secure_Programming/Readme_Secure_Programming.md)

---

*Hinweis*:

Dies ist eine Realisierung in C.

---

## Inhalt

  * [Allgemeines](#link1)
  * [Gültigkeit von Array-Indizes in der Programmiersprache C](#link2)
  * [Hilfestellungen für eine Absicherung](#link3)
  * [*vec* &ndash; Eine sichere Array-Implementierung für C](#link4)
  * [Vor- und Nachteile der *vec*&ndash;Bibliothek ](#link5)
  * [Struktur *IntVec* &ndash; eine einfache Realisierung](#link6)
  * [Literatur](#link7)

---

#### Quellcode

[*Vec.h*](Vec.h)<br />
[*Vec.c*](Vec.c)<br />

[*IntVec.h*](IntVec.h)<br />
[*IntVec.c*](IntVec.c)

---

## Allgemeines <a name="link1"></a>

Die Programmiersprache C kennt den Datentyp *Arrays*. Diese besitzen zum Definitionszeitpunkt eine feste Größe:

```c
int numbers[10];
```

Der Zugriff auf die Daten in einem Array erfolgt mit Indizes:

```c
int numbers[10];
...
size_t index = 5;
int value = numbers[index];  // fine
...
index = 20;
value = numbers[index];      // crashes
```

## Gültigkeit von Array-Indizes in der Programmiersprache C <a name="link2"></a>


In der Programmiersprache C wird die Gültigkeit von Array-Indizes (*Bounds Checking*) aus folgenden Gründen nicht standardmäßig überprüft:

### 1. Maximale Ausführungsgeschwindigkeit

  * Jede Überprüfung erfordert zusätzliche CPU-Instruktionen vor dem Speicherzugriff.
  * C wurde entwickelt, um die Leistung von Assembler zu erreichen.
  * Der Verzicht auf Prüfungen spart in Schleifen wertvolle Taktzyklen.

### 2. Direkter Speicherzugriff (Pointer-Arithmetik)

  * Arrays in C sind intern lediglich Zeiger (Pointer) auf die Startadresse.
  * Der Ausdruck `array[i]` wird vom Compiler direkt in `*(array + i)` übersetzt.
  * C berechnet einfach die Zieladresse, ohne die Größe des Blocks zu kennen.

### 3. Keine Metadaten im Laufzeitsystem

  * C-Arrays speichern ihre eigene Länge nicht im Speicher (kein Overhead).
  * Einem reinen Zeiger fehlt zur Laufzeit die Information über die Array-Grenzen.
  * Ohne diese Metadaten kann die Laufzeitumgebung keine Validierung durchführen.

### 4. Philosophie der Programmiersprache

  * Das Design-Prinzip von C lautet: &bdquo;Vertraue dem Programmierer&rdquo;, oder, um es in den Worten von Dennis Ritchie zu formulieren: &bdquo;A Programmer knows what he does&rdquo;.
  * Die Verantwortung für die Sicherheit liegt komplett beim Entwickler.
  * Der Programmierer muss Grenzprüfungen bei Bedarf selbst implementieren.

### 5. Konsequenzen bei Fehlern

  * Ein Zugriff außerhalb der Grenzen führt zu undefiniertem Verhalten (*Undefined Behavior*).
  * Das Programm liest oder überschreibt fremde Speicherbereiche.
  * Dies führt oft zu Abstürzen (*Segmentation Fault*) oder Sicherheitslücken (*Buffer Overflow*).


## Hilfestellungen für eine Absicherung <a name="link3"></a>

Zunächst kann man festhalten, dass vor jedem Zugriff auf ein Array der Index gegen die Array-Größe validiert werden muss.

Dies ist natürlich sehr aufwändig, wenn man dies überall im Quellcode bei jedem Zugriff auf das Feld machen würde:

*Beispiel*:

```c
#define MAX_ELEMENTS 10

int myArray[MAX_ELEMENTS];
...
int index = 12;
...
if (index >= 0 && index < MAX_ELEMENTS) {
    myArray[index] = 123;
} else {
    // Fehlerbehandlung: Index außerhalb des gültigen Bereichs
}
```

Ein besserer Lösungsansatz besteht darin, anstelle der direkten Verwendung des C-Array Datentyps
den Umweg über eine C-Hilfsbibliothek zu gehen.

Derartige Implementierungen können Arrays mit fester oder dynamischer Kapazität realisieren,
also den Funktionsumfang des intrinsischen C-Array Datentyps noch erweitern.

Das Ziel besteht darin, einige der häufigsten und fehleranfälligen Aspekte typischen C-Codes zu eliminieren.
Die Länge und Kapazität eines solchen C-Arrays werden automatisch verwaltet und jeglicher lesender als auch schreibender
Zugriff auf die Daten wird bzgl. der Zugriffsgrenzen überprüft.

Im Wesentlichen handelt es sich bei derartigen C-Hilfsbibliotheken
um eine schlanke Verwaltungsschicht für Daten mit fester Kapazität.

## *vec* &ndash; Eine sichere Array-Implementierung für C <a name="link4"></a>

In den Beispielen zu diesem repository wird eine Bibliothek `vec` von *RXI* verwendet:

Die neueste Version dieser Bibliothek befindet sich unter
[vec - A type-safe dynamic array implementation for C](https://github.com/rxi/vec),

Sie ist typsicher, dynamisch und für mehrere C-Datentypen ausgelegt, also generisch.

Das einzige Manko dieser Bibliothek besteht darin, dass ausgerechnet die Indizes nicht auf Gültigkeit überprüft werden.
Dies kann man aber einfach ergänzen.

*Beispiel*:

```cpp
01: #include "Vec.h"
02: #include <stdio.h>
03: 
04: void test_vec()
05: {
06:     vec_int_t v;
07: 
08:     vec_init(&v);
09:     vec_push(&v, 123);
10:     vec_push(&v, 456);
11: 
12:     printf("v[0]: %d\n", v.data[0]);
13:     printf("v[1]: %d\n", v.data[1]);
14: }
```

*Ausgabe*:

```
v[0]: 123
v[1]: 456
```

Die beiden Dateien `vec.h` und `vec.c` sind unverändert aus dem Originalrepository übernommen worden.
Einzig und allen für den indizierten Zugriff auf die Daten wurden Ergänzungen vorgenomen:

```cpp
01: // added: PeLo
02: #define vec_at(v, i) \
03:   ((i) >= 0 && (i) < (v).length ? (v).data[i] : (abort(), (v).data[0]))
04: 
05: 
06: // added: PeLo
07: // Note: To create a safe "setter" in C, typically a do - while (0) macro pattern is used.
08: // This is a standard C idiom that allows the macro to behave like a single statement,
09: // making it safe to use inside if / else blocks without causing syntax errors.
10: // This ensures the macro always requires a semicolon and acts as a single block.
11: // Without it, an if (condition) vec_set_safe(...); else ... might break
12: // because the else would attach to the internal if of the macro.
13: //
14: #define vec_set_safe(v, i, val) do {               \
15:     if ((i) >= 0 && (i) < (v).length) {            \
16:         (v).data[i] = (val);                       \
17:     } else {                                       \
18:         fprintf(stderr, "Index %d out of bounds for length %d\n", (int)(i), (v).length); \
19:         abort();                                   \
20:     }                                              \
21: } while (0)
22: 
23: 
24: // added: PeLo
25: // Alternative: A "Silent" Setter
26: // Usage: if (!vec_try_set(v, 10, 5)) { /* handle error */ }
27: //
28: #define vec_try_set(v, i, val) \
29:     ((i) >= 0 && (i) < (v).length ? ((v).data[i] = (val), 1) : 0)
```


---

## Vor- und Nachteile der *vec*&ndash;Bibliothek <a name="link5"></a>

Die *vec*-Hilfsbibliothek unterstützt 6 der häufigsten C-Standarddatentypen:
`int`, `char`, `float`, `double`, `void*` und  `char*`.

```cpp
typedef vec_t(void*) vec_void_t;
typedef vec_t(char*) vec_str_t;
typedef vec_t(int) vec_int_t;
typedef vec_t(char) vec_char_t;
typedef vec_t(float) vec_float_t;
typedef vec_t(double) vec_double_t;
```

Dies ist eine Stärke der Bibliothek, ihr Nachteil, sofern man dies auf diese Weise überhaupt so formulieren möchte,
besteht in der sehr anspruchsvollen Realisierung: Umfangreicher Einsatz von C-Makros sowie der *Stringification*-Technik.
Natürlich hat dies einen Grund: Nur auf diese Weise kann man in C eine (ansatzweise) generische Bibliothek erstellen.

Vor diesem Hintergrund sollte man mit Änderungen an dieser Vorlage äußert behutsam umgehen, 
sofern dies überhaupt notwendig sein sollte.

Ein weiterer Nachteil der vorliegenden Realisierung zum Studium des Quelltextes besteht darin,
dass sich C-Makros nicht debuggen lassen.

Aus diesem Grund habe ich eine zweite Bibliothek diesem Repository hinzugefügt: `int_vec`.
Die Funktionalität der Struktur `int_vec` und den auf ihr operierenden Funktionen ist ähnlich zur *vec*&ndash;Bibliothek,
nur dass diese einzig und allein `int`-Arrays unterstützt. Dafür sind die Funktionen in C geschrieben, die man debuggen kann.


## Struktur *IntVec* &ndash; eine einfache Realisierung <a name="link6"></a>

Zum Zwecke des Studiums einer Hilfsbibliothek für sichere Array-Realisierungen
kann man in den beiden Dateien `IntVec.h` und `IntVec.c` eine abgespeckte Variante der *vec*-Bibliothek von *RIX* betrachten.

*Beispiel*: Header-Datei

```cpp
01: // -------------------------------------------------------------------------
02: // types
03: 
04: struct int_vec_
05: {
06:     int* data; 
07:     size_t length;
08:     size_t capacity;
09: };
10: 
11: typedef struct int_vec_ int_vec;
12: 
13: // -------------------------------------------------------------------------
14: // functions
15: 
16: // lifecycle
17: extern void int_vec_init          (int_vec* ptr);
18: extern void int_vec_free          (int_vec* ptr);
19: 
20: // capacity management
21: extern int  int_vec_reserve       (int_vec* vec, size_t new_capacity);
22: extern int  int_vec_resize        (int_vec* vec, size_t new_length);
23: extern int  int_vec_shrink_to_fit (int_vec* vec);
24: extern void int_vec_clear         (int_vec* ptr);
25: extern void int_vec_truncate      (int_vec* ptr, size_t len);
26: 
27: // vector based operations
28: extern int int_vec_push           (int_vec* ptr, int val);
29: extern int int_vec_pop            (int_vec* vec, int* out_value); 
30: extern int int_vec_insert         (int_vec* vec, size_t index, int value);
31: extern int int_vec_remove         (int_vec* vec, size_t index);
32: 
33: // element access / queries
34: extern const int* int_vec_get     (const int_vec* vec, size_t index);
35: extern       int  int_vec_set     (      int_vec* vec, size_t index, int value);
36: extern       int  int_vec_front   (const int_vec* vec, int* out);
37: extern       int  int_vec_back    (const int_vec* vec, int* out);
38: extern       int  int_vec_find    (const int_vec* vec, int value, size_t* index_ptr);
39: 
40: // iterator design
41: extern const int* int_vec_next    (const int_vec* vec, size_t* index);
42: extern const int* int_vec_prev    (const int_vec* vec, size_t* index_ptr);
```

Ein Beispiel für eine Anwendung:

```cpp
01: #include "IntVec.h"
02: #include <stdio.h>
03: 
04: void test()
05: {
06:     int_vec vec;
07: 
08:     int_vec_init(&vec);
09:     int_vec_push(&vec, 123);
10:     int_vec_push(&vec, 456);
11:     int_vec_push(&vec, 789);
12: 
13:     printf("v[0]: %d\n", vec.data[0]);
14:     printf("v[1]: %d\n", vec.data[1]);
15:     printf("v[2]: %d\n", vec.data[2]);
16: 
17:     size_t i = 0;
18:     const int* value;
19:     while ((value = int_vec_next(&vec, &i)) != NULL) {
20:         printf("%zu: %d\n", i, *value);
21:     }
22: 
23:     int_vec_free(&vec);
24: }
```

*Ausgabe*:

```
v[0]: 123
v[1]: 456
v[2]: 789
1: 123
2: 456
3: 789
```

---

## Literatur <a name="link7"></a>

[vec - A type-safe dynamic array implementation for C](https://github.com/rxi/vec)<br/>
    (abgerufen am 14.05.2026).


[stack_array - Small C helper library for fixed-capacity arrays](https://github.com/jamesnolanverran/stack_array)<br/>
    (abgerufen am 14.05.2026).


[tsarray - Generic type-safe dynamic array library for C](https://github.com/israel-lugo/tsarray)<br/>
    (abgerufen am 14.05.2026).


---

[Zurück](../Secure_Programming/Readme_Secure_Programming.md)

---
