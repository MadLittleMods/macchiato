// Macchiato
// v0.5.1
// https://github.com/MadLittleMods/macchiato
//
// Mocha.js inspired testing for C++
//
// Requires C++11. Works on the following platforms:
//		- Most desktop environments
//		- Visual Studio
//		- Arduino


#ifndef Macchiato_h
#define Macchiato_h

// fabs
#include <cmath>

#include "functionSig.h"
#include "PlatformString.h"


namespace Macchiato {

	struct TestResult {
		PlatformString message;
		bool didPass = true;
	};


	struct _MacchiatoSettings {
		bool useAnsiColor = true;
	};
	_MacchiatoSettings MacchiatoSettings;

	struct __MacchiatoUtilClass {

		void incrementDepth() {
			this->currentDepth += 1;
		};
		void decrementDepth() {
			this->currentDepth -= 1;
		};


		PlatformString generateCurrentDepthString() {
			PlatformString depthString = "";
			for(unsigned int i = 0; i < currentDepth-1; i++) {
				depthString += "\t";
			}

			return depthString;
		};
		PlatformString generateCurrentChildDepthString() {
			PlatformString childDepthString = "";
			for(unsigned int i = 0; i < currentDepth; i++) {
				childDepthString += "\t";
			}

			return childDepthString;
		};

