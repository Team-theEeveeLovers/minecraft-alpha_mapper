// 
/**
* @file VectorTypes.h
* @brief A header file containing vector types
* @author Team theEeveeLovers Sean
* @date August 22nd, 2024
*/
#ifndef VECTOR_TYPES
#define VECTOR_TYPES
#include "../globals.h"




// These types hold 2 values
// They are perfect for storing coordinates in a 2 dimensional space
/**
 * @brief A vector type holding two integers.
 * It can be constructed using @code vector2_int(x, y) @endcode 
 * or just @code vector2_int() @endcode for a default containing 0's
 */
typedef struct vector2_int {
	int										x, y;
	constexpr vector2_int()					: x(0), y(0) { }
	constexpr vector2_int(int _x, int _y)	: x(_x), y(_y) { }
} vector2_int;

/**
 * @brief A vector type holding two floats.
 * It can be constructed using @code vector2_float(x, y) @endcode
 * or just @code vector2_float() @endcode for a default containing 0's
 */
typedef struct vector2_float {
	float									x, y;
	constexpr vector2_float() : x(0.f), y(0.f) { }
	constexpr vector2_float(float _x, float _y) : x(_x), y(_y) { }
} vector2_float;




// These types hold 3 values
// They are perfect for storing coordinates in a 3 dimensional space
/**
 * @brief A vector type holding three integers.
 * It can be constructed using @code vector3_int(x, y, z) @endcode
 * or just @code vector3_int() @endcode for a default containing 0's
 */
typedef struct vector3_int {
	int										x, y, z;
	constexpr vector3_int() : x(0), y(0), z(0) { }
	constexpr vector3_int(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }
} vector3_int;

/**
 * @brief A vector type holding three floats.
 * It can be constructed using @code vector3_float(x, y, z) @endcode
 * or just @code vector3_float() @endcode for a default containing 0's
 */
typedef struct vector3_float {
	float									x, y, z;
	constexpr vector3_float() : x(0.f), y(0.f), z(0.f) { }
	constexpr vector3_float(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
} vector3_float;




// These types hold 4 values, which makes them similar to SDL_Rect and SDL_FRect respectively
// The main difference is these have a constructor when SDL Rects are constructed using an array
// They are perfect for storing a pair of coordinates in a 2 dimensional space, possibly for defining two points of a rectangle
/**
 * @brief A vector type holding four integers.
 * It can be constructed using @code vector4_int(x, y, z, w) @endcode
 * or just @code vector4_int() @endcode for a default containing 0's
 */
typedef struct vector4_int {
	int										x, y, z, w;
	constexpr vector4_int() : x(0), y(0), z(0), w(0) { }
	constexpr vector4_int(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) { }
} vector4_int;

/**
 * @brief A vector type holding four floats.
 * It can be constructed using @code vector4_float(x, y, z, w) @endcode
 * or just @code vector4_float() @endcode for a default containing 0's
 */
typedef struct vector4_float {
	float									x, y, z, w;
	constexpr vector4_float() : x(0.f), y(0.f), z(0.f), w(0.f) { }
	constexpr vector4_float(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
} vector4_float;




#endif 