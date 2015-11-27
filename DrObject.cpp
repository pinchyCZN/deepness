// DrObject.cpp: implementation of the DrObject class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stream.h"
#include "DrObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrObject::DrObject()
{
	inited=false;
	flip_normals=false;
	tx_scrollY=0;
	tx_scrollX=0;
}

DrObject::~DrObject()
{
	if (inited)
	{
		delete verts;
		delete rverts;
		delete faces;
		delete normv;
		delete rnormv;
		delete normf;
		delete rnormf;
		delete vertex_sort;
	}
}

void DrObject::error(String txt)
{
	ShowCursor (1);  // show cursor
    MessageBox(0, txt.c_String(), "Error", MB_OK|MB_ICONERROR);
    PostQuitMessage(0);
}

void DrObject::load(String name)
{
	unsigned char longe[2];
	long i,q,l;
	char virheilmo[256];
	HjbStream *tiedosto = new HjbStream(name, "rb");


	if (tiedosto->getError())
	{
		tiedosto->close();
		error(name+" cannot load 3dobject");
		return;
	}


	numpoints=tiedosto->readWord();
	numfaces=tiedosto->readWord();

//	error(name+" "+(int)numpoints+" "+(int)numfaces);

	init();

	filltype=0;
	texnum=0;
	for (l=0;l<numpoints;l++)
	{
		verts[l].x=(signed short)tiedosto->readWord();
		rverts[l].x=verts[l].x;

		verts[l].y=(signed short)tiedosto->readWord();
		rverts[l].y=verts[l].y;

		verts[l].z=(signed short)tiedosto->readWord();
		rverts[l].z=verts[l].z;
	}
	for (l=0;l<numfaces;l++)
	{
		faces[l].a=tiedosto->readWord();
		faces[l].b=tiedosto->readWord();
		faces[l].c=tiedosto->readWord();

		faces[l].texnum=tiedosto->readWord();
		faces[l].filltype=0;//tiedosto->readWord();

		faces[l].ax=(signed short)tiedosto->readWord()/256.0;
		faces[l].ay=1.0-(signed short)tiedosto->readWord()/256.0;
		faces[l].bx=(signed short)tiedosto->readWord()/256.0;
		faces[l].by=1.0-(signed short)tiedosto->readWord()/256.0;
		faces[l].cx=(signed short)tiedosto->readWord()/256.0;
		faces[l].cy=1.0-(signed short)tiedosto->readWord()/256.0;
	}
	tiedosto->close();
	normals=0;
	calc_normals();
	toList();
}

void DrObject::init()
{
	inited=true;
/*
	verts  = ( Dr_Vector    *)malloc(numpoints*sizeof(Dr_Vector));
	rverts = ( Dr_Vector    *)malloc(numpoints*sizeof(Dr_Vector));
	faces  = ( Dr_Face      *)malloc(numfaces *sizeof(Dr_Face));
	normv  = ( Dr_Vector    *)malloc(numpoints*sizeof(Dr_Vector));
	rnormv = ( Dr_Vector    *)malloc(numpoints*sizeof(Dr_Vector));
	normf  = ( Dr_Vector    *)malloc(numfaces *sizeof(Dr_Vector));
	rnormf = ( Dr_Vector    *)malloc(numfaces *sizeof(Dr_Vector));
	vertex_sort = ( int     *)malloc(numpoints*4+16);
*/

/*	String name="haista paska";

	error(name+" "+(int)numpoints+" "+(int)numfaces);
*/
	verts	= new Dr_Vector[numpoints];
	rverts	= new Dr_Vector[numpoints];
	faces	= new Dr_Face[numfaces];
	normv	= new Dr_Vector[numpoints];
	rnormv	= new Dr_Vector[numpoints];
	normf	= new Dr_Vector[numfaces];
	rnormf	= new Dr_Vector[numfaces];
	vertex_sort = new int[numpoints*16];


/*	if(!obj->verts||!obj->rverts||!obj->faces||!obj->normv||!obj->rnormv||!obj->normf||!obj->rnormf)
	error("out of memory");
*/
	xrot = 0;
	yrot = 0;
	zrot = 0;
	xpos = 0;
	ypos = 0;
	zpos = 0;
	filltype=0;
	blobdiv=64;
	blobadd=64;
	colormul=16;

	for (int i=0;i<numpoints;i++)
		vertex_sort[i]=i;
}

