# Benchmarking

[Zurück](../../Readme.md)

---

## Inhalt

  * [Allgemeines](#link1)
  * [Vergleich `const std::string&` versus `std::string_view`](#link2)

---

#### Quellcode

[*CompileTimeProgramming*](CompileTimeProgramming.cpp).<br />

---

## Allgemeines <a name="link1"></a>

---

## Vergleich `const std::string&` versus `std::string_view` <a name="link2"></a>

*Beispiel*:

```cpp
01: // prefix version using std::string
02: static std::string prefix(const std::string& str) {
03:     if (str.length() >= 5) {
04:         // extract a part of string
05:         auto substr = str.substr(1, 4);  // substr is a std::string
06:         return substr;
07:     }
08:     return {};
09: }
10: 
11: // prefix version using std::string_view
12: static std::string_view prefix(std::string_view str) {
13:     if (str.length() >= 5) {
14:         // extract a part of string
15:         auto substr = str.substr(1, 4);  // substr is a std::string_view
16:         return substr;
17:     }
18:     return {};
19: }
20: 
21: // Note: take care of SSO
22: constexpr auto StringArgument1 = "1234567890";
23: constexpr auto StringArgument2 = "The quick brown fox jumps over the lazy dog";
24: constexpr auto StringArgument = StringArgument1;
25: 
26: static void StdStringPrefix(benchmark::State& state) {
27: 
28:     for (auto _ : state) {
29: 
30:         std::string s{ StringArgument };
31:         auto result{ prefix(s) };
32: 
33:         benchmark::DoNotOptimize(result);
34:     }
35: }
36: 
37: BENCHMARK(StdStringPrefix); // register StdStringPrefix as a benchmark
38: 
39: static void StdStringViewPrefix(benchmark::State& state) {
40: 
41:     for (auto _ : state) {
42: 
43:         std::string_view s{ StringArgument };
44:         auto result{ prefix(s) };
45: 
46:         benchmark::DoNotOptimize(result);
47:     }
48: }
49: 
50: BENCHMARK(StdStringViewPrefix); // register StdStringViewPrefix as a benchmark
```

#### Erste Ausführung

Es wird bei dieser Ausführung die Zeichenkette `1234567890` zu Grunde gelegt.
Hier kommt es zur Anwendung der *SSO* (*Small String Optimization*):

<img src="Buch_Andrist_Sehr_High Quick_Cpp_Benchmark_String_Comparison_SSO.png" width="700">

Im zweiten Vergleich wurde eine sehr lange Zeichenkette verwendet.
Diese wird &ndash; bei Verwendung der `std::string`-Klasse &ndash;
auf dem Heap abgelegt. Ein `std::string_view`-Objekt hingegen
benutzt eine `const char*`-Adresse. In diesem Fall entscheiden der Compiler/Linker,
in welchem Datensegment des Prozesses die Zeichenkette abgelegt wird.

Man kann in jedem Fall sagen, das es *nicht* die Halde ist.

<img src="Buch_Andrist_Sehr_High Quick_Cpp_Benchmark_String_Comparison_No_SSO.png" width="700">

---

