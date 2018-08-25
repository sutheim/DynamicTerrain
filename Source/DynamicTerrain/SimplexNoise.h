// SimplexNoise
// Copyright © 2003-2011, Stefan Gustavson
//
// Contact: stegu@itn.liu.se
//
// This library is public domain software, released by the author
// into the public domain in February 2011. You may do anything
// you like with it. You may even remove all attributions,
// but of course I'd appreciate it if you kept my name somewhere.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

/** \file
\brief Declares the SimplexNoise class for producing Perlin simplex noise.
\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
* This is a clean, fast, modern and free Perlin Simplex noise class in C++.
* Being a stand-alone class with no external dependencies, it is
* highly reusable without source code modifications.
*
*
* Note:
* Replacing the "float" type with "double" can actually make this run faster
* on some platforms. A templatized version of SimplexNoise could be useful.
*/

#pragma once

#include "CoreMinimal.h"



class DYNAMICTERRAIN_API SimplexNoise
{
public:
	SimplexNoise() {}
	~SimplexNoise() {}

	/** 1D, 2D, 3D and 4D float Perlin noise
	*/
	static float sNoise(float x);
	static float sNoise(float x, float y);
	static float sNoise(float x, float y, float z);
	static float sNoise(float x, float y, float z, float w);

private:
	static unsigned char perm[];
	static float  grad(int hash, float x);
	static float  grad(int hash, float x, float y);
	static float  grad(int hash, float x, float y, float z);
	static float  grad(int hash, float x, float y, float z, float t);

};