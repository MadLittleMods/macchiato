# Macchiato

[Mocha](http://mochajs.org/)/[Chai](http://chaijs.com/) inspired C++ test framework for desktop and Arduino.

# Latest Version: 0.5.1

# How to use/include

Just add the `macchiato` folder to your include path.

```
#include "Macchiato.h"
// Optionally use the namespace to do `describe` instead of `Macchiato::describe`, etc
using namespace Macchiato;
```

Macchiato is also available as a header-only library in `macchiato-header-only/MacchiatoHeaderOnly.h`. This is a no dependency/full independent version of Macchiato. We simply concatenate the files together and remove the `#include *.h` to the h file dependencies


## Details:

Requires C++11. Works on the following platforms:

 - Most desktop environments
 - Visual Studio
 - Arduino

Macchiato uses a `PlatformString` type that is made to be compatible on many platforms(Arduino's `String` and normal desktop `std::string`). There should be no difference in dealing with `PlatformString`'s because it has implicit cast/conversions and operators to all the normal types. If you need some more compatibility, make an issue/pull request.



# API

 - `describe(PlatformString description, function<void> callback)`
 - `it(PlatformString testDescription, function<void> callback)`
 - `expect<Ta, Te=Ta>(Ta actual)`: (BDD)

```
#include <iostream>
#include "Macchiato.h"
using namespace Macchiato;

describe("Box", [&]() {
	it("should have 6 faces", [&]() {
		return expect<Box>(MyBox().getNumFaces())).to->equal(6)->getResult();
	});
});

std::cout << Macchiato::getResultantTestOutput() << std::endl;
```


## BDD

Behaviour driven development (BDD)

 - `expect`
 	 - `expect<Ta, Te=Ta>(actual).then->then->then->equal(expected)->getResults();`

## Language Chains

Provided as chainable getters to improve the readability of your assertions. They do not provide testing capabilities.

 - `to`
 - `be`
 - `been`
 - `is`
 - `that`
 - `which`
 - `then`:
 	 - *substitute for `and` because `and` is a reserved keyword in C++*
 - `has`
 - `have`
 - `with`
 - `at`
 - `of`
 - `same`

## Chainable test members

These provide actual functionality in the chain.

 - `never`: Negates any of assertions following in the chain.
 	 - *substitute for `not` because `not` is a reserved keyword in C++*
 	 - `expect<int>(3).to->never->equal->(3)->getResults();`

 - `equal(Te value)`: Asserts that the target is equal (==) to `value`.
 	 - Aliases: `eql(...)`
 - `closeTo(double expected, double tolerance)`: Asserts that the target is equal to `expected`, to within a +/- `tolerance` range.
 - `within(double lower, double upper)`: Asserts that the target is within a range.
 - `above(double value)`: Asserts that the target is greater than `value`.
 	 - Aliases: `gt(...)`, `greaterThan(...)`
 - `least(double value)`: Asserts that the target is greater than or equal to `value`.
 	 - Aliases: `gte(...)`
 - `below(double value)`: Asserts that the target is less than `value`.
 	 - Aliases: `lt(...)`, `lessThan(...)`
 - `most(double value)`: Asserts that the target is less than or equal to `value`.
 	 - Aliases: `lte(...)`
 - `satisfy`: Asserts that the target passes a given truth test.
 	 - `satisfy(function<bool, Ta> testFunc)`
 	 - `satisfy(function<bool, Ta> testFunc, function<PlatformString, Ta, expectFlags> failMessageFunc)`
 	 - `satisfy(bool testResultBool, PlatformString failMessage)`



# Options:

  - `Macchiato::MacchiatoSettings`
 	 - `useAnsiColor`: bool - whether to add ANSI escape codes for colored text. (Supported in many consoles)




# Differences from Mocha.js

We strive to make the API syntax the same between Macchiato and Mocha. Some sacrifices had to be made for C++.

 - Some member names used in Mocha are reserved keywords in C++ which needed to be renamed
 	 - `then`(Macchiato C++) is equivalent to `and`(Mocha JS)
 	 - `never`(Macchiato C++) is equivalent to `not`(Mocha JS)
 - Inline function declaration syntax difference
 	 - Macchiato C++: `[&](){ /*...*/ }`
 	 - Mocha JS: `function() { /*...*/ }`
 - Test chaining syntax is different
 	 - Macchiato C++: `expect<int>(3).to->equal(3)->getResults();`
 	 - Mocha JS: `expeect(3).to.equal(3);`