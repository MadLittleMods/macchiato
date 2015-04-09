/* * /
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
/* */


/* */
#include "Macchiato.h"
using namespace Macchiato;


// Keeps track since we last sent a serial in our debug serial
elapsedMicros sinceSerial;


void setup() {
	Serial.begin(9600);

}

int aliveCount = 0;
void loop() {
	

	if (sinceSerial >= 0.5 * 1000000) {
		
		Serial.print("Alive: " + String(aliveCount) + "\n");

		// Necessary because we run multiple times over and over
		// We don't want to see the sum of all, just the results of this run, so we need to reset
		Macchiato::ClearTestResults();

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

		Serial.print(Macchiato::GetResultantTestOutput().value);

		aliveCount++;
		sinceSerial = 0;
	}
}
/* */