/** KempoApi: The Turloc Toolkit *****************************/
/** *    *                                                  **/
/** **  **  Filename: ArcBall.h                             **/
/**   **    Version:  Common                                **/
/**   **                                                    **/
/**                                                         **/
/**  Arcball class for mouse manipulation.                  **/
/**                                                         **/
/**                                                         **/
/**                                                         **/
/**                                                         **/
/**                              (C) 1999-2003 Tatewake.com **/
/**   History:                                              **/
/**   08/17/2003 - (TJG) - Creation                         **/
/**   09/23/2003 - (TJG) - Bug fix and optimization         **/
/**   09/25/2003 - (TJG) - Version for NeHe Basecode users  **/
/**                                                         **/
/*************************************************************/

/*************************************************************************************/
/**                                                                                 **/
/** Copyright (c) 1999-2009 Tatewake.com                                            **/
/**                                                                                 **/
/** Permission is hereby granted, free of charge, to any person obtaining a copy    **/
/** of this software and associated documentation files (the "Software"), to deal   **/
/** in the Software without restriction, including without limitation the rights    **/
/** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       **/
/** copies of the Software, and to permit persons to whom the Software is           **/
/** furnished to do so, subject to the following conditions:                        **/
/**                                                                                 **/
/** The above copyright notice and this permission notice shall be included in      **/
/** all copies or substantial portions of the Software.                             **/
/**                                                                                 **/
/** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      **/
/** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        **/
/** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     **/
/** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          **/
/** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   **/
/** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN       **/
/** THE SOFTWARE.                                                                   **/
/**                                                                                 **/
/*************************************************************************************/

#pragma once

// 8<--Snip here if you have your own math types/funcs-->8 

# include <assert.h>

//Math types derived from the KempoApi tMath library
typedef union {
    struct {
	float X, Y;
    } s;

    float T[2];
} Tuple2fT;			//A generic 2-element tuple that is represented by single-precision floating point x,y coordinates. 

typedef union {
    struct {
	float X, Y, Z;
    } s;

    float T[3];
} Tuple3fT;			//A generic 3-element tuple that is represented by single precision-floating point x,y,z coordinates. 

typedef union {
    struct {
	float X, Y, Z, W;
    } s;

    float T[4];
} Tuple4fT;			//A 4-element tuple represented by single-precision floating point x,y,z,w coordinates. 

typedef union {
    float M[9];
    struct {
	//column major
	union {
	    float M00;
	    float XX;
	    float SX;
	};			//XAxis.X and Scale X
	union {
	    float M10;
	    float XY;
	};			//XAxis.Y
	union {
	    float M20;
	    float XZ;
	};			//XAxis.Z
	union {
	    float M01;
	    float YX;
	};			//YAxis.X
	union {
	    float M11;
	    float YY;
	    float SY;
	};			//YAxis.Y and Scale Y
	union {
	    float M21;
	    float YZ;
	};			//YAxis.Z
	union {
	    float M02;
	    float ZX;
	};			//ZAxis.X
	union {
	    float M12;
	    float ZY;
	};			//ZAxis.Y
	union {
	    float M22;
	    float ZZ;
	    float SZ;
	};			//ZAxis.Z and Scale Z
    } s;
} Matrix3fT;			//A single precision floating point 3 by 3 matrix. 

typedef union {
    float M[16];
    struct {
	//column major
	union {
	    float M00;
	    float XX;
	    float SX;
	};			//XAxis.X and Scale X
	union {
	    float M10;
	    float XY;
	};			//XAxis.Y
	union {
	    float M20;
	    float XZ;
	};			//XAxis.Z
	union {
	    float M30;
	    float XW;
	};			//XAxis.W
	union {
	    float M01;
	    float YX;
	};			//YAxis.X
	union {
	    float M11;
	    float YY;
	    float SY;
	};			//YAxis.Y and Scale Y
	union {
	    float M21;
	    float YZ;
	};			//YAxis.Z
	union {
	    float M31;
	    float YW;
	};			//YAxis.W
	union {
	    float M02;
	    float ZX;
	};			//ZAxis.X
	union {
	    float M12;
	    float ZY;
	};			//ZAxis.Y
	union {
	    float M22;
	    float ZZ;
	    float SZ;
	};			//ZAxis.Z and Scale Z
	union {
	    float M32;
	    float ZW;
	};			//ZAxis.W
	union {
	    float M03;
	    float TX;
	};			//Trans.X
	union {
	    float M13;
	    float TY;
	};			//Trans.Y
	union {
	    float M23;
	    float TZ;
	};			//Trans.Z
	union {
	    float M33;
	    float TW;
	    float SW;
	};			//Trans.W and Scale W
    } s;
} Matrix4fT;			//A single precision floating point 4 by 4 matrix. 


