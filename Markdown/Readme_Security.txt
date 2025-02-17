// ============================================================

Tipps für Agenda:

https://insights.sei.cmu.edu/blog/secure-coding-in-c11-and-c14/


// ============================================================

Sanitizers:

https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170

Home of the Sanitizers:

https://github.com/google/sanitizers

------------------------

https://learn.microsoft.com/en-us/cpp/code-quality/build-reliable-secure-programs?view=msvc-170

HIER:

Key tools Be aware of and use the following:

Code analysis documentation - C++ and .NET     // <====
/analyze - Visual C++ compiler                 // <====
/W4 and /WX - Visual C++ compiler              // <====
Use the C++ Core Guidelines Checkers           // <====
CodeQL | GitHub
Binskim user guide | GitHub
See also (Windows only): SAL annotations


--------------------------------------------

UBSan

Undefined Behavior Sanitizer, UBSan for short,  is a runtime undefined behaviour checker.

It has the ability to detect integer overflows in the form of compilation options (though it is supposed to check UBs, it also do us the favor to check unsigned overflows):

clang++ -fsanitize=signed-integer-overflow -fsanitize=unsigned-integer-overflow


// ============================================================

Udemy:

Secure Coding in C/C++
Master Secure C/C++ Programming: Uncover Flaws, Learn Defensive Programming Techniques & Design Robust, Secure Software

// =============================================================


Artikel:

https://www.mayhem.security/blog/best-practices-for-secure-programming-in-c

https://medium.com/@AlexanderObregon/secure-coding-practices-in-c-12b756af90fe

// =============================================================

Youtube:

https://www.youtube.com/watch?v=mdBhb1CkOws

// =============================================================

Catching Integer Overflows in C

Overflow // Underflow

https://www.fefe.de/intof.html

https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow

https://www.geeksforgeeks.org/how-do-i-detect-unsigned-integer-overflow-in-cpp/



// =============================================================

Catching Integer Overflows in C

SafeInt !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

https://learn.microsoft.com/en-us/cpp/safeint/safeint-library?view=msvc-170

https://github.com/dcleblanc/SafeInt

https://giodicanio.com/2023/11/13/protecting-your-c-plus-plus-code-against-integer-overflow-made-easy-by-safeint/

https://www.nuonsoft.com/blog/2009/06/09/safeint-in-visual-c-2010/



// =============================================================

Hier muss das Tool Cppcheck  herunter geladen werden ... und danach instlliert werden:

https://cppcheck.sourceforge.io/

Dann muss für Visual Studio ein PluhgIn geladen werden:

https://github.com/VioletGiraffe/cppcheck-vs-addin

https://github.com/VioletGiraffe/cppcheck-vs-addin/releases/tag/1.5



// =============================================================

Type Punning

https://blog.hiebl.cc/posts/practical-type-punning-in-cpp/

// =============================================================

Die ganzen Cast Operatoren beschreiben !!!!!!!!!!!

// =============================================================

Warning Level 4 !!!

Dangling C++ Referenzen .. wie bekommt man die hin ...

// =============================================================