void DrObject::calc_normals()
{
	float ax,bx,ay,by,az,bz;
	float nx, ny, nz, nl;
	int q,n,l;

	for(q=0;q<numfaces;q++) 
	{
		ax =   (rverts[faces[q].b].x - rverts[faces[q].a].x);
		ay =   (rverts[faces[q].b].y - rverts[faces[q].a].y);
		az =   (rverts[faces[q].b].z - rverts[faces[q].a].z);
		bx =   (rverts[faces[q].c].x - rverts[faces[q].a].x);
		by =   (rverts[faces[q].c].y - rverts[faces[q].a].y);
		bz =   (rverts[faces[q].c].z - rverts[faces[q].a].z);

		nx = ay * bz - az * by; 
		ny = az * bx - ax * bz; 
		nz = ax * by - ay * bx;

		nl = sqrt(nx*nx + ny*ny + nz*nz);

		if(nl!=0) 
		{ 
			nl=1.0/nl;
			normf[q].x = nx*nl;
			normf[q].y = ny*nl;
			normf[q].z = nz*nl;     
		}
		else {
			normf[q].x = 0;
			normf[q].y = 0;
			normf[q].z = 1;     
		}
	}

	for(l=0;l<numpoints;l++) 
	{
		nx=0; ny=0; nz=0;
		for(q=0;q<numfaces;q++) 
		{
			if ( (faces[q].a == l) || (faces[q].b == l) || (faces[q].c == l) ) 
			{
				nx+=normf[q].x;
				ny+=normf[q].y;
				nz+=normf[q].z;
			}
		}

		nl = sqrt(nx*nx + ny*ny + nz*nz);

		if(nl!=0) 
		{
			nl=1/nl;
			//nl=nl/400.0;
			normv[l].x = nx*nl;
			normv[l].y = ny*nl;
			normv[l].z = nz*nl;     
		}
		else 
		{
			normv[l].x = 0;
			normv[l].y = 0;
			normv[l].z = 1;
		}
	}
	memcpy(rnormv,normv,numpoints*sizeof(Dr_Vector));
	memcpy(rnormf,normf,numfaces*sizeof(Dr_Vector));
}

void DrObject::toList()
{
	callList = glGenLists(1);
	glNewList(callList, GL_COMPILE);

	render();

	glEndList();
}

void DrObject::drawList()
{
	glCallList(callList);
}

void DrObject::render()
{
	glBegin(GL_TRIANGLES);
	for (int i=0;i<numfaces;i++)
	{
		int t1=faces[i].a;
		int t2=faces[i].b;
		int t3=faces[i].c;

		if (!flip_normals) 
		{
			glTexCoord2f(faces[i].ax+tx_scrollX, faces[i].ay+tx_scrollY); 
			glNormal3f(rnormv[t1].x, rnormv[t1].y, rnormv[t1].z);
			glVertex3f(rverts[t1].x, rverts[t1].y, rverts[t1].z);
		}
		else 
		{
			glTexCoord2f(faces[i].ax+tx_scrollX, faces[i].ay+tx_scrollY); 
//			glNormal3f(rnormv[t1].x, rnormv[t1].y, rnormv[t1].z);
			glNormal3f(-rnormv[t1].x, -rnormv[t1].y, -rnormv[t1].z);
			glVertex3f(rverts[t1].x, rverts[t1].y, rverts[t1].z);
		}


		if (!flip_normals) 
		{
			glTexCoord2f(faces[i].bx+tx_scrollX, faces[i].by+tx_scrollY); 
			glNormal3f(rnormv[t2].x, rnormv[t2].y, rnormv[t2].z);
			glVertex3f(rverts[t2].x, rverts[t2].y, rverts[t2].z);
		}
		else 
		{
			glTexCoord2f(faces[i].cx+tx_scrollX, faces[i].cy+tx_scrollY); 
//			glNormal3f(rnormv[t3].x, rnormv[t3].y, rnormv[t3].z);
			glNormal3f(-rnormv[t3].x, -rnormv[t3].y, -rnormv[t3].z);
			glVertex3f(rverts[t3].x, rverts[t3].y, rverts[t3].z);
		}


		if (!flip_normals) 
		{
			glTexCoord2f(faces[i].cx+tx_scrollX, faces[i].cy+tx_scrollY); 
			glNormal3f(rnormv[t3].x, rnormv[t3].y, rnormv[t3].z);
			glVertex3f(rverts[t3].x, rverts[t3].y, rverts[t3].z);
		}
		else 
		{
			glTexCoord2f(faces[i].bx+tx_scrollX, faces[i].by+tx_scrollY); 
//			glNormal3f(rnormv[t2].x, rnormv[t2].y, rnormv[t2].z);
			glNormal3f(-rnormv[t2].x, -rnormv[t2].y, -rnormv[t2].z);
			glVertex3f(rverts[t2].x, rverts[t2].y, rverts[t2].z);
		}
		
	}
	glEnd();
}

