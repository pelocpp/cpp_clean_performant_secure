# Guidelines Generische Programmierung

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
  WEITER 397

---

[Zurück](../../Readme.md)

---
