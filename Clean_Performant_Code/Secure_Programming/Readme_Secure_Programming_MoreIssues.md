# Weitere Probleme Probleme des C/C++ *Secure Programming*

[Zurück](./Readme_Secure_Programming.md)

---

## Inhalt

### Weitere Probleme des *Secure Programming* in C/C++

  * [Vorsicht in der Verwendung von Zeigern](#link1)
  * [Auch C++ Referenzen sind nicht zu 100% sicher](#link2)
  * [Verschwindendes `std::memset`](#link3)

  * [Speicherlecks (*Memory Leaks*)](#link4)
  * [Race Conditions](#link5)
  * [Injection attacks](#link6)
  * [Input validation and sanitization](#link7)
  * [Memory management](#link8)

---

#### Quellcode

[*SecureProgrammingMoreIssues.cpp*](SecureProgrammingMoreIssues.cpp)

---

## Weitere Probleme des *Secure Programming* in C/C++

### Vorsicht in der Verwendung von Zeigern <a name="link1"></a>


*Beispiel*:

```cpp
01: void decay(const int* ages) {
02:     // Size of the pointer = 8
03:     std::println("Size of an 'int*' pointer:          {}", sizeof(ages));
04: 
05:     // Compile Error
06:     // std::cout << std::size(ages) << '\n';
07: }
08: 
09: void test() {
10: 
11:     int ages[] = { 15, 30, 50 };
12:     // Number of elements = 3
13:     std::println("Number of array elements:           {}", std::size(ages));
14: 
15:     // Size of an element = 4
16:     std::println("Size of a single array element:     {}", sizeof(ages[0]));
17: 
18:     // Size of array = 12 (= 3 * 4)
19:     std::println("Number of bytes used by this array: {}", sizeof(ages));
20:     decay(ages);
21: }
```


*Ausgabe*:

```
Number of array elements:           3
Size of a single array element:     4
Number of bytes used by this array: 12
Size of an 'int*' pointer:          8
```

Man beachte den Gebrauch zweier systemnaher Funktionen:

  * `std::size` &ndash; Gibt die Anzahl der Elemente in einem Container an. 
  * `sizeof` &ndash; Gibt die konstante Größe eines Typs oder Objekts zur Übersetzungszeit in Bytes an.

---

### Auch C++ Referenzen sind nicht zu 100% sicher <a name="link2"></a>

*Beispiel*:

```cpp
01: struct Data {
02:     Data(int& value) : m_value(value) {}
03:     int& m_value;
04: };
05: 
06: Data function() {
07: 
08:     int value = 123;
09: 
10:     Data data(value);
11: 
12:     std::println("value: {}", value);
13: 
14:     return data;                          // implicitly returning reference to local value
15: }
16: 
17: void test()
18: {
19:     Data data = function();
20:     std::println("{}", data.m_value);     // Oooooops
21: }
```

*Ausgabe*:

```
value: 123
91
```

---

### Verschwindendes `std::memset` <a name="link3"></a>

*Beschreibung*:

[CWE-14: Compiler Removal of Code to Clear Buffers](https://cwe.mitre.org/data/definitions/14.html)

Worum geht es?


```cpp
01: bool connectToServer(char* pwd) {
02:     std::println("{}", pwd);
03:     return true;
04: }
05: 
06: bool getPasswordFromUser(char* pwd, size_t pwdSize) {
07:     strncpy(pwd, "My super secret password", pwdSize);
08:     std::println("{}", pwd);
09:     return true;
10: }
11: 
12: void test()
13: {
14:     char pwd[64];
15: 
16:     if (getPasswordFromUser(pwd, sizeof(pwd))) {
17:         if (connectToServer(pwd)) {
18:             // interaction with server
19:         }
20:     }
21: 
22:     std::memset(pwd, 0, sizeof(pwd)); // <- Removed by the optimizer !!!
23: }
```

`std::memset` kann möglicherweise wegoptimiert werden,
wenn der von dieser Funktion geänderte Speicherbereich für den Rest seiner Lebensdauer
nicht mehr verwendet wird.

Aus diesem Grund kann diese Funktion *nicht* zum Bereinigen des Speichers verwendet werden
(z. B. um ein Array, das ein Passwort gespeichert hat, mit Nullen zu füllen).

*Lösung*:

Diese Optimierung ist für `std::memset_explicit` und `std::memset_s` verboten:
Sie führen garantiert den Speicherschreibvorgang aus.


*Beachte*:<br />
Visual C++ stellt diese beiden Funktionen nicht zur Verfügung!

---

[Zurück](./Readme_Secure_Programming.md)

---