void DrObject::renderPoints()
{
	glBegin(GL_POINTS);
	for (int i=0;i<numpoints;i++)
	{
		glVertex3f(rverts[i].x,rverts[i].y,rverts[i].z);
	}
	glEnd();
}


void DrObject::tile(float uMul, float vMul)
{
	for (int i=0;i<numfaces;i++)
	{
		faces[i].ax*=uMul;
		faces[i].ay*=vMul;
		faces[i].bx*=uMul;
		faces[i].by*=vMul;
		faces[i].cx*=uMul;
		faces[i].cy*=vMul;
	}
	toList();
}

void DrObject::wobble1(float time, float div, float amount)
{
	float tm=time*0.001;
	float mul=1.0/div;
	for (int i=0;i<numpoints;i++)
	{
		float koht=verts[i].x+verts[i].y-verts[i].z;
		float s=sin(koht*mul+tm)*amount;
		rverts[i].x+=s*rnormv[i].x;
		rverts[i].y+=s*rnormv[i].y;
		rverts[i].z+=s*rnormv[i].z;
	}
}

void DrObject::wobble2(float time, float div, float amount)
{
	int i;
	float koht;
	float tm1=time*0.00095;
	float mul1=1.03/div;
	float tm2=time*0.0013;
	float mul2=1.12/div;
	float tm3=time*0.0019;
	float mul3=0.95/div;
	for (i=0;i<numpoints;i++)
	{
		koht=rverts[i].x* 0.13
			+rverts[i].y* 0.15
			+rverts[i].z*-0.16
			+rnormv[i].x* 4.3;
		rverts[i].x+=(sin(tm1+koht*mul1)*rnormv[i].x)*amount;

		koht=rverts[i].x* 0.14
			+rverts[i].y*-0.10
			+rverts[i].z*-0.18
			+rnormv[i].y* 4.1;
		rverts[i].y+=(sin(tm2+koht*mul2)*rnormv[i].y)*amount;

		koht=rverts[i].x*-0.152
			+rverts[i].y* 0.122
			+rverts[i].z* 0.11
			+rnormv[i].z* 4.7;
		rverts[i].z+=(sin(tm3+koht*mul3)*rnormv[i].z)*amount;
	}
}

void DrObject::wobble3(float time, float div, float amount)
{
	float tm=time*0.001;
	float mul=1.0/div;
	for (int i=0;i<numpoints;i++)
	{
		float koht=verts[i].x-verts[i].y+verts[i].z;
		float s=sin(tm+koht*mul)*amount;
		rverts[i].x+=s*rnormv[i].x;
		rverts[i].y+=s*rnormv[i].y;
		rverts[i].z+=s*rnormv[i].z;
	}
}

void DrObject::wobble4(float time, float div, float amount)
{
	float tm1=-time*0.0007;
	float tm2= time*0.0010;
	float tm3= time*0.0015;
	float mul1= 0.8/div;
	float mul2=-1.0/div;
	float mul3= 1.3/div;
	for (int i=0;i<numpoints;i++)
	{
		float koht=verts[i].x-verts[i].y+verts[i].z;
		float s1=( sin( tm1+koht*mul1)+1);
		float s2=( cos( tm2-koht*mul2)+1);
		float s3=(-sin(-tm3+koht*mul3)+1);
		rverts[i].x+=s1*s1*s1*rnormv[i].x*amount;
		rverts[i].y+=s2*s2*s2*rnormv[i].y*amount;
		rverts[i].z+=s3*s3*s3*rnormv[i].z*amount;
	}
}

void DrObject::base2roto()
{
	memcpy(rverts,verts,numpoints*sizeof(Dr_Vector));
	memcpy(rnormv,normv,numpoints*sizeof(Dr_Vector));
	memcpy(rnormf,normf,numfaces*sizeof(Dr_Vector));
}

