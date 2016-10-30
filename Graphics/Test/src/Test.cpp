#include "Test.h"
#include "AssertionException.h"

void Test::assertEquals(string v1, string v2){
	if(v1.compare(v2) != 0){
		throw new AssertionException(v1, v2, this->getName());
	}
}
void Test::assertEquals(int v1, int v2){
	if(v1 != v2){
		throw new AssertionException(v1, v2, this->getName());
	}
}
void Test::assertEquals(float v1, float v2){
	if(v1 != v2){
		throw new AssertionException(v1, v2, this->getName());
	}
}	
void Test::assertGraterThan(int v1, int v2){
	throw new AssertionException(this->getName()); // TODO
}
void Test::assertGraterThan(float v1, float v2){
	throw new AssertionException(this->getName()); // TODO
}
void Test::assertSmallerThan(int v1, int v2){
	throw new AssertionException(this->getName()); // TODO
}
void Test::assertSmallerThan(float v1, float v2){
	throw new AssertionException(this->getName()); // TODO
}
void Test::assertTrue(string field, bool v){
	if(!v){
		throw new AssertionException(field, this->getName());
	}
}