//"Inherited" types
#define Point2fT    Tuple2fT	//A 2 element point that is represented by single precision floating point x,y coordinates.

#define Quat4fT     Tuple4fT	//A 4 element unit quaternion represented by single precision floating point x,y,z,w coordinates.

#define Vector2fT   Tuple2fT	//A 2-element vector that is represented by single-precision floating point x,y coordinates.
#define Vector3fT   Tuple3fT	//A 3-element vector that is represented by single-precision floating point x,y,z coordinates.

//Custom math, or speed overrides
#define FuncSqrt    sqrtf

//utility macros
//assuming IEEE-754(float), which i believe has max precision of 7 bits
# define Epsilon 1.0e-5

//Math functions

#ifdef ARCBALL_C
    /**
      * Sets this vector to be the vector cross product of vectors v1 and v2.
      * @param v1 the first vector
      * @param v2 the second vector
      */

static void Vector3fCross(Vector3fT * NewObj, const Vector3fT * v1,
			  const Vector3fT * v2)
{
    Vector3fT Result;		//safe not to initialize

    assert(NewObj && v1 && v2);

    // store on stack once for aliasing-safty
    // i.e. safe when a.cross(a, b)

    Result.s.X = (v1->s.Y * v2->s.Z) - (v1->s.Z * v2->s.Y);
    Result.s.Y = (v1->s.Z * v2->s.X) - (v1->s.X * v2->s.Z);
    Result.s.Z = (v1->s.X * v2->s.Y) - (v1->s.Y * v2->s.X);

    //copy result back
    *NewObj = Result;
}

    /**
      * Computes the dot product of the this vector and vector v1.
      * @param  v1 the other vector
      */

static float Vector3fDot(const Vector3fT * NewObj, const Vector3fT * v1)
{
    assert(NewObj && v1);

    return (NewObj->s.X * v1->s.X) +
	(NewObj->s.Y * v1->s.Y) + (NewObj->s.Z * v1->s.Z);
}

    /**
      * Returns the squared length of this vector.
      * @return the squared length of this vector
      */

static float Vector3fLengthSquared(const Vector3fT * NewObj)
{
    assert(NewObj);

    return (NewObj->s.X * NewObj->s.X) +
	(NewObj->s.Y * NewObj->s.Y) + (NewObj->s.Z * NewObj->s.Z);
}

    /**
      * Returns the length of this vector.
      * @return the length of this vector
      */

static float Vector3fLength(const Vector3fT * NewObj)
{
    assert(NewObj);

    return FuncSqrt(Vector3fLengthSquared(NewObj));
}

    /**
      * Sets the value of this matrix to the matrix conversion of the
      * quaternion argument. 
      * @param q1 the quaternion to be converted 
      */
    //$hack this can be optimized some(if s == 0)

static void Matrix3fSetRotationFromQuat4f(Matrix3fT * NewObj,
					  const Quat4fT * q1)
{
    float n, s;
    float xs, ys, zs;
    float wx, wy, wz;
    float xx, xy, xz;
    float yy, yz, zz;

    assert(NewObj && q1);

    n = (q1->s.X * q1->s.X) + (q1->s.Y * q1->s.Y) + (q1->s.Z * q1->s.Z) +
	(q1->s.W * q1->s.W);
    s = (n > 0.0f) ? (2.0f / n) : 0.0f;

    xs = q1->s.X * s;
    ys = q1->s.Y * s;
    zs = q1->s.Z * s;
    wx = q1->s.W * xs;
    wy = q1->s.W * ys;
    wz = q1->s.W * zs;
    xx = q1->s.X * xs;
    xy = q1->s.X * ys;
    xz = q1->s.X * zs;
    yy = q1->s.Y * ys;
    yz = q1->s.Y * zs;
    zz = q1->s.Z * zs;

    NewObj->s.XX = 1.0f - (yy + zz);
    NewObj->s.YX = xy - wz;
    NewObj->s.ZX = xz + wy;
    NewObj->s.XY = xy + wz;
    NewObj->s.YY = 1.0f - (xx + zz);
    NewObj->s.ZY = yz - wx;
    NewObj->s.XZ = xz - wy;
    NewObj->s.YZ = yz + wx;
    NewObj->s.ZZ = 1.0f - (xx + yy);
}

    /**
     * Sets the value of this matrix to the result of multiplying itself
     * with matrix m1. 
     * @param m1 the other matrix 
     */

