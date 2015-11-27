// DrObject.h: interface for the DrObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROBJECT_H__BE5E44B7_F789_431D_98B6_78BEB1154E50__INCLUDED_)
#define AFX_DROBJECT_H__BE5E44B7_F789_431D_98B6_78BEB1154E50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl\gl.h>              // Header File For The OpenGL32 Library
#include <gl\glu.h>             // Header File For The GLu32 Library
#include <gl\glaux.h>			// Header File For The GLaux Library

#include "String.h"

typedef struct
{
	float   x, y, z;
} Dr_Vector;

typedef struct
{
	long   a, b, c,texnum,filltype;
	float  ax,ay,bx,by,cx,cy;
} Dr_Face;

class DrObject  
{
public:
	void toList_cubeNormals(float weight);
	void calc_normals_loota();
	float tx_scrollY;
	float tx_scrollX;
	void calc_rnormals();
	void renderPoints();
	bool inited;
	void flipNormals();
	void base2roto();
	void wobble4(float time, float div, float amount);
	void wobble3(float time, float div, float amount);
	void wobble2(float time, float div, float amount);
	void wobble1(float time, float div, float amount);
	void tile(float uMul, float vMul);
	void render();
	void drawList();
	GLuint callList;
	void toList();
	void calc_normals();
	void load(String name);
	DrObject();
	virtual ~DrObject();

	long numpoints,numfaces,filltype,texnum;
	float xrot,yrot,zrot,xpos,ypos,zpos;
	long blobdiv, blobadd, colormul;
	int normals;
	Dr_Vector *normv;    
	Dr_Vector *rnormv; 
	Dr_Vector *normf;  
	Dr_Vector *rnormf; 
	Dr_Vector *verts;  
	Dr_Face *faces;    
	Dr_Vector *rverts; 
	int *vertex_sort;
private:
	void join_normals(Dr_Vector *a, Dr_Vector *b, float weight, Dr_Vector *dest);

	bool flip_normals;
	void init();
	void error(String txt);
};

#endif // !defined(AFX_DROBJECT_H__BE5E44B7_F789_431D_98B6_78BEB1154E50__INCLUDED_)
