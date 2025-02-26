# Guidelines zur Vererbung

[Zur�ck](./Readme_Guidelines.md)

---

## Inhalt

  * [Wann nutzen Sie eine Hierarchie von Klassen?](#link1)
  * [`virtual` oder nicht `virtual`](#link2)
  * [Haben virtuelle Methoden einen Overhead im Vergleich zu nicht virtuellen Methoden?](#link3)
  * [Abstrakte Klassen und Schnittstellen](#link4)
  * [Konstruktoren und virtuelle Methoden](#link5) 
  * [Destruktoren und virtuelle Methoden](#link6)
  * [Einmal `virtual` &ndash; immer `virtual`](#link7)
  * [Destruktoren und `virtual`: Virtueller Basisklassendestruktor](#link8)
  * [Schl�sselwort `override`](#link9)
  * [Schl�sselwort `final`](#link10)
  * [Schl�sselwort `override`](#link11)
  * [Schl�sselw�rter `virtual`, `override` und `final`](#link12)
  * [Sch�tzen Sie sich vor *Object Slicing*](#link13)
  * [Literatur](#link14)

---

#### Wann nutzen Sie eine Hierarchie von Klassen? <a name="link1"></a>

Modelliert man etwas in Quellcode, das eine *inh�rent hierarchische Struktur* besitzt,
sollte man zu einer *Hierarchie von Klassen* greifen.

Diese Regel ist eigentlich einleuchtend,
auch wenn sie im *Wording* etwas schrill ist.

Gelingt es, eine Analogie zwischen der realen Welt und Quellcode herzustellen,
ist man h�ufig auf der sicheren Seite.

Stellen wir uns vor, wir m�ssen ein komplexes System modellieren,
das aus einer Reihe von Teilsystemen besteht.
Im meinem Fall handelte es sich um die Werkzeuge einer Werkzeugmaschinensteuerung.

Ein exemplarisches Werkzeuge stellt die Schnittstelle zum Benutzer dar. 

Damit leiten sich sofort Anforderungen ab,
verschiedene Benutzerschnittstellen wie Tastatur, Mauszeiger, Touchscreen
oder einfach auch nur Buttons zu unterst�tzen. 

Solch ein System von Teilsystemen besitzt eine inh�rent hierarchische Struktur.
Eine Modellierung bildete daher die physikalische Struktur ab und
ist damit relativ leicht im Top-down-Ansatz zu erfassen.

*Beispiel*:

```cpp
01: class DrawableUIElement {
02: public:
03:     virtual void render() const = 0;
04:     // ...
05: };
06: 
07: class AbstractButton : public DrawableUIElement {
08: public:
09:     virtual void onClick() = 0;
10:     // ...
11: };
12: 
13: class PushButton : public AbstractButton {
14:     virtual void render() const override {};
15:     virtual void onClick() override {};
16:     // ...
17: };
18: 
19: class Checkbox : public AbstractButton {
20:     // ...
21: };
```

#### Wann nutzen Sie eine Hierarchie von Klassen? <a name="link2"></a>

WEITER:

https://www.heise.de/blog/C-Core-Guidelines-Klassenhierarchien-3852049.html


---

#### `virtual` oder nicht `virtual`  <a name="link3"></a>

  * Einige Klassen dienen nur dem Zweck, mehr oder weniger *Daten* zu halten.
   Derartige Klassen sind dann auch nicht Teil einer Hierarchie.
   Es gibt hier keinen Grund f�r *virtuelle* Methoden.
  * Eine Klasse ist Teil einer Hierarchie von Klassen. Auch ist dies zun�chst kein Grund,
   Methoden als *virtuell* zu definieren. Hier bedarf es einer gewissen &bdquo;Weitsicht&rdquo;:
   Ist man von dem Vorhandensein als auch der Intention einer Methode in einer Weise �berzeugt,
   dass es keinen Grund geben kann (oder sollte), diese in einer abgeleiteten Klasse
   zu �berschreiben &ndash; und damit zu erweitern oder ggf. zu ersetzen &ndash;,
   dann ist `virtual` nicht angesagt.
  * Gibt es hingegen in einer Vaterklasse eine Methode &ndash; ggf. mit einer Realisierung, aus welchen Gr�nden auch immer &ndash;,
   und ist diese Methode gewisserma�en als Hinweis f�r Kindklassen konzipiert, 
   an ihrem *Verhalten* einen Beitrag zu leisten, dann sollte die Methode als `virtual` definiert werden.


#### Haben virtuelle Methoden einen Overhead im Vergleich zu nicht virtuellen Methoden? <a name="link4"></a>

Einfache Frage, einfache Antwort: Ja. In der Umsetzung von virtuellen Methoden auf den Maschinencode weisen Klassen bzw. deren
Objekte mit virtuellen Methoden Nachteile in punkto

  * Geschwindigkeit und
  * Speicher

auf. Zu den Details:

  * Der Aufruf einer virtuellen Methode erfolgt *indirekt*:<br />
  Wird eine virtuelle Methode aufgerufen, dann ben�tigt dieser Aufruf indirekte Zugriffe
  und auch Additionsbefehle: Die indirekten Zugriffe kommen dadurch zustande,
  dass die Adressen virtueller Methoden in Tabellen hinterlegt sind
  (so genannte *virtual function pointer table* oder kurz *vtable* genannt).
  Die Additionen kommen durch Offsets zustande, mit denen innerhalb in dieser Tabelle zugegriffen wird.

  * Virtuelle Methoden k�nnen &ndash; im Regelfall &ndash; nicht zu `inline`-Code optimiert werden:<br />
   Das so genannte *Inlining* ist eine Optimierungstechnik des Compilers. Diese ist nicht anwendbar,
   wenn der zu optimierende Code erst zur Laufzeit bekannt ist.

  * Pro Objekt ist mehr Speicher notwendig:<br />
   Jedes Objekt, das mindestens eine virtuelle Methode hat, besitzt eine zus�tzliche, &bdquo;versteckte&rdquo; Variable.
   Es handelt sich dabei um eine Zeigervariable (typischerweise unter dem Namen `vptr` bekannt),
   der in eine statische Klassentabelle (`vtable`) zeigt. 
   
Wir betrachten diese Aussagen an zwei Beispielen:

*Beispiel*:

```cpp
01: class A
02: {
03: public:
04:     void func() {};
05: };
06: 
07: class Base
08: {
09: public:
10:     virtual void func() {};
11: };
12: 
13: class Derived : public Base
14: {
15: public:
16:     virtual void func() override {};
17: };
18: 
19: void test()
20: {
21:     // comparison virtual / non-virtual method invocation
22:     A a;
23:     a.func();
24: 
25:     Base* bp;
26:     bp = new Derived();
27:     bp->func();
28: }
```

*Ausgabe*:

```
        // comparison virtual / non-virtual method invocation
        A a;
        a.func();
00007FF7341C10FF  lea         rcx,[a]
00007FF7341C1103  call        A::func (07FF734185595h)

        Base* bp;
        bp = new Derived();
        bp->func();
00007FF7341C115F  mov         rax,qword ptr [bp]
00007FF7341C1163  mov         rax,qword ptr [rax]
00007FF7341C1166  mov         rcx,qword ptr [bp]
00007FF7341C116A  call        qword ptr [rax]
```

*Beispiel*:

```cpp
01: class X
02: {
03: private:
04:     double m_value{};
05: 
06: public:
07:     void func() {};
08: };
09: 
10: class Y
11: {
12: private:
13:     double m_value{};
14: 
15: public:
16:     virtual void func() {};
17: };
18: 
19: void test()
20: {
21:     // comparison object sizes of classes with / without virtual methods
22:     X x;
23:     Y y;
24: 
25:     std::println("Sizeof x: {}", sizeof(x));
26:     std::println("Sizeof y: {}", sizeof(y));
27: }
```


*Ausgabe*:

```
Sizeof x: 8
Sizeof y: 16
```

#### Abstrakte Klassen und Schnittstellen <a name="link5"></a>

Wenn Ihr Design auf abstrakte Klassen und Schnittstellen baut, dann ist `virtual` nat�rlich angesagt.

*Beispiel*:

```cpp
01: class IObserver {
02: public:
03:     virtual ~IObserver() {};
04:     virtual void update(const std::string& message) = 0;
05: };
06: 
07: class ISubject {
08: public:
09:     virtual ~ISubject() {}
10:     virtual void attach(IObserver* observer) = 0;
11:     virtual void detach(IObserver* observer) = 0;
12: };
```

*Beispiel*:

```cpp
01: class DecoratorBase : public Component
02: {
03: protected:
04:    std::shared_ptr<Component> m_component;
05: 
06: public:
07:     DecoratorBase(const std::shared_ptr<Component>& component) 
08:         : m_component{ component }
09:     {}
10: 
11:     // decorator delegates all work to the wrapped component
12:     virtual std::string operation() const override {
13:         return m_component->operation();
14:     }
15: };
```

#### Konstruktoren und virtuelle Methoden <a name="link6"></a>

Konstruktoren als solche sind niemals virtuell. 
Innerhalb von Konstruktoren darf man keine virtuellen Methoden aufrufen.

Der Aufruf virtueller Funktionen im Konstruktor ist verboten,
da der Zugriff auf noch nicht initialisierte Variablen der abgeleiteten Klasse in �berschriebenen Versionen
virtueller Methoden erfolgen k�nnte, die in der abgeleiteten Klasse implementiert sind.

*Hinweis*:
Betrachten Sie hierzu das Design Pattern &bdquo;*Virtueller Konstruktor*&rdquo;.


#### Destruktoren und virtuelle Methoden <a name="link7"></a>

Man kann wie bei den Konstruktoren �hnliche �berlegungen anstellen:
Auch in Destruktoren sollte man auf den Aufruf von virtuellen Methoden verzichten.

#### Einmal `virtual` &ndash; immer `virtual` <a name="link8"></a>

Eine virtuelle Methode einer Basisklasse, die in einer abgeleiteten Klasse �berschrieben wird,
ist ebenfalls virtuell. 

Man muss das Schl�sselwort folglich nicht mehr explizit hinschreiben: Einmal `virtual` &ndash; immer `virtual`.

#### Destruktoren und `virtual`: Virtueller Basisklassendestruktor <a name="link9"></a>

Ein Destruktor einer Klasse ist als `virtual` kennzuzeichnen,
wenn es in der Klasse mindestens eine virtuelle Methode gibt.


*Beispiel*:

```cpp
01: class Base
02: {
03: public:
04:     ~Base() {
05:         std::println("d'tor Base");
06:     }
07: 
08:     virtual void doSomething() {}
09: };
10: 
11: class Derived : public Base
12: {
13: public:
14:     ~Derived() {
15:         std::println("d'tor Derived");
16:     }
17: };
18: 
19: void test() {
20:     Base* bp = new Derived();
21:     delete bp;
22: }
```

*Frage*:<br />
Welcher der beiden Konstruktoren wird in dem Code-Snippet aufgerufen?

*Ausgabe*:

```
d'tor Base
```

*Frage*:<br />
Wie kann man erreichen, dass auch zus�tzlich der Destruktor der abgeleiteten Klasse aufgerufen wird?

*Antwort*:

```cpp
virtual ~Base()
```


*Ausgabe*:

```
d'tor Derived
d'tor Base
```


#### Schl�sselwort `override` <a name="link10"></a>

Deklariert man eine Methode mit dem Schl�sselwort `override`, dr�ckt man die Absicht aus,
sie zu �berschreiben. 

Daher ist es sinnvoll, `override` an der Stelle anzugeben,
an der man diese Absicht hat.

Dies kann und sollte man dann den �bersetzter auch �berpr�fen lassen,
n�mlich die Absicht, diese Methode zu �berschreiben.

Frei nach dem Motto

> **The more things you check at compile time the fewer runtime bugs you have.**

Eine derartige Deklaration reicht dann auch aus, um zu �berpr�fen,
ob die Methode urspr�nglich (in einer der Basisklassen) als `virtual` deklariert wurde
und ob man in der abgeleiteten Klasse die Signatur richtig verwendet hat.

*Beispiel*:

```cpp
01: class Base
02: {
03: public:
04:     virtual void func() { std::println("In base class"); }
05: };
06: 
07: class Derived : public Base
08: {
09: public:
10:     void func(int a)
11:     {
12:         std::println("In derived class");
13:     }
14: };
15: 
16: void test()
17: {
18:     Base base;
19:     Derived derived;
20:     std::println("Compiles successfully");
21: 
22:     Base* bp;
23:     bp = &derived;
24:     bp->func();
25: }
```

*Frage*:<br />
Welche Ausgabe erwarten Sie in der Konsole?

*Ausgabe*:

```
Compiles successfully
In base class
```

#### Schl�sselwort `final` <a name="link11"></a>

Das C++ Schl�sselwort `final` wurde ab C++ 11 eingef�hrt,
um die weitere Vererbung einer Klasse oder das �berschreiben einer virtuellen Funktion zu verhindern.

Es dient zwei Hauptzwecken:

  * Verhindern der Klassenvererbung:<br />
   Wenn eine Klasse als `final` markiert wird, wird verhindert, dass sie als Basisklasse verwendet werden kann.

  * Verhindern des �berschreibens virtueller Methoden:<br />
    Wenn `final`  auf eine virtuelle Methode  angewendet wird,
    wird sichergestelt, dass keine abgeleitete Klasse diese Methode �berschreiben kann.

Dieses Sprachfeature tr�gt zu Verbesserungen der *Programmsicherheit*,
der *Wartbarkeit* und von *potentiellen Optimierungen* bei.

In einigen F�llen erm�glicht es dem Compiler, optimierteren Code zu generieren.
Diese Technik ist bekannt als *Devirtualisierung*.

Wir betrachten einige Beispiele dazu:

*Beispiel*:

```cpp
01: struct Base {
02:     virtual void func() {};
03: };
04: 
05: struct Derived : public Base {
06:     void func() final {};                   // virtual as it overrides base::f
07: };
08: 
09: struct MostDerived : public Derived {
10:     void func() {};
11: };
```

*Frage*: Ist dieses Code-Snippet �bersetzungsf�hig?
 
*Beispiel*:

```cpp
01: struct Base final {
02:     virtual void func() {};
03: };
04: 
05: struct Derived : public Base {
06: };
```

*Frage*: Ist dieses Code-Snippet �bersetzungsf�hig?

*Beispiel*:

```cpp
01: class IAbstract
02: {
03: public:
04:     virtual void doSomething() = 0;
05: };
06: 
07: class CDerived : public IAbstract
08: {
09: private:
10:     int m_x{};
11: 
12: public:
13:     void doSomething() final { m_x = 1; }
14: 
15:     void anotherMethod (void) { doSomething(); }
16: };
```

*Frage*:<br />
Auf welche Weise kann der Compiler die Methode `anotherMethod` optimieren?

*Antwort*:<br />
Unter Verwendung des `final`-Schl�sselworts in Zeile 13
kann der Compiler `CDerived::doSomething()` **direkt** aus `anotherMethod()` heraus
oder sogar inline aufrufen.

Ohne `final` muss ein indirekter Aufruf innerhalb von `anotherMethod()` generiert werden,
da `anotherMethod()` innerhalb einer abgeleiteten Klasse aufgerufen werden k�nnte,
die `doSomething()` �berschrieben hat.

*Abschlie�ende Frage*:<br />
Welchen Sinn ergibt es, virtuelle Methode in einer als `final` deklarierten Methode aufzurufen?

*Antwort*:<br />
Keinen!
<br />Wenn Sie eine Methode als `final` deklarieren, dann bringen Sie zum Ausdruck,
dass sich das Verhalten des Objekts nicht mehr �ndert! Aber halten Sie sich auch daran?
Wir stellen uns den Aufruf einer virtuellen Methode in einer als `final` deklarierten Methode vor.
Kann man dann noch sagen, dass die Methode dann noch das tut, was wir mit ihrer Realisierung verbinden.
Wir haben doch mit `final` zum Ausdruck bringen wollen, das wir ein Versprechen
&bdquo;*diese Methode tut das, was ich sage*&rdquo;
geben wollten.

*Fazit*:<br />
Als `final` deklarierte Methoden sollten keine virtuellen Methoden aufrufen!

#### Schl�sselw�rter `virtual`, `override` und `final` <a name="link12"></a>

Es gibt mehrere Optionen, eine virtuelle Methoden mit den oben genannten Schl�sselw�rter zu markieren.

Verwenden Sie entweder

   *  nur `virtual`
   * oder nur `override`
   * oder nur `final`
   
`override` und `final` implizieren `virtual`, eine doppelte Benennung ist folglich nicht n�tig
`virtual` muss an der urspr�nglichen Deklaration vorhanden sein
Ein fehlendes `virtual` oder `override` an einer virtuellen Methode verwirrt


#### Sch�tzen Sie sich vor *Object Slicing* <a name="link13"></a>


https://stackoverflow.com/questions/274626/what-is-object-slicing

---

[Zur�ck](./Readme_Guidelines.md)

---
