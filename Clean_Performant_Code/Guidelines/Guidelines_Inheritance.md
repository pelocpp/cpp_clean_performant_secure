# Guidelines Generische Programmierung

[Zur�ck](Guidelines.md)

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
   Ohne Vererbung gibt es keinen Grund f�r *virtuelle* Methoden
  * Eine Klasse ist Teil einer Klassenhierarchie. Auch dies ist zun�chst kein Grund,
   Methoden als *virtuell* zu definieren. Hier bedarf es eine gewissen &bdquo;Weitsicht&rdquo;:
   Ist man von der Realisierung als auch dem Vorhandensein einer Methode �berzeugt,
   dass es keinen Grund geben kann (oder sollte), diese in einer abgeleiteten Klasse
   zu �berschreiben &ndash; und damit zu erweitern oder ggf. zu ersetzen &ndash;,
   dann ist `virtual` nicht angesagt.
  * Ist hingegen die Realisierung eher minimalistisch  erfolgt &ndash; aus welchen Gr�nden auch immer &ndash;,
   und ist diese Methode gewisserma�en als Hinweis f�r Kindklassen konzipiert, 
   an ihrem *Verhalten* einen Beitrag zu leisten, dann sollte die Methode als `virtual` definiert werden.



## `final` und `override` 

## Haben virtuelle Methoden einen Overhead im Vergleich zu nicht virtuellen Methoden?

In der Umsetzung von virtuellen Methoden auf den Maschinencode weisen Klassen bzw. deren
Objekte mit virtuellen Methoden Nachteile auf:

  * Geschwindigkeit
  * Speicher

Zu den Details:

  * Der Aufruf einer virtuellen Methode erfolgt indirekt.
  Wird eine virtuelle Methode aufgerufen, dann ben�tigt dieser Aufruf indirekte Zugriffe
  und auch Additionsbefehle: Die indirekten Zugriffe kommen dadurch zustande,
  dass die Adressen virtueller Methoden in Tabellen hinterlegt sind
  (so genannte *virtual function pointer table* oder kurz *vtable*).
  WEITER 397

---

[Zur�ck](../../Readme.md)

---
