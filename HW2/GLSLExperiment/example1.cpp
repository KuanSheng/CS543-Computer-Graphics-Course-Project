// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#include <math.h>

//----------------------------------------------------------------------------
int width = 0;
int height = 0;
int numvertex;
int numface;
int index = 0;
int order = 0;
int direction;
int flag = 0;
int Rflag = 0;
int mflag = 0;
int nflag = 0;
int pflag = 0;
int eflag = 0;

int number = 0;
float xmax ,xmin ,ymax, ymin ,zmax, zmin;
float movex = 0,movey = 0,movez = 0;
float ROTATE_Y = 0;
float step1 = 0;
float pulsestep=1;

#define x_positive 1
#define x_negative 2
#define y_positive 3
#define y_negative 4
#define z_positive 5
#define z_negative 6
#define Rotate 20
#define normalL 7
#define pulse 8
#define cube 9
//GLuint vColor = glGetAttribLocation( program, "vColor" ); 
// remember to prototype
void generateGeometry( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void quad( int a, int b, int c, int d );
void colorcube(void);
void drawCube(void);
void readPLYfile(char *FileName);



typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// handle to program
GLuint program;

using namespace std;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[50000];
color4 colors[50000];
point4 vertices[50000];
point4 normal[50000];
point4 normalvertice[50000];
point4 normalcolors[50000];
point4 normal2[50000];
point4 cubepoints[30];
char File[43][20]=
	{
	"airplane.ply",
	"ant.ply",
	"apple.ply",
	"balance.ply",
	"beethoven.ply",
	"big_atc.ply",
	"big_dodge.ply",
    "big_porsche.ply",
    "big_spider.ply",
    "canstick.ply",
    "chopper.ply",
    "cow.ply",
    "dolphins.ply",
    "egret.ply",
    "f16.ply",
    "footbones.ply",
    "fracttree.ply",
    "galleon.ply",
    "hammerhead.ply",
    "helix.ply",
    "hind.ply",
    "kerolamp.ply",
    "ketchup.ply",
    "mug.ply",
    "part.ply",
    "pickup_big.ply",
    "pump.ply",
    "pumpa_tb.ply",
    "sandal.ply",
    "saratoga.ply",
    "scissors.ply",
    "shark.ply",
    "steeringweel.ply",
    "stratocaster.ply",
    "street_lamp.ply",
    "teapot.ply",
    "tennis_shoe.ply",
    "tommygun.ply",
    "trashcan.ply",
    "turbine.ply",
    "urn2.ply",
    "walkman.ply",
    "weathervane.ply"};


// Vertices of a unit cube centered at origin, sides aligned with axes
//point4 vertices[8] = {
    //point4( -0.5, -0.5,  0.5, 1.0 ),
    //point4( -0.5,  0.5,  0.5, 1.0 ),
    //point4(  0.5,  0.5,  0.5, 1.0 ),
    //point4(  0.5, -0.5,  0.5, 1.0 ),
    //point4( -0.5, -0.5, -0.5, 1.0 ),
    //point4( -0.5,  0.5, -0.5, 1.0 ),
    //point4(  0.5,  0.5, -0.5, 1.0 ),
    //point4(  0.5, -0.5, -0.5, 1.0 )
//};
// RGBA olors

// quad generates two triangles for each face and assigns colors
//    to the vertices
//void quad( int a, int b, int c, int d )
//{
    //colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    //colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    //colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    //colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    //colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
   // colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
//}

// generate 12 triangles: 36 vertices and 36 colors
//void colorcube()
//{
    //quad( 1, 0, 3, 2 );
    //quad( 2, 3, 7, 6 );
    //quad( 3, 0, 4, 7 );
    //quad( 6, 5, 1, 2 );
    //quad( 4, 5, 6, 7 );
    //quad( 5, 4, 0, 1 );
//}

void generateGeometry( void )
{	
    //colorcube();

    // Create a vertex array object
	//readPLYfile("cow.ply");
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );


	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );
     // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}
