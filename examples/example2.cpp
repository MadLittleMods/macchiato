#include <iostream>
#include "Macchiato.h"
// Optionally use the namespace to do `describe` instead of `Macchiato::describe`, etc
using namespace Macchiato;

class Program
{
	public:
		Program()
		{
			foo = 1;
		}
		int foo;
		int bar () { return foo - 1; }
};

int main() {
	describe("Program", [&]() {
		it("should have field 'foo' with initial value 1", [&]() {
			Program ProgramPrototype = Program ();
			return expect(ProgramPrototype.foo).to->equal(1)->getResult();
		});
		it("should have function 'bar' which returns 0 in initial state", [&]() {
			Program ProgramPrototype = Program ();
			return expect(ProgramPrototype.bar()).to->equal(0)->getResult();
		});
		it("should have function 'bar' which returns 9999 when 'foo' is 1000", [&]() {
			Program ProgramPrototype = Program ();
			ProgramPrototype.foo = 1000;
			return expect(ProgramPrototype.bar()).to->equal(999)->getResult();
		});
	});

	std::cout << Macchiato::GetResultantTestOutput() << std::endl;

	return 0;
}
