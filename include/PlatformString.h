// PlatformString
// v0.3.1
//
// Requires C++11. Works on the following platforms:
//		- Most desktop environments
//		- Visual Studio
//		- Arduino


#ifndef PlatformString_h
#define PlatformString_h


#if defined(ARDUINO)
#include "Arduino.h"

struct PlatformString {
	String value;
	
	PlatformString() 
		: PlatformString("")
	{
	};
	
	PlatformString(char const* val)
		: PlatformString(String(val))
	{
	};
	
	PlatformString(String val)
		: value(val)
	{
	};
	
	
	PlatformString(bool val)
		: value(val ? "true" : "false")
	{
	};
	
	PlatformString(int val)
		: value(String(val))
	{
	};
	PlatformString(unsigned int val)
		: value(String(val))
	{
	};
	
	PlatformString(double val)
		: PlatformString(val, 4) 
	{
	};
	
	PlatformString(double val, byte precision)
		: value(PlatformString::FloatToString(val, precision))
	{
	};
	
	PlatformString operator + (char const* s) {
		return PlatformString(this->value + String(s));
	};
	PlatformString operator + (const PlatformString &s) {
		return PlatformString(this->value + s.value);
	};

	// For left hand side operator overloading
	friend PlatformString operator + (char const* s, const PlatformString &ps) {
		return PlatformString(String(s) + ps.value);
	};

	PlatformString& operator +=  (char const* s) {
		this->value = this->value + String(s);
		return *this;
	};
	PlatformString& operator +=  (const PlatformString &s) {
		this->value = this->value + s.value;
		return *this;
	};
	
	operator String() { return this->value; }
	
	private:
		static String FloatToString(float val, byte precision) {
			// returns val with number of decimal places determine by precision
			// precision is a number from 0 to 6 indicating the desired decimial places
			// example: floatToString(3.1415, 2); // returns 3.14 (two decimal places)
		
			String output = "";
		
			if(val < 0.0){
				output += "-";
				val = -val;
			}
		
			output += int(val);  //prints the int part
			if(precision > 0) 
			{
				output += "."; // print the decimal point
		
				unsigned long frac;
				unsigned long mult = 1;
				byte padding = precision -1;
				while(precision--)
					mult *=10;
		
				if(val >= 0)
					frac = (val - int(val)) * mult;
				else
					frac = (int(val)- val ) * mult;
				unsigned long frac1 = frac;
		
				while( frac1 /= 10 )
					padding--;
				while(  padding--)
					output += "0";
		
				output += frac;
			}
		
			return output;
		};
};


#else
#include <string>
#include <iostream>
// std::min, std::max
#include <algorithm>

struct PlatformString {
	std::string value;
	
	PlatformString() 
		: PlatformString("")
	{
	};
	
	PlatformString(char const* val)
		: PlatformString(std::string(val))
	{
	};
	
	PlatformString(std::string val)
		: value(val) 
	{
	};
	
	
	PlatformString(bool val)
		: value(val ? "true" : "false")
	{
	};
	
	PlatformString(int val)
		: value(std::to_string(val))
	{
	};
	PlatformString(unsigned int val)
		: value(std::to_string(val))
	{
	};
	
	PlatformString(double val)
		: value(RemoveTrailingZeros(std::to_string(val)))
	{
	};
	
	PlatformString operator + (char const* s) {
		return PlatformString(this->value + std::string(s));
	};
	PlatformString operator + (const PlatformString &s) {
		return PlatformString(this->value + s.value);
	};

	// For left hand side operator overloading
	friend PlatformString operator + (char const* s, const PlatformString &ps) {
		return PlatformString(std::string(s) + ps.value);
	};

	PlatformString& operator += (char const* s) {
		this->value = this->value + std::string(s);
		return *this;
	};
	PlatformString& operator += (const PlatformString &s) {
		this->value = this->value + s.value;
		return *this;
	};
	
	friend std::ostream& operator << (std::ostream& stream, const PlatformString &s) {
		return stream << s.value;
	};

	operator std::string() { return this->value; }

	private:
		static std::string RemoveTrailingZeros(std::string str) {
			return str.erase((std::min)((int)str.find_last_not_of('0') + 2, (int)str.length()), std::string::npos);
		};
};

#endif


#endif