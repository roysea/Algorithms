/*
*Constants
C++ supports two notions of immutability:

  const: The compiler enforces the promise not to modify a variable,
  even though the variable can only be evaluated at run time.

  constexpr: evaluated at compile time.

  The distinction between const and constexpr is subtle.

  In general, though, const is much more common than constexpr.
  When in doubt, use const, especially to guard against accidentally modifying a variable.

*/
#include <iostream>

int main()
{
    int i;
    std::cout << "Enter an integer value for i: ";
    std::cin >> i;
    const int j = i * 2;  // "j can only be evaluated at run time."
                          // "But I promise not to change it after it is initialized."

    //--------------------compare with constexpr --------------------
    int i;
    std::cout << "Enter an integer value for i: ";
    std::cin >> i;
    constexpr int j = i * 2;  // "j can only be evaluated at run time."
                              // "constexpr must be evaluated at compile time."
                              // "So this code will produce a compilation error."
}

// constexpr int k = 3;  // "k, in contrast, can be evaluated at compile time."
//
// std::cout << "j = " << j << "\n";
// std::cout << "k = " << k << "\n";