static void Matrix3fMulMatrix3f(Matrix3fT * NewObj, const Matrix3fT * m1)
{
    Matrix3fT Result;		//safe not to initialize

    assert(NewObj && m1);

    // alias-safe way.
    Result.s.M00 =
	(NewObj->s.M00 * m1->s.M00) + (NewObj->s.M01 * m1->s.M10) +
	(NewObj->s.M02 * m1->s.M20);
    Result.s.M01 =
	(NewObj->s.M00 * m1->s.M01) + (NewObj->s.M01 * m1->s.M11) +
	(NewObj->s.M02 * m1->s.M21);
    Result.s.M02 =
	(NewObj->s.M00 * m1->s.M02) + (NewObj->s.M01 * m1->s.M12) +
	(NewObj->s.M02 * m1->s.M22);

    Result.s.M10 =
	(NewObj->s.M10 * m1->s.M00) + (NewObj->s.M11 * m1->s.M10) +
	(NewObj->s.M12 * m1->s.M20);
    Result.s.M11 =
	(NewObj->s.M10 * m1->s.M01) + (NewObj->s.M11 * m1->s.M11) +
	(NewObj->s.M12 * m1->s.M21);
    Result.s.M12 =
	(NewObj->s.M10 * m1->s.M02) + (NewObj->s.M11 * m1->s.M12) +
	(NewObj->s.M12 * m1->s.M22);

    Result.s.M20 =
	(NewObj->s.M20 * m1->s.M00) + (NewObj->s.M21 * m1->s.M10) +
	(NewObj->s.M22 * m1->s.M20);
    Result.s.M21 =
	(NewObj->s.M20 * m1->s.M01) + (NewObj->s.M21 * m1->s.M11) +
	(NewObj->s.M22 * m1->s.M21);
    Result.s.M22 =
	(NewObj->s.M20 * m1->s.M02) + (NewObj->s.M21 * m1->s.M12) +
	(NewObj->s.M22 * m1->s.M22);

    //copy result back to this
    *NewObj = Result;
}


static void Matrix4fSetRotationScaleFromMatrix4f(Matrix4fT * NewObj,
						 const Matrix4fT * m1)
{
    assert(NewObj && m1);

    NewObj->s.XX = m1->s.XX;
    NewObj->s.YX = m1->s.YX;
    NewObj->s.ZX = m1->s.ZX;
    NewObj->s.XY = m1->s.XY;
    NewObj->s.YY = m1->s.YY;
    NewObj->s.ZY = m1->s.ZY;
    NewObj->s.XZ = m1->s.XZ;
    NewObj->s.YZ = m1->s.YZ;
    NewObj->s.ZZ = m1->s.ZZ;
}

    /**
      * Performs SVD on this matrix and gets scale and rotation.
      * Rotation is placed into rot3, and rot4.
      * @param rot3 the rotation factor(Matrix3d). if null, ignored
      * @param rot4 the rotation factor(Matrix4) only upper 3x3 elements are changed. if null, ignored
      * @return scale factor
      */