void DrObject::flipNormals()
{
	flip_normals=!flip_normals;
}

void DrObject::calc_rnormals()
{
	float ax,bx,ay,by,az,bz;
	float nx, ny, nz, nl;
	int q,n,l;

	for(q=0;q<numfaces;q++) 
	{
		ax =   (rverts[faces[q].b].x - rverts[faces[q].a].x);
		ay =   (rverts[faces[q].b].y - rverts[faces[q].a].y);
		az =   (rverts[faces[q].b].z - rverts[faces[q].a].z);
		bx =   (rverts[faces[q].c].x - rverts[faces[q].a].x);
		by =   (rverts[faces[q].c].y - rverts[faces[q].a].y);
		bz =   (rverts[faces[q].c].z - rverts[faces[q].a].z);

		nx = ay * bz - az * by; 
		ny = az * bx - ax * bz; 
		nz = ax * by - ay * bx;

		nl = sqrt(nx*nx + ny*ny + nz*nz);

		if(nl!=0) 
		{ 
			nl=1.0/nl;
			rnormf[q].x = nx*nl;
			rnormf[q].y = ny*nl;
			rnormf[q].z = nz*nl;     
		}
		else {
			rnormf[q].x = 0;
			rnormf[q].y = 0;
			rnormf[q].z = 1;     
		}
	}

	for(l=0;l<numpoints;l++) 
	{
		nx=0; ny=0; nz=0;
		for(q=0;q<numfaces;q++) 
		{
			if ( (faces[q].a == l) || (faces[q].b == l) || (faces[q].c == l) ) 
			{
				nx+=rnormf[q].x;
				ny+=rnormf[q].y;
				nz+=rnormf[q].z;
			}
		}

		nl = sqrt(nx*nx + ny*ny + nz*nz);

		if(nl!=0) 
		{
			nl=1.0/nl;
			rnormv[l].x = nx*nl;
			rnormv[l].y = ny*nl;
			rnormv[l].z = nz*nl;     
		}
		else 
		{
			rnormv[l].x = 0;
			rnormv[l].y = 0;
			rnormv[l].z = 1;
		}
	}

}

void DrObject::toList_cubeNormals(float weight)
{
	callList = glGenLists(1);
	glNewList(callList, GL_COMPILE);

	Dr_Vector *fn = new Dr_Vector();

	glBegin(GL_TRIANGLES);
	for (int i=0;i<numfaces;i++)
	{
		int t1=faces[i].a;
		int t2=faces[i].b;
		int t3=faces[i].c;

		Dr_Vector *fnorm = &normf[i];

		glTexCoord2f(faces[i].ax+tx_scrollX, faces[i].ay+tx_scrollY); 
		join_normals(fnorm,&rnormv[t1],weight,fn);
		glNormal3f(fn->x,fn->y,fn->z);
		glVertex3f(rverts[t1].x, rverts[t1].y, rverts[t1].z);


		glTexCoord2f(faces[i].bx+tx_scrollX, faces[i].by+tx_scrollY); 
		join_normals(fnorm,&rnormv[t2],weight,fn);
		glNormal3f(fn->x,fn->y,fn->z);
		glVertex3f(rverts[t2].x, rverts[t2].y, rverts[t2].z);

		glTexCoord2f(faces[i].cx+tx_scrollX, faces[i].cy+tx_scrollY); 
		join_normals(fnorm,&rnormv[t3],weight,fn);
		glNormal3f(fn->x,fn->y,fn->z);
		glVertex3f(rverts[t3].x, rverts[t3].y, rverts[t3].z);
		
	}
	glEnd();

	glEndList();

}

void DrObject::join_normals(Dr_Vector *a, Dr_Vector *b, float weight, Dr_Vector *dest)
{
	dest->x=a->x*(1.0-weight)-b->x*weight;
	dest->y=a->y*(1.0-weight)-b->y*weight;
	dest->z=a->z*(1.0-weight)-b->z*weight;

	float nl = sqrt(dest->x*dest->x + dest->y*dest->y + dest->z*dest->z);

	if(nl!=0) 
	{
		nl=1/nl;
		dest->x = dest->x*nl;
		dest->y = dest->y*nl;
		dest->z = dest->z*nl;
	}
	else 
	{
		dest->x=0;
		dest->y=0;
		dest->z=1;
	}
}