void yRotation( void ){
	float xPixel = (xmax+xmin)/2;
	float zPixel = (zmax+zmin)/2;
	int i;
	movex = (xmax + xmin)/2;
	for(i=0;i<index;i++){
		points[i].x = points[i].x - xPixel;
		points[i].z = points[i].z - zPixel;
	}
	for(i=0;i<number;i++){
		normalvertice[i].x = normalvertice[i].x - xPixel;
		normalvertice[i].z = normalvertice[i].z - zPixel;
	}

}

void readPLYfile(char *FileName)
{
	FILE *plyfile;
	char line[256];
	int i=0,j=0;
	//int numvertex;
	//int numface;
	int num, one, two, three;
	float temp[3][3];
	float x,y,z;
	float c = 1.0;

	if((plyfile = fopen(FileName, "rt")) == NULL) // Open The File
	{
		printf("no such file exist!");
		exit(0);
	}

	for(i=0;i<6;i++){
		memset(line, 0, 256);
		fscanf(plyfile, "%s",line);
	}

    fscanf(plyfile,"%d",&numvertex);

	for(i=0;i<11;i++){
		memset(line, 0, 256);
		fscanf(plyfile, "%s",line);
	}

	fscanf(plyfile,"%d",&numface);

	for(i=0;i<6;i++){
		memset(line, 0, 256);
		fscanf(plyfile, "%s",line);
	}

	//for(i=0;i<numvertex;i++){
		//colors[i] = color4( 0.0, 0.0, 1.0, 1.0 );
	//}

	for(i=0;i<numvertex;i++){
		fscanf(plyfile,"%f %f %f", &x, &y, &z);
		vertices[i] = point4(x,y,z,c);
		if(i == 0){
		xmax = x;
		xmin = x;
		ymax = y;
		ymin = y;
		zmax = z;
		zmin = z;
		}
		else{ 
			if(x >= xmax)
				xmax = x;
			else if(x <= xmin)
				xmin = x;

			if(y >= ymax)
				ymax = y;
			else if(y <= ymin)
				ymin = y;

			if(z >= zmax)
				zmax = z;
			else if(z <= zmin)
				zmin = z;

		}
	}
	//printf("%f\t%f\t%f\n",xmax,ymax,zmax);
	index = 0;
	number = 0;
	for(i=0;i<numface;i++){
		fscanf(plyfile,"%d",&num);
		if(num != 3)
			exit(0);
		fscanf(plyfile, "%d %d %d", &one, &two, &three);
		colors[index] = color4(1.0,0.0,0.0,1.0);
		points[index] = vertices[one];
		index++;
		colors[index] = color4(1.0,0.0,0.0,1.0);
		points[index] = vertices[two];
		index++;
		colors[index] = color4(1.0,0.0,0.0,1.0);
		points[index] = vertices[three];
		index++;


		temp[0][0]=points[index-1].x - points[index-2].x;
		temp[0][1]=points[index-1].y - points[index-2].y;
		temp[0][2]=points[index-1].z - points[index-2].z;
		temp[1][0]=points[index-1].x - points[index-3].x;
		temp[1][1]=points[index-1].y - points[index-3].y;
		temp[1][2]=points[index-1].z - points[index-3].z;

        normal[i].x = -(temp[0][1]*temp[1][2]-temp[0][2]*temp[1][1]);
		normal[i].y = (temp[0][0]*temp[1][2]-temp[0][2]*temp[1][0]);
		normal[i].z = temp[0][0]*temp[1][1]-temp[0][1]*temp[1][0];

		float mode = sqrt(normal[i].x*normal[i].x + normal[i].y*normal[i].y + normal[i].z*normal[i].z)*sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin))/sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin));
		 normal[i].x = 10*normal[i].x/mode;
		 normal[i].y = 10*normal[i].y/mode;
		 normal[i].z = 10*normal[i].z/mode;
 
		normalvertice[number] = point4((points[index-1].x+points[index-2].x+points[index-3].x)/3,(points[index-1].y+points[index-2].y+points[index-3].y)/3,(points[index-1].z+points[index-2].z+points[index-3].z)/3,1.0);
		normalcolors[number] = color4(0.0,0.0,1.0,1.0);
		number++;

		normalvertice[number] = point4((points[index-1].x+points[index-2].x+points[index-3].x)/3+normal[i].x,(points[index-1].y+points[index-2].y+points[index-3].y)/3+normal[i].y,
			(points[index-1].z+points[index-2].z+points[index-3].z)/3+normal[i].z,1.0);
		normalcolors[number] = color4(0.0,0.0,1.0,1.0);
		number++;
	}

	int t=0;
	for(i=0;i<numface;i++){
		normal2[t] = normal[i];
		t++;
		normal2[t] = normal[i];
		t++;
		normal2[t] = normal[i];
		t++;
 	}

	if(pflag == 1){
		//float ratio = sqrt(normal2[i].x*normal2[i].x + normal2[i].y*normal2[i].y + normal2[i].z*normal2[i].z)/sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin));
		for(i=0;i<index;i++){
		    float ratio = sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin));
			//points[i].x = points[i].x + 0.01*step1*normal2[i].x/ratio;
			//points[i].y = points[i].y + 0.01*step1*normal2[i].y/ratio;
			//points[i].z = points[i].z + 0.01*step1*normal2[i].z/ratio;

			points[i].x = points[i].x + 0.01*step1*normal2[i].x*ratio/10;
			points[i].y = points[i].y + 0.01*step1*normal2[i].y*ratio/10;
			points[i].z = points[i].z + 0.01*step1*normal2[i].z*ratio/10;
		}
		
	}

	printf("%f\t",step1);
	fclose(plyfile);
}
void readdata( void ){
	readPLYfile(File[order]);
}
void drawfile( void )
{
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

	//readPLYfile("airplane.ply");
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, index );
	glDisable( GL_DEPTH_TEST ); 
}
void drawCube(void)
{
	// change to GL_FILL
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	glDisable( GL_DEPTH_TEST ); 
}
void drawNext(){
	nflag = 0;
	if(order < 42)
	order++;
	else 
		order = 0;
    display();
}

