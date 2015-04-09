# Macchiato

C++ test framework for desktop and Arduino.

Macchiato strives to be as close and familiar as [Mocha](http://mochajs.org/)/[Chai](http://chaijs.com/).

![](http://i.imgur.com/fhGebYm.png)

## Latest Version: 0.6.1

# How to use/include

Just add the `include` folder to your include path.

Macchiato is also available as a header-only library in `single_include/Macchiato.hpp`. This is a no-dependency/fully-independent version of Macchiato. We simply concatenate the dependencies together and remove the `#include *.h` to the h file dependencies

```
#include <iostream>
#include "Macchiato.h"
// Optionally use the namespace to do `describe` instead of `Macchiato::describe`, etc
using namespace Macchiato;

describe("Foo", [&]() {
	describe("with bar", [&]() {
		it("should baz", [&]() {
			return expect(true).to->equal(true)->getResult();
		});

		// This test will fail because it is false != true
		it("should qux", [&]() {
			return expect(false).to->equal(true)->getResult();
		});

		// An `it` call without a callback is considered "pending"
		// In other words, the test still needs to written/implemented.
		it("should norf");
	});
});

int main() {
	std::cout << Macchiato::GetResultantTestOutput() << std::endl;

	return 0;
}
```

## Built in runner

Macchiato has a main function that you can use to remove some boilerplate from your testing code. The code below works on desktop and Arduino.

On the Arduino platform, it spits out the test results over Serial every 1 second.

On the desktop platform, you can pass in command-line arguments/flags.

```
// This tells Macchiato to provide a main function. Only do this in one source/cpp file
#define MACCHIATO_MAIN
#include "Macchiato.h"
using namespace Macchiato;

MACCHIATO_RUNTESTS([] {
	describe("Foo", [&]() {
		describe("with bar", [&]() {
			it("should baz", [&]() {
				return expect(true).to->equal(true)->getResult();
			});

			// This test will fail because it is false != true
			it("should qux", [&]() {
				return expect(false).to->equal(true)->getResult();
			});

			// An `it` call without a callback is considered "pending"
			// In other words, the test still needs to written/implemented.
			it("should norf");
		});
	});
});
```



## Details:

Requires C++11. Works on the following platforms:

 - Most desktop environments
 - Visual Studio
 - Arduino

Macchiato uses a `PlatformString` type that is made to be compatible on many platforms(Arduino's `String` and normal desktop `std::string`). There should be no difference in dealing with `PlatformString`'s because it has implicit cast/conversions and operators to all the normal types. If you need some more compatibility, make an issue/pull request.



# API

 - `describe(PlatformString description, function<void> callback)`
 	 - `describe` can be nested as many times as you would like. Each describe will indent the output inside.
 - `it(PlatformString testDescription, function<Macchiato::TestResult> callback)`
 	 - Call `it` inside of `describe` blocks
 - `expect(Ta actual)`: (BDD)

 - `PlatformString Macchiato::GetResultantTestOutput()`: Returns the test output including a summary
 - `void Macchiato::ClearTestResults();`: Clears the test output and resets the test counts. Useful for multiple or subsequent calls that need to be separated.

```
#include <iostream>
#include "Macchiato.h"
using namespace Macchiato;

describe("Box", [&]() {
	it("should have 6 faces", [&]() {
		return expect(MyBox().getNumFaces())).to->equal(6)->getResult();
	});
});

std::cout << Macchiato::GetResultantTestOutput() << std::endl;
```


## BDD

Behaviour driven development (BDD)

 - `expect`
 	 - `expect(actual).then->then->then->equal(expected)->getResults();`

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
 	 - `expect(3).to->never->equal->(3)->getResults();`

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
 	 - `satisfy(Macchiato::MacchiatoPlugin plugin, Te expected)`
 	 - `satisfy(bool testResultBool, PlatformString failMessage)`

## Plugins

Plugin-like functionality is supported via `Macchiato::MacchiatoPlugin` which can be used in `expect().satisify(Macchiato::MacchiatoPlugin plugin, Te expected)`.

````
MacchiatoPlugin<typename Ta, typename Te = Ta>(
	function<bool, Ta, Te> testFunc,
	function<PlatformString, Ta, Te, testFlags> failMessageFunc
);
```

### Example

```
#include "Macchiato.h"
using namespace Macchiato;

// For `fmod` (used in the example plugin)
#include <math.h>

auto doesDivideEvenlyIntoPlugin = MacchiatoPlugin<double>(
	[&](double actual, double expected) {
		return fmod(expected, actual) == 0;
	},
	[&](double actual, double expected, testFlags flags) {
		return PlatformString("Expected ") + PlatformString(actual) + " to " + (flags.negate ? "not " : "") + "divide evenly into " + PlatformString(expected) + " but got a remainder of " + PlatformString(fmod(expected, actual));
	}
);

describe("Some numbers", [&]() {
	it("should divide evenly into other numbers", [&]() {
		return expect(2.0).to->satisfy(doesDivideEvenlyIntoPlugin, 10.0)->getResult();
	});
});
```




# Options:

  - `Macchiato::MacchiatoSettings`
 	 - `useAnsiColor`: bool - whether to add ANSI escape codes for colored text. (Supported in many consoles)
 	 	 - Default: `true`
 	 	 - If you are wanting to have colored Text in Windows (CMD prompt), follow this guide [*Using libuv with Windows and Visual Studio: Getting Started* by Eric Eastwood](http://ericeastwood.com/blog/24/using-libuv-with-windows-and-visual-studio-getting-started)
 	 - `indentToken`: PlatformString - The indentation string/token every time we go a level deeper
 	 	 - Default: `"\t"`


# CLI flags

If you are using the Macchiato main runner these commands will be parsed automattically.

 - `-no-color`: Remove the ANSI color escape codes from the resultant output (plain text)

If you are using your own `int main()` (default) and want to have Macchiato parse the commands, just pass in `argc` and `argv` to `Macchiato::MacchiatoParseCLIArgs(argc, argv)`.

```
int main (int argc, char * const argv[]) {
	// Parse the incoming arguments
	Macchiato::MacchiatoParseCLIArgs(argc, argv);
	
	// Do some tests... describe(...) { it(...) {}; };
	
	// Output the test results
	std::cout << Macchiato::GetResultantTestOutput() << std::endl;
};
```



# Differences from Mocha.js

We strive to make the API syntax the same between Macchiato and Mocha. Some sacrifices had to be made for C++.

We currently do not support the full Mocha.js API. Missing TDD and some BDD `expect` members.

 - Some member names used in Mocha are reserved keywords in C++ which needed to be renamed
 	 - `then`(Macchiato C++) is equivalent to `and`(Mocha JS)
 	 - `never`(Macchiato C++) is equivalent to `not`(Mocha JS)
 - Inline function declaration syntax difference
 	 - Macchiato C++: `[&](){ /*...*/ }`
 	 - Mocha JS: `function() { /*...*/ }`
 - Test chaining syntax is different
 	 - Macchiato C++: `expect(3).to->equal(3)->getResults();`
 	 - Mocha JS: `expect(3).to.equal(3);`