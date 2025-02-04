# Guidelines zur Vererbung

[Zurück](Guidelines.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Literatur](#link19)

---

## Allgemeines <a name="link1"></a>

---

## `virtual` oder nicht `virtual` 

  * Einige Klassen dienen nur dem Zweck, mehr oder weniger *Daten* zu halten.
   Derartige Klassen sind dann auch nicht Teil einer Hierarchie.
   Ohne Vererbung gibt es keinen Grund für *virtuelle* Methoden
  * Eine Klasse ist Teil einer Klassenhierarchie. Auch dies ist zunächst kein Grund,
   Methoden als *virtuell* zu definieren. Hier bedarf es eine gewissen &bdquo;Weitsicht&rdquo;:
   Ist man von der Realisierung als auch dem Vorhandensein einer Methode überzeugt,
   dass es keinen Grund geben kann (oder sollte), diese in einer abgeleiteten Klasse
   zu überschreiben &ndash; und damit zu erweitern oder ggf. zu ersetzen &ndash;,
   dann ist `virtual` nicht angesagt.
  * Ist hingegen die Realisierung eher minimalistisch  erfolgt &ndash; aus welchen Gründen auch immer &ndash;,
   und ist diese Methode gewissermaßen als Hinweis für Kindklassen konzipiert, 
   an ihrem *Verhalten* einen Beitrag zu leisten, dann sollte die Methode als `virtual` definiert werden.

## `final` und `override` 

## Haben virtuelle Methoden einen Overhead im Vergleich zu nicht virtuellen Methoden?

In der Umsetzung von virtuellen Methoden auf den Maschinencode weisen Klassen bzw. deren
Objekte mit virtuellen Methoden Nachteile auf:

  * Geschwindigkeit
  * Speicher

Zu den Details:

  * Der Aufruf einer virtuellen Methode erfolgt indirekt.
  Wird eine virtuelle Methode aufgerufen, dann benötigt dieser Aufruf indirekte Zugriffe
  und auch Additionsbefehle: Die indirekten Zugriffe kommen dadurch zustande,
  dass die Adressen virtueller Methoden in Tabellen hinterlegt sind
  (so genannte *virtual function pointer table* oder kurz *vtable*).

  * Viruelle Methoden können &ndash; im Regelfall &ndash; nicht zu `inline`-Code optimiert werden.
   Das so genannte *Inlining* ist eine Optimierungstechnik des Compilers. Diese ist nicht anwendbar,
   wenn der zu optoimierende Code erst zur Laufzeit bekannt ist.

  * Pro Objekt ist mehr Speicher notwendig.
   Jedes Objekt, das mindestens eine virtuelle Methode hat, besitzt eine zusätzliche, &bdquo;versteckte&rdquo; Variable.
   Es handelt sich dabei um eine Zeigervariable (typischerweise unter dem Namen `vtptr` bekannt),
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
19: static void guidelines_inheritance_virtual()
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
00007FF7341C1103  call        GuidelinesInheritance::A::func (07FF734185595h)   

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
21:     // comparison object sizesof classes with / without virtual methods
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



---

[Zurück](../../Readme.md)

---

*Beispiel*:

```cpp
```


*Ausgabe*:

```
```