void drawPRE(){
	nflag = 0;
	if(order>0)
		order--;
	else 
		order = 42;
	display();
}

void drawMove( void )
{
	float step;
	float angle = 5.0;
	
	bool stepdirect = true;
	switch(direction){
		case x_positive:
			step = (xmax-xmin)/500;
		    movex = movex + step;
			display();
			glutPostRedisplay();
			break;
        case x_negative:
			step = (xmax-xmin)/500;
		    movex = movex - step;
			display();
			glutPostRedisplay();
			break;
		case y_positive:
			step = (ymax-ymin)/500;
		    movey = movey + step;
			display();
			glutPostRedisplay();
			break;
		case y_negative:
			step = (ymax-ymin)/500;
		    movey = movey - step;
			display();
			glutPostRedisplay();
			break;
		case z_positive:
			step = (zmax-zmin)/50;
		    movez = movez + step;
			display();
			glutPostRedisplay();
			break;
		case z_negative:
			step = (zmax-zmin)/50;
		    movez = movez - step;
			display();
			glutPostRedisplay();
			break;
		case Rotate:
			if(order > 41){
				order = 0;
				Rflag = 0;
			    mflag = 0;
				flag = 0;
			    ROTATE_Y = 0;
				direction = 9;
			}

			if(ROTATE_Y >= 360 || ROTATE_Y <= -360)
			{order++;
			ROTATE_Y = 0;
			}

			if(order % 2 == 0)
		      ROTATE_Y = ROTATE_Y + angle;
	        else
		      ROTATE_Y = ROTATE_Y - angle;

	      display();
	      glutPostRedisplay();
			break;
		case pulse:
			
		   if(step1>30)
		  { pulsestep = -1;
		 
		   }
		   if(step1 <= 1)
			   {pulsestep = 1;

		  }

		   step1 = step1 + pulsestep;

			display();
			glutPostRedisplay();
			break;
	}
}
void drawNormalLine( void ){
   glBufferData( GL_ARRAY_BUFFER, sizeof(normalvertice) + sizeof(normalcolors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(normalvertice), normalvertice );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(normalvertice), sizeof(normalcolors), normalcolors );
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINES,0,number);
	glDisable(GL_DEPTH_TEST);


}
void drawbounding(){
	
		cubepoints[0]=point4(xmax,ymax,zmax,1.0);
		cubepoints[1]=point4(xmax,ymax,zmin,1.0);
		cubepoints[2]=point4(xmin,ymax,zmin,1.0);
		cubepoints[3]=point4(xmin,ymax,zmax,1.0);
		cubepoints[4]=point4(xmax,ymax,zmax,1.0);
		cubepoints[5]=point4(xmax,ymin,zmax,1.0);
		cubepoints[6]=point4(xmin,ymin,zmax,1.0);
		cubepoints[7]=point4(xmin,ymax,zmax,1.0);
		cubepoints[8]=point4(xmin,ymax,zmin,1.0);
		cubepoints[9]=point4(xmin,ymin,zmin,1.0);
		cubepoints[10]=point4(xmin,ymin,zmax,1.0);
		cubepoints[11]=point4(xmax,ymin,zmax,1.0);
		cubepoints[12]=point4(xmax,ymin,zmin,1.0);
		cubepoints[13]=point4(xmin,ymin,zmin,1.0);
		cubepoints[14]=point4(xmin,ymax,zmin,1.0);
		cubepoints[15]=point4(xmax,ymax,zmin,1.0);
		cubepoints[16]=point4(xmax,ymin,zmin,1.0);


	 glBufferData( GL_ARRAY_BUFFER, sizeof(cubepoints),
		  cubepoints, GL_STATIC_DRAW );
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_LINE_STRIP, 0, 17 );
	glDisable( GL_DEPTH_TEST ); 
}

