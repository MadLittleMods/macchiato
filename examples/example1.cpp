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