		PlatformString wrapInAnsiCyan(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[36;1m" + input.value + "\033[0m") : input;
		};

		PlatformString wrapInAnsiGreen(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[32;1m" + input.value + "\033[0m") : input;
		};
		PlatformString wrapInAnsiRed(PlatformString input) {
			return MacchiatoSettings.useAnsiColor ? PlatformString("\033[31;1m" + input.value + "\033[0m") : input;
		};


		// Just a helper to add to the resultant output
		void log(PlatformString message) {
			resultantOutputString += message;
		};


		// Helper to increment the summary of tests
		enum TestResultType {
			Pass, Fail, Pending
		};
		void logTestResult(TestResultType tType) {
			if(tType == TestResultType::Pass) {
				this->numTestsPassed++;
			}
			else if(tType == TestResultType::Fail) {
				this->numTestsFailed++;
			}
			else if(tType == TestResultType::Pending) {
				this->numTestsPending++;
			}
		}




		PlatformString getResultantOutput() {
			PlatformString result = PlatformString(this->resultantOutputString);
			// Add a small summary at the end
			result += PlatformString("\n\n") +
				this->wrapInAnsiGreen(PlatformString(this->numTestsPassed) + " passing") + "\n" +
				this->wrapInAnsiRed(PlatformString(this->numTestsFailed) + " failing") + "\n" +
				this->wrapInAnsiCyan(PlatformString(this->numTestsPending) + " pending");


			result += "\n";

			return result;
		};


		private:
			PlatformString resultantOutputString = "";

			unsigned int currentDepth = 0;

			unsigned int numTestsPassed = 0;
			unsigned int numTestsFailed = 0;
			unsigned int numTestsPending = 0;


	};
	__MacchiatoUtilClass _MacchiatoUtil;



	PlatformString getResultantTestOutput() {
		return _MacchiatoUtil.getResultantOutput();
	};



	void describe(
		PlatformString description,
		function<void> describeCb
	) {
		// We just got to this depth
		_MacchiatoUtil.incrementDepth();

		// Log the current subject
		_MacchiatoUtil.log(_MacchiatoUtil.generateCurrentDepthString() + description + "\n");

		// Run the describe and get the results
		// They should of put some `it` calls inside the callback
		describeCb();

		// Now that we are done, reduce the depth
		_MacchiatoUtil.decrementDepth();
	};





	void it(PlatformString testDescription) {
		_MacchiatoUtil.logTestResult(_MacchiatoUtil.TestResultType::Pending);

		PlatformString message = _MacchiatoUtil.generateCurrentChildDepthString() +
			_MacchiatoUtil.wrapInAnsiCyan(PlatformString("----: ") + testDescription);

		_MacchiatoUtil.log(message);
	};

	void it(PlatformString testDescription, function<Macchiato::TestResult> itCb) {
		Macchiato::TestResult testResult = itCb();

		_MacchiatoUtil.logTestResult(testResult.didPass ? _MacchiatoUtil.TestResultType::Pass : _MacchiatoUtil.TestResultType::Fail);

		PlatformString message = _MacchiatoUtil.generateCurrentChildDepthString() + 
			(testResult.didPass ? _MacchiatoUtil.wrapInAnsiGreen("Pass") : _MacchiatoUtil.wrapInAnsiRed("Fail")) + ": " + 
			testDescription + 
			(testResult.didPass ? "" : "\n" + _MacchiatoUtil.generateCurrentChildDepthString() + "\t" + testResult.message) + 
			"\n";

		_MacchiatoUtil.log(message);
	};




	// expect: BDD
	struct expectFlags {
		bool negate = false;
	};
	template <typename Ta, typename Te = Ta>
	struct expect {
		
		expect(Ta actual) {
			this->actual = actual;
		};
		


		expect* equal(Te expected) {
			this->addTestResult(
				this->actual == expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "equal " + PlatformString(expected)
			);
			
			return this;
		};
		expect* eql(Te expected) {
			return this->equal(expected);
		};
		
		expect* closeTo(double expected) {
			return this->closeTo(expected, 0.0001);
		};
		expect* closeTo(double expected, double tolerance) {
			this->addTestResult(
				fabs(this->actual - expected) <= tolerance,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "equal " + PlatformString(expected) + " within tolerance of " + PlatformString(tolerance)
			);
			
			return this;
		};

		expect* within(double lower, double upper) {
			this->addTestResult(
				this->actual > lower && this->actual < upper,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be above " + PlatformString(lower) + " and below " + PlatformString(upper)
			);
			
			return this;
		};


		expect* above(double expected) {
			this->addTestResult(
				this->actual > expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be greater than " + PlatformString(expected)
			);
			
			return this;
		};
		expect* gt(double expected) {
			return this->above(expected);
		};
		expect* greaterThan(double expected) {
			return this->above(expected);
		};

		expect* least(double expected) {
			this->addTestResult(
				this->actual >= expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be greater than or equal to " + PlatformString(expected)
			);
			
			return this;
		};
		expect* gte(double expected) {
			return this->least(expected);
		};

		expect* below(double expected) {
			this->addTestResult(
				this->actual < expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be lesser than " + PlatformString(expected)
			);
			
			return this;
		};
		expect* lt(double expected) {
			return this->below(expected);
		};
		expect* lessThan(double expected) {
			return this->below(expected);
		};

		expect* most(double expected) {
			this->addTestResult(
				this->actual <= expected,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "be less than or equal to " + PlatformString(expected)
			);
			
			return this;
		};
		expect* lte(double expected) {
			return this->most(expected);
		};

		expect* satisfy(function<bool, Ta> testFunc) {
			bool testResultBool = testFunc(this->actual);
			
			return this->satisfy(
				testResultBool,
				PlatformString("Expected ") + PlatformString(this->actual) + " to " + (this->flags.negate ? "not " : "") + "satisfy the given test"
			);
		};
		expect* satisfy(function<bool, Ta> testFunc, function<PlatformString, Ta, expectFlags> failMessageFunc) {
			bool testResultBool = testFunc(this->actual);
			PlatformString failMessage = failMessageFunc(this->actual, this->flags);
			
			return this->satisfy(testResultBool, failMessage);
		};
		expect* satisfy(bool testResultBool, PlatformString failMessage) {
			this->addTestResult(
				testResultBool,
				failMessage
			);
			
			return this;
		};
		
		


		class MemberLogicClass {
			expect* expectPointer;
			function<void, expect*> getterFunc;
			public:
				MemberLogicClass(expect *i, function<void, expect*> getterFunc) : expectPointer(i), getterFunc(getterFunc) {};
				
				// Setter
				expect* operator = (const expect i) {
					return this->expectPointer = (expect*)&i;
					
				};
				// Setter
				expect* operator = (const expect *i) {
					return this->expectPointer = (expect*)i;
				};
				// Getter
				expect* operator -> () {
					this->getterFunc(this->expectPointer);
					return this->expectPointer; 
				};
				// Getter
				operator expect* () const {
					this->getterFunc(this->expectPointer);
					return this->expectPointer;
				};
		};


		// Sets the negate flag when used
		// expect<int>(3).to->never->equal->(5);
		MemberLogicClass never{this, [&](expect* expectPointer) {
			expectPointer->flags.negate = !expectPointer->flags.negate;
		}};


		
		// Provided as chainable getters to improve the readability of your assertions.
		// They do not provide testing capabilities.
		expect* to = this;
		expect* be = this;
		expect* been = this;
		expect* is = this;
		expect* that = this;
		expect* which = this;
		// `and` is a reserved keyword
		expect* then = this;//expect* and = this;
		expect* has = this;
		expect* have = this;
		expect* with = this;
		expect* at = this;
		expect* of = this;
		expect* same = this;
		
		
		
		TestResult getResult() {
			return this->testResult;
		};
		
		operator bool() {
			return this->testResult.didPass;
		};


		protected: 
			Ta actual;
			expectFlags flags;
			
			TestResult testResult;
			
			void addTestResult(bool testResultBool, PlatformString message) {
				bool didPass = (this->flags.negate ? !testResultBool : testResultBool);

				this->testResult.didPass = this->testResult.didPass && didPass;

				if(!didPass) {
					// Concat a newline if this is a consecutive test
					if(this->testResult.message.value.length() > 0) {
						this->testResult.message += "\n";
					}
					this->testResult.message += message;
				}

				

				// Reset the flag
				this->flags.negate = false;
			};


		//private:
	};





}


#endif