//void drawRotate( void ){
	//float angle = 5.0;
	//if(order % 2 == 0)
		//ROTATE_Y = ROTATE_Y + angle;
	//else
		//ROTATE_Y = ROTATE_Y - angle;
	//display();
	//glutPostRedisplay();
//}
//----------------------------------------------------------------------------
// this is where the drawing should happen
void display( void )
{
	// remember to enable depth buffering when drawing in 3d

	// TIP1: if you get bogged down with many matrix operations
	// or need to perform some complexed fitting or solving
	// it may be time to include some package like diffpac or lapack
	// http://www.netlib.org/lapack/#_lapack_for_windows
	// http://www.diffpack.com/

	// TIP2: if you feel that GLSL is too restrictive OpenCL or CUDA can be
	// used to generate images or other information, they support interface to OpenGL
	// http://www.khronos.org/registry/cl/

	// TIP3: prototype your operations in Matlab or Mathematica first to verify correct behavior
	// both programs support some sort of runtime interface to C++ programs

	// TIP4: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	
	// TIP5: take a look at the "Assembly" generated from the opengl compilers, it might lead you to some optimizations
	// http://http.developer.nvidia.com/Cg/cgc.html

	// avoid using glTranslatex, glRotatex, push and pop
	// pass your own view matrix to the shader directly
	// refer to the latest OpenGL documentation for implementation details

	// PROTIP1: You can access the depth buffer value and screen location at each fragment
	// in the fragment shader, go wild

	// PROTIP2: Render stuff to texture, then run filters on the texture in a second pass to 
	// produce cool effects
	//readPLYfile("cow.ply");
	readdata();
	if(Rflag == 1){
		yRotation();
	}
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	// PROTIP4: Do not set the near and far plane too far appart!
	// depth buffers do not have unlimited resolution
	// surfaces will start to fight as they come nearer to each other
	// if the planes are too far appart (quantization errors :(   )

	// PROTIP5: If you must have extreme distances, use multiple render passes
	// which divide the entire scene into adjacent viewing volumes
	// render far away volumes first
	// also scale your objects appropriatly, dont use scales at the upper or lower bounds
	// of floating point precision

	// WARNING1: I believe Angel::transpose(...) does not transpose a mat4, but just returns
	// an identical matrix, can anyone verify this?
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)60.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 10000.0);

	float viewMatrixf[16];
	viewMatrixf[0] = perspectiveMat[0][0];viewMatrixf[4] = perspectiveMat[0][1];
	viewMatrixf[1] = perspectiveMat[1][0];viewMatrixf[5] = perspectiveMat[1][1];
	viewMatrixf[2] = perspectiveMat[2][0];viewMatrixf[6] = perspectiveMat[2][1];
	viewMatrixf[3] = perspectiveMat[3][0];viewMatrixf[7] = perspectiveMat[3][1];

	viewMatrixf[8] = perspectiveMat[0][2];viewMatrixf[12] = perspectiveMat[0][3];
	viewMatrixf[9] = perspectiveMat[1][2];viewMatrixf[13] = perspectiveMat[1][3];
	viewMatrixf[10] = perspectiveMat[2][2];viewMatrixf[14] = perspectiveMat[2][3];
	viewMatrixf[11] = perspectiveMat[3][2];viewMatrixf[15] = perspectiveMat[3][3];
	
	Angel::mat4 modelMat = Angel::identity();
	//modelMat = modelMat * Angel::Translate(0.0, 0.0, -2.0f) * Angel::RotateY(45.0f) * Angel::RotateX(35.0f);
	modelMat = modelMat * Angel::Translate(-(xmax+xmin)/2+movex, -(ymax+ymin)/2+movey, -sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin))+movez)*Angel::RotateX(0.0f)*Angel::RotateY(ROTATE_Y);
	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];
	
	// set up projection matricies
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( viewMatrix, 1, GL_FALSE, viewMatrixf);
	drawfile();
	if(nflag == 1){
		drawNormalLine();
}
	if(eflag == 1)
	drawbounding();
    glFlush(); // force output to graphics hardware

	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 'W':
		if(mflag == 0){
			glClear(GL_COLOR_BUFFER_BIT);}
		 flag = 0;
         Rflag = 0;
         mflag = 0;
         nflag = 0;
         pflag = 0;
         eflag = 0;
		 order = 0;
		 direction = 9;
		break;
	case 'N':
		if(mflag == 0){
		glClear(GL_COLOR_BUFFER_BIT);
		movex=0;
		movey=0;
		movez=0;
		drawNext();}
		break;
	case 'P':
		if(mflag == 0){
		glClear(GL_COLOR_BUFFER_BIT);
		movex=0;
		movey=0;
		movez=0;
		drawPRE();}
		break;
	case 'X':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = x_positive;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'Y':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = y_positive;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'Z':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = z_positive;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'x':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = x_negative;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'y':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = y_negative;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'z':
		if(flag == 0)
			flag = 1;
		else
			flag = 0;
		if(flag == 1){
        direction = z_negative;
		drawMove();}
		else 
			direction = 9;
		break;
	case 'R':
		Rflag = 1;
		flag = 2;
		mflag = 1;
		direction = 20;
		drawMove();
        break;
	case 'm':
		if(nflag == 0){
			//glClear(GL_COLOR_BUFFER_BIT);
			nflag = 1;
		    display();
		}
		else if(nflag ==1){
			//glClear(GL_COLOR_BUFFER_BIT);
			nflag = 0;
			display();
		}
		break;
	case 'B':
		if(pflag == 0)
		pflag = 1;
		else if(pflag == 1){
			pflag = 0;
		}
			direction = 8;
		drawMove();
		break;
	case 'e':
		if(eflag == 0)
			eflag =1;
		else if(eflag == 1)
			eflag = 0;
		display();
		break;
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
	width = 512;
	height = 512;
    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
	// should run a test here 
	// with different cases
	// this is a good time to find out information about
	// your graphics hardware before you allocate any memory
    glutInitContextVersion( 3, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Color Cube" );

	// init glew
    glewInit();

    generateGeometry();

	// assign handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutIdleFunc(drawMove);
	//glutIdleFunc(drawRotate);
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
