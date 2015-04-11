#include <iostream>
#include "Macchiato.h"
// Optionally use the namespace to do `describe` instead of `Macchiato::describe`, etc
using namespace Macchiato;

int main() {
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

	std::cout << Macchiato::GetResultantTestOutput() << std::endl;

	return 0;
}