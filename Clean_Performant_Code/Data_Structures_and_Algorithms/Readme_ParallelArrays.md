# Performanzbetrachtungen bei Objekten unterschiedlicher Größe / Parallele Arrays

[Zurück](Readme_Data_Structures_and_Algorithms.md)

---

#### Quellcode

[*ParallelArrays.cpp*](ParallelArrays.cpp)<br />

---

## Allgemeines <a name="link1"></a>

Wir betrachten als Beispiel Iterationen über array-ähnliche Datenstrukturen,
die aus Objekten bestehen.

Zwei Faktoren spielen eine Rolle:

  * räumliche Lokalität (*spatial locality*).
  * zeitliche Lokalität (*temporal locality*).

Bei der Iteration über zusammenhängend im Speicher abgelegte Elemente
erhöht sich die Wahrscheinlichkeit, dass die benötigten Daten bereits zwischengespeichert sind,
wenn es uns dank räumlicher Lokalität gelingt, unsere Objekte klein zu halten.

Dies hat bisweilen erhebliche Auswirkungen auf die Performance.

---

## Erstes Beispiel: Vergleich von `SmallObject`- und `BigObject`-Objekten <a name="link1"></a>

Wir betrachten in diesem Beispiel die Iteration
von `std::vector`-Containern, die aus
`SmallObject`- und `BigObject`-Objekten bestehen.


`SmallObject` und `BigObject` sind bis auf die Größe eines enthaltenen Datenarrays identisch.
Beide Strukturen enthalten einen `int`-Wert namens `m_score`,
den wir zu Testzwecken mit einem zufälligen Wert initialisieren.

Nun können wir vergleichen, wie lange es dauert,
die `m_score`-Werte von `SmallObject`-Objekten im Vergleich zu `BigObject`-Objekten
zu summieren:


```cpp
01: template <size_t Size>
02: class Object {
03: private:
04:     std::array<char, Size> m_data{};
05:     int m_score{ std::rand() };
06: public:
07:     auto getScore() const { return m_score; }
08: };
09: 
10: using SmallObject = Object<4>;
11: using BigObject = Object<256>;
12: 
13: constexpr auto Size = 1'000'000;
14: 
15: auto smallObjects = std::vector<SmallObject>(Size);
16: auto bigObjects = std::vector<BigObject>(Size);
17: 
18: template <class T>
19: auto sumScores(const std::vector<T>& objects) {
20: 
21:     ScopedTimer watch{};
22: 
23:     size_t sum{ 0 };
24: 
25:     for (const auto& obj : objects) {
26:         sum += obj.getScore();
27:     }
28: 
29:     return sum;
30: }
31: 
32: static void test_parallel_arrays_02() {
33: 
34:     size_t sum{ 0 };
35:     sum += sumScores(smallObjects);
36:     sum += sumScores(bigObjects);
37: }
```

---

## Zweites Beispiel: Zerlegung größerer Objekte <a name="link1"></a>

Wir haben gesehen, dass es von Vorteil ist,
die Größe unserer Klassen klein zu halten.´,
auch wenn dies oft leichter gesagt als getan ist.

Wir betrachten nun eine Klasse `User` und überlegen,
wie wir sie in kleinere Teile aufteilen können.

*Variante* 1:

```cpp
struct User
{
    std::string m_name;
    std::string m_username;
    std::string m_password;
    std::string m_securityQuestion;
    std::string m_securityAnswer;
    short       m_level{};
    bool        m_isPlaying{};
};
```

<img src="Struct_User_Memory_Layout_Short_Variant_01.png" width="600">

*Abbildung* 1: Struktur `User` in einem ersten Entwurf: Übersichtsdarstellung.

<img src="Struct_User_Memory_Layout_Long_Variant_01.png" width="600">

*Abbildung* 2: Struktur `User` in einem ersten Entwurf: Detaildarstellung.

Um bessere Laufzeiten zu erzielen, sollten man die Struktur `User` aufteilen.
In einem ersten Blick könnte man meinen, das Programm muss zwingend langsamer werden,
da man bei mehreren Objekten/Strukturen mehr indirekte Zugriffe hat.
Das ist prinzipiell auch richtig beobachtet. Eine Aufteilung sollte diejenigen Informationen auslagern,
auf die man zur Laufzeit seltener zugreift.

In unserem Beispiel werden vier Daten der `User`-Struktur in eine zweite Struktur `AuthInfo` ausgelagert.
Das Passwort als auch die Namen spielen während der Laufzeit eine eher untergeordnetere Rolle:

```cpp
struct User {
    std::string m_name;
    std::unique_ptr<AuthInfo> auth_info_;
    short m_level{};
    bool m_isPlaying{};
};
```

```cpp
struct AuthInfo {
    std::string m_username;
    std::string m_password;
    std::string m_securityQuestion;
    std::string m_securityAnswer;
};
```


<img src="Struct_User_Memory_Layout_Short_Variant_02.png" width="600">

*Abbildung* 3: Struktur `User` nach einem Redesign: Übersichtsdarstellung.

<img src="Struct_User_Memory_Layout_Long_Variant_02.png" width="600">

*Abbildung* 4: Struktur `User` nach einem Redesign: Detaildarstellung.

Es kommt nun neben Struktur `User` eine zweite Struktur `AuthInfo` ins Spiel:

<img src="Struct_Auth_Memory_Layout_Short_Variant_02.png" width="600">

*Abbildung* 5: Struktur `AuthInfo`: Übersichtsdarstellung.

<img src="Struct_Auth_Memory_Layout_Long_Variant_02.png" width="600">

*Abbildung* 6: Struktur `AuthInfo`: Detaildarstellung.

Die dritte Variante beherbergt den radikalsten Ansatz.
Es wird auf die Objektorientierung verzichtet!
Stattdessen kommen mehrere Arrays zum Einsatz.
Datentechnisch könnten dies `std::array`- oder `std::vector`-Objekte sein,
in jedem Fall sollten sie ihre Daten im Speicher konsekutiv abgelegt haben:

<img src="cpp_stl_parallel_arrays.svg" width="600">

*Abbildung* 7: Speicherlayout bei Verwendung von drei parallelen Arrays.

Vergleichen Sie die Ausführungszeiten dieser drei Varianten!

---

[Zurück](Readme_Data_Structures_and_Algorithms.md)

---
