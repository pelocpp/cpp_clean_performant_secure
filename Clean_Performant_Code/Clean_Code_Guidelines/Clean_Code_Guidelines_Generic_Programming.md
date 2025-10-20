# Richtlinien zur generischen Programmierung

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link19)

---

#### Quellcode

[*Clean_Code_Guidelines_GenericProgramming.cpp*](Clean_Code_Guidelines_GenericProgramming.cpp)

---

## Allgemeines <a name="link1"></a>

---

## Wozu dienen Tempates?

Man kann den Abstraktionslevel des Quellcodes steigern.

*Beispiel*:

```cpp
01: bool isGreaterThanFifty(int n) {
02: 
03:     return n > 50;
04: }
05: 
06: template <typename T>
07: bool isGreaterThanFifty(T n) {
08: 
09:     return n > T{ 50 };
10: }
11: 
12: bool isGreaterThanFifty(auto n) {
13: 
14:     return n > decltype(n) { 50 };
15: }
```

*Beispiel*:

```cpp
01: template <typename T>
02: class Point2D
03: {
04: public:
05:     Point2D(T x, T y) : m_x{ x }, m_y{ y } {}
06: 
07:     auto x() { return m_x; }
08:     auto y() { return m_y; }
09:     // ...
10: 
11: private:
12:     T m_x{};
13:     T m_y{};
14: };
```

---

[Zurück](../Clean_Code/Readme_Clean_Code.md)

---