static float Matrix4fSVD(const Matrix4fT * NewObj, Matrix3fT * rot3,
			   Matrix4fT * rot4)
{
    float s, n;

    assert(NewObj);

    // this is a simple svd.
    // Not complete but fast and reasonable.
    // See comment in Matrix3d.

    s = FuncSqrt(((NewObj->s.XX * NewObj->s.XX) +
		  (NewObj->s.XY * NewObj->s.XY) +
		  (NewObj->s.XZ * NewObj->s.XZ) +
		  (NewObj->s.YX * NewObj->s.YX) +
		  (NewObj->s.YY * NewObj->s.YY) +
		  (NewObj->s.YZ * NewObj->s.YZ) +
		  (NewObj->s.ZX * NewObj->s.ZX) +
		  (NewObj->s.ZY * NewObj->s.ZY) +
		  (NewObj->s.ZZ * NewObj->s.ZZ)) / 3.0f);

    if (rot3)			//if pointer not null
    {
	rot3->s.XX = NewObj->s.XX;
	rot3->s.XY = NewObj->s.XY;
	rot3->s.XZ = NewObj->s.XZ;
	rot3->s.YX = NewObj->s.YX;
	rot3->s.YY = NewObj->s.YY;
	rot3->s.YZ = NewObj->s.YZ;
	rot3->s.ZX = NewObj->s.ZX;
	rot3->s.ZY = NewObj->s.ZY;
	rot3->s.ZZ = NewObj->s.ZZ;

	// zero-div may occur.

	n = 1.0f / FuncSqrt((NewObj->s.XX * NewObj->s.XX) +
			    (NewObj->s.XY * NewObj->s.XY) +
			    (NewObj->s.XZ * NewObj->s.XZ));
	rot3->s.XX *= n;
	rot3->s.XY *= n;
	rot3->s.XZ *= n;

	n = 1.0f / FuncSqrt((NewObj->s.YX * NewObj->s.YX) +
			    (NewObj->s.YY * NewObj->s.YY) +
			    (NewObj->s.YZ * NewObj->s.YZ));
	rot3->s.YX *= n;
	rot3->s.YY *= n;
	rot3->s.YZ *= n;

	n = 1.0f / FuncSqrt((NewObj->s.ZX * NewObj->s.ZX) +
			    (NewObj->s.ZY * NewObj->s.ZY) +
			    (NewObj->s.ZZ * NewObj->s.ZZ));
	rot3->s.ZX *= n;
	rot3->s.ZY *= n;
	rot3->s.ZZ *= n;
    }

    if (rot4)			//if pointer not null
    {
	if (rot4 != NewObj) {
	    Matrix4fSetRotationScaleFromMatrix4f(rot4, NewObj);	// private method
	}
	// zero-div may occur.

	n = 1.0f / FuncSqrt((NewObj->s.XX * NewObj->s.XX) +
			    (NewObj->s.XY * NewObj->s.XY) +
			    (NewObj->s.XZ * NewObj->s.XZ));
	rot4->s.XX *= n;
	rot4->s.XY *= n;
	rot4->s.XZ *= n;

	n = 1.0f / FuncSqrt((NewObj->s.YX * NewObj->s.YX) +
			    (NewObj->s.YY * NewObj->s.YY) +
			    (NewObj->s.YZ * NewObj->s.YZ));
	rot4->s.YX *= n;
	rot4->s.YY *= n;
	rot4->s.YZ *= n;

	n = 1.0f / FuncSqrt((NewObj->s.ZX * NewObj->s.ZX) +
			    (NewObj->s.ZY * NewObj->s.ZY) +
			    (NewObj->s.ZZ * NewObj->s.ZZ));
	rot4->s.ZX *= n;
	rot4->s.ZY *= n;
	rot4->s.ZZ *= n;
    }

    return s;
}


static void Matrix4fSetRotationScaleFromMatrix3f(Matrix4fT * NewObj,
						 const Matrix3fT * m1)
{
    assert(NewObj && m1);

    NewObj->s.XX = m1->s.XX;
    NewObj->s.YX = m1->s.YX;
    NewObj->s.ZX = m1->s.ZX;
    NewObj->s.XY = m1->s.XY;
    NewObj->s.YY = m1->s.YY;
    NewObj->s.ZY = m1->s.ZY;
    NewObj->s.XZ = m1->s.XZ;
    NewObj->s.YZ = m1->s.YZ;
    NewObj->s.ZZ = m1->s.ZZ;
}


static void Matrix4fMulRotationScale(Matrix4fT * NewObj, float scale)
{
    assert(NewObj);

    NewObj->s.XX *= scale;
    NewObj->s.YX *= scale;
    NewObj->s.ZX *= scale;
    NewObj->s.XY *= scale;
    NewObj->s.YY *= scale;
    NewObj->s.ZY *= scale;
    NewObj->s.XZ *= scale;
    NewObj->s.YZ *= scale;
    NewObj->s.ZZ *= scale;
}

    /**
      * Sets the rotational component (upper 3x3) of this matrix to the matrix
      * values in the T precision Matrix3d argument; the other elements of
      * this matrix are unchanged; a singular value decomposition is performed
      * on this object's upper 3x3 matrix to factor out the scale, then this
      * object's upper 3x3 matrix components are replaced by the passed rotation
      * components, and then the scale is reapplied to the rotational
      * components.
      * @param m1 T precision 3x3 matrix
      */

static void Matrix4fSetRotationFromMatrix3f(Matrix4fT * NewObj,
					    const Matrix3fT * m1)
{
    float scale;

    assert(NewObj && m1);

    scale = Matrix4fSVD(NewObj, NULL, NULL);

    Matrix4fSetRotationScaleFromMatrix3f(NewObj, m1);
    Matrix4fMulRotationScale(NewObj, scale);
}
#endif

// 8<--Snip here if you have your own math types/funcs-->8 
struct _ArcBall_t {
    Vector3fT StVec;
    Vector3fT EnVec;
    float AdjustWidth;
    float AdjustHeight;
    Matrix4fT Transform;
    Matrix3fT LastRot;
    Matrix3fT ThisRot;
    Point2fT MousePt;
    int isClicked;
    int isRClicked;
    int isDragging;
};


void init_arcBall(ArcBall_t * a, float NewWidth, float NewHeight);
void arcmouseClick(void);
void arcmouseDrag(void);
