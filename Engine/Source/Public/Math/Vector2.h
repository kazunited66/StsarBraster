#pragma once

using namespace std; 

struct Vector2 {

	//simple constructor 
	Vector2() : x(0.0f), y(0.0f) {}

	//assigned constuctor 
	Vector2(float x, float y) : x(x), y(y) {}

	//uniform constructor 
	Vector2(float uniform) : x(uniform), y(uniform) {}

	~Vector2() = default;


	//OPERATORS

	//vector2 assingmed operator
	Vector2& operator=(const Vector2& Other) {
		x = Other.x;
		y = Other.y;

		return *this;
	}

	//boolean operators 
	//if 2 Vector2s are equal
	bool operator == (const Vector2& Other) const{
		return x == Other.x && y == Other.y;
	}

	//if 2 Vector2s are not equal
	bool operator!=(const Vector2& Other) const {
		return x != Other.x || y != Other.y;

	}

	//arithmetic operators 

	//addition
	Vector2 operator+(const Vector2& Other) const {
		return Vector2(Other.x + x, Other.y + y);
	}

	//addition equals
	Vector2& operator+=(const Vector2& Other) {

		return*this = *this + Other;

	}
	//subtract
	Vector2 operator-(const Vector2& Other) const {
		return Vector2(x - Other.x, y - Other.y);
	}
	//subtract euqals
	Vector2& operator-=(const Vector2& Other) {
		return *this = *this - Other;
	}
	//multiply vector2 by vector2
	Vector2 operator*(const Vector2 Other) const {
		return Vector2(x * Other.x, y * Other.y);
	}
	//multiply equals vector2 by vector2
	Vector2& operator*=(const Vector2& Other) {
		return *this = *this * Other;
	}
	//multiply vector2 by float 
	Vector2 operator*(const float& Scalar) const {
		return Vector2(x * Scalar, y * Scalar);
	}
	//multiply equqals by float
	Vector2& operator*=(const float& Scalar) {
		return *this = *this * Scalar;
	}
	//divistion vector2
	Vector2 operator/(const Vector2& Other) const;

	//divisiton equals vector2
	Vector2& operator/=(const Vector2& Other);

	//division float 
	Vector2 operator/(const float& Scalar) const ;

	//division equals float 
	Vector2& operator/=(const float& Other);
    
	

	//return an infinitite negative vector 
	//otherwise know as invalid vector
	static Vector2 NaN();
	//get the squared length/magnitude/size of the vector2 
	//this is inaccurate but cheaper
	float LengthSqd() const {
		return x * x + y * y;
	}

	//Get the length/magnitude/size of the vector 
	float Length() const;

	//Get the distance between 2 vectors
	//this is inaccurate by double the size but cheaper 
	static float DistSqd( Vector2& V1, Vector2& V2) {
		Vector2 DistanceVector(V1 - V2);

		return DistanceVector.x * DistanceVector.x + DistanceVector.y * DistanceVector.y;

	}

	//get the distance between 2 vectors
	static float Distance(Vector2& V1, Vector2& V2);
	
	//change the vector2 to a normalised value 
	Vector2& Normalise();
	//return a normalissed vector based on another vector 
	Vector2 Normalised(const Vector2& Other);

	//log the vector to the console 
	void Log();

	//log the length of the vector2 to console 
	void LogLength();
	
	float x, y;


};