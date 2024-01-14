# mgsmbCalc

This is mainly an eval library, but I also made a tiny console program to use the library.<br>
I chose to use my own [math library](https://github.com/j-gehrig/math), meaning the algorithms might not be as good as they could.<br>
##### Currently, functions are broken. I'll fix that very soon.<br>
<br>

## Using the library
<br>

```c++
namespace Eval {
  double eval(const char* termCharPtr) { ... }
}
```
<br>

This function inside the namespace `Eval` just takes one argument and returns the result as a non-rounded double.<br>
If you haven't already, include the `eval.h` file next to the other includes of your C++ program.<br>
When compiling you also need to link the eval library to your program. <br>
Since I like to use static libraries, I already provided the compiled eval library in the [lib directory](https://github.com/j-gehrig/mgsmbCalc/tree/main/lib) of the repository.<br>
If you want to compile it yourself, you also have to link my math library, which I also provided, to the eval library.<br>
<br>
<i>Example program using eval:</i>
```c++
#include <iostream>
#include "../includes/eval.h"

main() {
  const char* exampleTerm = "2+2";
  double result = Eval::eval(exampleTerm);
  std::cout << exampleTerm << " = " << result << std::endl;
  return 0;
}
```
<br>
<br>

## Things I have planned for the future:
1. Fix the eval library
2. Improve the console application
3. Learn QT and make a GUI calculator version
4. Use a better math library or improve my own
5. Improve code
6. Avoid using string class
7. Make the library and calculators faster
