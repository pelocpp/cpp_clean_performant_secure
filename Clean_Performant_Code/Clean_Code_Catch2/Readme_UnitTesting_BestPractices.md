# Einige Best Practices zum Unit Testing

[Zurück](./Readme_Catch2.md)

---

## Inhalt

  * [Beachte die Namensgebung für Unit Tests](#link1)
  * [Verwende das *AAA*-Muster](#link2)
  * [Vermeide mehrfache Aufrufe von zu testenden Methoden](#link3)
  * [Unit Tests und globale Variablen](#link4)
  * [Unit Tests und private Methoden](#link5)
  * [Vermeide komplexe Logik](#link6)
  * [Vermeide magische Zeichenfolgen](#link7)
  * [Mocks und Stubs](#link8)
  * [Literatur](#link9)

---

#### Quellcode

[*Catch_Unit_Testing_Best_Practices.cpp*](Catch_Unit_Testing_Best_Practices.cpp)<br />

---

## Beachte die Namensgebung für Unit Tests <a name="link1"></a>

Der Name eines Unit Tests sollte aus drei Teilen bestehen:

  * Name der zu testenden Methode / Funktion
  * Szenario, in dem die Methode getestet wird
  * Erwartetes Verhalten beim Aufruf des Szenarios

Benennungsstandards sind wichtig, da sie den Zweck und die Anwendung des Tests verdeutlichen.
Tests stellen nicht nur sicher, dass Ihr Code funktioniert.
Sie dienen auch der Dokumentation.
Allein durch die Betrachtung der Unit-Tests sollten Sie das Verhalten Ihres Codes ableiten können,
ohne den Code selbst betrachten zu müssen.
Darüber hinaus können Sie bei fehlgeschlagenen Tests genau erkennen, welche Szenarien nicht Ihren Erwartungen entsprechen.

## Verwende das *AAA*-Muster <a name="link2"></a>

Der Code von Unit-Testmethoden sollte leicht lesbar und übersichtlich sein.
Daher schreiben wir Unit-Tests mit dem *AAA*-Muster.
Das *AAA*-Muster ist ein gängiges Muster für Unit-Tests.
Es sorgt für eine klare Trennung zwischen Testobjekt-Setups, Aktionen und Ergebnissen.
Es unterteilt die Unit-Testmethoden in drei Teile: *Arrange*, *Act* und *Assert*.

  * In *Arrange* erstellen und setzen wir die notwendigen Objekte für den Test.
  * In *Act* rufen wir die zu testende Methode auf und ermitteln den tatsächlichen Wert.
  * In *Assert* prüfen wir den erwarteten und den tatsächlichen Wert. Assertions bestimmen, ob der Test fehlschlägt oder erfolgreich ist.
    Stimmen Erwartungswert und tatsächlicher Wert überein, ist der Test erfolgreich.

---

## Vermeide mehrfache Aufrufe von zu testenden Methoden <a name="link3"></a>

Wir sollten für jede zu testende Methode eine neue separate Testmethode erstellen.
Dies zeigt uns deutlich, welches Szenario fehlgeschlagen ist.

Ein nicht empfehlenswertes Beispiel:

```cpp
01: struct Calculator
02: {
03:     static int add(int x, int y) { return x + y; }
04:     static int sub(int x, int y) { return x - y; }
05:     static int mul(int x, int y) { return x * y; }
06:     static int div(int x, int y) { return x / y; }
07: };
08: 
09: TEST_CASE("Bad Example for testing several methods", "[AdditionAndSubtraction_IntegerNumbers_ReturnsSumAndDifference]")
10: {
11:     // arrange
12:     auto number1{ 9 };
13:     auto number2{ 5 };
14:     auto expected1{ 14 };
15:     auto expected2{ 4 };
16: 
17:     // act 
18:     auto actual1 = Calculator::add(number1, number2);
19:     auto actual2 = Calculator::sub(number1, number2);
20: 
21:     // assert
22:     CHECK(actual1 == expected1);
23:     CHECK(actual2 == expected2);
24: }
```

Ein empfehlenswertes Beispiel:

```cpp
01: TEST_CASE("Good Example for testing a single method", "[Add_TwoNumbers_ReturnsSum]") 
02: {
03:     // arrange
04:     auto [a, b, expected] = GENERATE(
05:         std::tuple{ 1, 4, 5 },
06:         std::tuple{ 2, 5, 7 },
07:         std::tuple{ 3, 6, 9 }
08:     );
09: 
10:     // act 
11:     auto actual = Calculator::add(a, b);
12: 
13:     // assert
14:     CHECK(expected == actual);
15: }
```

---

## Unit Tests und globale Variablen <a name="link4"></a>

Globale Variablen sind in Unit Tests problematisch,
weil sie Zustand zwischen Tests teilen können &ndash; das macht Tests voneinander abhängig
und schwer reproduzierbar. Es gibt aber Strategien, wie man damit umgehen kann:

### Verwendung eines RAII-Guards

Manchmal möchte man den ursprünglichen Wert einer globale Variablen sichern
und nach dem Test zurücksetzen. Dafür kann man einen RAII Guard schreiben:

```cpp
01: struct GlobalGuard
02: {
03:     int oldValue;
04:     GlobalGuard() : oldValue{ g_counter } {}
05:     ~GlobalGuard() { g_counter = oldValue; }
06: };
```

### Verwendung eines Text Fixtures

Für wiederverwendbares Setup/Teardown kann man eine *Fixture*-Klasse definieren:

  * Konstruktor = *Setup*
  * Destruktor = *Teardown*

Catch2 unterstützt *Fixture*-Klassen mit dem Testmakro `TEST_CASE_METHOD`:


```cpp
01: struct GlobalFixture
02: {
03:     GlobalFixture() { g_counter = 0; }   // setup
04:     ~GlobalFixture() { g_counter = 0; }  // teardown
05: };
06: 
07: TEST_CASE_METHOD(GlobalFixture, "Global Variables with Test Fixture", "[global_variables_test_fixture]") {
08:     g_counter++;
09:     REQUIRE(g_counter == 1);
10: }
```

---

## Unit Tests und private Methoden <a name="link5"></a>

Wie testet man private Methoden einer Klasse?

Kurze Antwort: Überhaupt nicht!

Längere Antwort:<br />

  * Private Methoden sind ... privat. Wir können sie nicht direkt über Unit-Tests aufrufen. Das liegt an den Zugriffsklassen.
    Wie testen wir sie also, wenn wir sie nicht erreichen können?
  * Wir testen öffentliche Methoden, da diese zugänglich sind. Öffentliche Methoden rufen private Methoden auf.
    Sie sollten den gesamten Ablauf der öffentlichen Methoden testen, d. h. auch, ob die über die öffentliche Methode aufgerufenen privaten Methoden ordnungsgemäß funktionieren.

## Vermeide komplexe Logik <a name="link6"></a>

Vermeiden Sie logische Bedingungen wie &bdquo;if&rdquo;, &bdquo;for&rdquo;, &bdquo;while&rdquo; und &bdquo;switch&rdquo;.

Erstellen Sie keine Daten im Testmethodenbereich.
Konzentriere Dich ausschließlich auf das Ergebnis.

Ein nicht empfehlenswertes Beispiel:

```cpp
01: TEST_CASE("Bad Example for testing IsPrime", "[Avoid_Using_Complex_Logic]")
02: {
03:     // arrange
04:     int testCases[] { 2, 3, 5 };
05:     bool expected = true;
06: 
07:     // act and assert
08:     for(auto number : testCases)
09:     {
10:         // act
11:         auto actual = IsPrime(number);
12: 
13:         // assert
14:         CHECK(expected == actual);
15:     }
16: }
```

Ein empfehlenswertes Beispiel:

```cpp
01: TEST_CASE("Good Example for testing IsPrime", "[Avoid_Using_Complex_Logic_Improved]")
02: {
03:     // arrange
04:     int number = GENERATE(2, 3, 5);
05:     bool expected = true;
06: 
07:     // act
08:     auto actual = IsPrime(number);
09: 
10:     // assert
11:     REQUIRE(actual == expected);
12: }
```

Beachte das Catch2 Sprachmittel `GENERATE`.

---

## Vermeide magische Zeichenfolgen (&bdquo;Magic Strings&rdquo;) <a name="link7"></a>

Magische Zeichenfolgen und Zahlen, die oft fest im Code kodiert sind,
stellen bei Unit-Tests eine Herausforderung dar.
Diese Werte sind nicht eindeutig und können bei Tests und Wartung zu Fehlern führen.

Anstatt diese &bdquo;magischen&rdquo; Werte direkt zu verwenden, sollten Sie konstante Werte mit aussagekräftigen Namen verwenden.
Konstanten bieten eine zentrale Informationsquelle für diese Werte und verbessern die Lesbarkeit des Codes.
Die explizite Angabe von Werten verbessert das Codeverständnis.

---

## Mocks und Stubs <a name="link8"></a>

Wir wollen die Begrifflichkeiten Mock und Stub an einem Beispiel erläutern:

*Stub*:<br />
Ein Stub ist ein einfaches Fake-Objekt. Es stellt sicher, dass ein Test reibungslos abläuft.
Stub-Objekte implementieren Methoden mit Anweisungen, die ein bestimmtes Ergebnis zurückgeben.

Beobachtung:
Stubs geben immer dieselbe Antwort zurück.

*Mock*:<br />
Ein Mock-Objekt überprüft, ob ein bestimmtes Verhalten auftritt &ndash; also ob z. B. eine Methode aufgerufen wurde.
Mock-Objekte setzen Erwartungen, d.h. sie treffen Aussagen (*Assertions*) darüber, ob oder wie sie aufgerufen wurden.

*Beispiel*:

Wir haben eine `UserService`-Klasse, die einen `UserRepository` benutzt:

```cpp
01: class User
02: {
03: private:
04:     std::string m_name;
05: 
06: public:
07:     explicit User(const std::string& name) : m_name{ name } {}
08: 
09:     const std::string& getName() const { return m_name; }
10: };
11: 
12: class UserRepository
13: {
14: public:
15:     virtual ~UserRepository() = default;
16:     virtual std::shared_ptr<User> findById(int id) = 0;
17:     virtual void save(const User& user) = 0;
18: };
19: 
20: class UserService
21: {
22: private:
23:     UserRepository& m_repo;
24: 
25: public:
26:     explicit UserService(UserRepository& repo) : m_repo(repo) {}
27: 
28:     std::string getUserName(int id)
29:     {
30:         auto user = m_repo.findById(id);
31:         return (user != nullptr) ? user->getName() : "";
32:     }
33: 
34:     void registerUser(const std::string& name) {
35:         m_repo.save(User{ name });
36:     }
37: };
```

### Stub in C++

Ein **Stub** liefert immer vordefinierte Daten zurück:

```cpp
01: class UserRepositoryStub : public UserRepository
02: {
03: public:
04:     std::shared_ptr<User> findById(int id) override {
05:         return std::make_shared<User>("Alice");  // there is always "Alice" returned
06:     }
07: 
08:     void save(const User& user) override {
09:         // leave blank, not relevant for this test
10:     }
11: };
12: 
13: TEST_CASE("getUserName", "[getUserName]")
14: {
15:     // arrange
16:     UserRepositoryStub stub;
17:     UserService service{ stub };
18: 
19:     // act
20:     auto name{ service.getUserName(1) };
21: 
22:     // assert
23:     CHECK(name == "Alice");
24: }
```

Beobachtung:<br />
Der **Stub** gibt immer dieselbe Antwort zurück.


### Mock in C++

Ein **Mock** prüft, ob eine Methode aufgerufen wurde.
Wir zählen also Aufrufe und merken uns die Parameter:

```cpp
01: class UserRepositoryMock : public UserRepository
02: {
03: private:
04:     int m_saveCalled;
05:     std::optional<std::string> m_savedName;
06: 
07: public:
08:     UserRepositoryMock () : m_saveCalled{} {}
09: 
10:     std::shared_ptr<User> findById(int) override {
11:         return nullptr;  // no implementation, not needed for this test
12:     }
13: 
14:     void save(const User& user) override {
15:         m_saveCalled++;
16:         m_savedName = user.getName();
17:     }
18: 
19:     int getSaveCalled() const { return m_saveCalled; }
20:     std::optional<std::string> getSavedName() const { return m_savedName; }
21: };
22: 
23: TEST_CASE("UserService calls save on UserRepository", "[registerUser]")
24: {
25:     // arrange
26:     UserRepositoryMock mock;
27:     UserService service{ mock };
28: 
29:     // act
30:     service.registerUser("Bob");
31: 
32:     // assert
33:     REQUIRE(mock.getSaveCalled() == 1);
34:     REQUIRE(mock.getSavedName() == "Bob");
35: }
```

Beobachtung:<br />
Der **Mock** dient hier zur Verhaltensprüfung.

---

## Literatur <a name="link9"></a>

Die Beispiele und Anregungen dieses Abschnitts wurden sehr stark inspiriert von folgenden Aufsätzen:

[&bdquo;Unit Testing Best Practices&rdquo;](https://medium.com/@kaanfurkanc/unit-testing-best-practices-3a8b0ddd88b5)
von *Kaan Furkan Çakıroğlu*.

[&bdquo;Unit Testing Best Practices for .NET&rdquo;](https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-best-practices)
von Microsoft.

[&bdquo;What are Unit Testing Best Practices?&rdquo;](https://www.ibm.com/think/insights/unit-testing-best-practices)
von IBM.

---

[Zurück](./Readme_Catch2.md)

---
