#pragma once
#include "Exception.h"
#include "StringUtils.h"

class AssertionException : public Exception {

public:
	AssertionException(string v1, string v2, string testName) : 
		Exception("Assertion failed on " + testName + ": " + v1 + " not equals " + v2){
	}
	AssertionException(int v1, int v2, string testName) : 
		Exception("Assertion failed on " + testName + ": " + StringUtils::toString(v1) + " not equals " + StringUtils::toString(v2)){
	}
	AssertionException(float v1, float v2, string testName) : 
		Exception("Assertion failed on " + testName + ": " + StringUtils::toString(v1) + " not equals " + StringUtils::toString(v2)){
	}
	AssertionException(string invalidField, string testName) : 
		Exception("Assertion failed on " + testName + ": invalid field " + invalidField){
	}
	AssertionException(string testName) : 
		Exception("Assertion failed on " + testName){
	}
};