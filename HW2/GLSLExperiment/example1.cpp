// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#define X_LEFT		1
#define X_RIGHT		2
#define Y_UP		3
#define Y_DOWN		4
#define Z_FRONT		5
#define Z_BACK		6
#define Y_ROTATION  11

//----------------------------------------------------------------------------
int width = 512;
int height = 512;

// remember to prototype
void readVertexAndFaceFromFile(int);
void generateGeometry( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void drawFile();
void displayFileInScreen( void );
void move( void );
void moveCtrl( void );
void variableReset( void );
void yRotationPointTrans( void );
void drawNormalVectors( void );

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef struct 			
{
	int vertex1;		//X point index
	int vertex2;		//Y point index
	int vertex3;		//Z point index
} face3;

// handle to program
GLuint program;


using namespace std;

static char fileName[43][20] = {
						 "airplane.ply", 
						 "mug.ply", 
						 "weathervane.ply", 
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
						 "big_spider.ply", 
						 "canstick.ply", 
						 "big_dodge.ply", 
						 "big_porsche.ply",
						 "ant.ply", 
						 "apple.ply", 
						 "balance.ply",
						 "beethoven.ply",
						 "big_atc.ply"
						};


point4 points[10000];

point4 pointsBuf[60000];
color4 colorsBuf[60000] = {color4( 0.0, 1.0, 0.0, 1.0 )};
color4 colors[36];

face3    face[20000];
point4    normalOfFace[20000];
point4    normalVecter[40000];
point4 normalVecterColorsBuf[40000];
static int countOfVertex;
static long countOfFace; 

static float xMax, xMin, yMax, yMin, zMax, zMin;
static int fileIndex = 0;
static float xMove = 0;
static float yMove = 0;
static float zMove = 0;
static int direction = 0;
float step = 0;
int stopFlag = 0;
static int isYRotation = 0;
static float yRotate = 0.0;
static int enableNormalVecter = 0;

void readVertexAndFaceFromFile(int fileIndex)
{
	char line[256];
	FILE *inStream;
	int lineNum = 0;
	float x,y,z;
	int vertex1, vertex2, vertex3;

	xMax = yMax = zMax = -999999;
	xMin = yMin = zMin = FLT_MAX;

	if((inStream = fopen(fileName[fileIndex], "rt")) == NULL) // Open The File
	{
		printf("File does not exist!");
		exit(0);
	}

	while(!feof(inStream))
	{
		//Just go through file header
		memset(line, 0, 256);
		fscanf(inStream, "%s",line);
		if(strcmp(line, "vertex") == 0)
		{
			fscanf(inStream, "%d",&countOfVertex);
		}
		else if(strcmp(line, "face") == 0)
		{
			fscanf(inStream, "%ld",&countOfFace);

		}
		else if(strcmp(line, "end_header") == 0)
		{
			break;
		}
		else
		{
			continue;
		}

	}
	
	for(int j = 0; j < countOfVertex; j++)
	{	//read each vertex
		
		fscanf(inStream,"%f %f %f", &x, &y, &z);
		if(j == 0)
		{
			xMax = xMin = x;
			yMax = yMin = y;
			zMax = zMin = z;
		}
		points[j] =  point4( x, y, z, 1.0 );
		//printf("%f %f %f\n", x,y,z);
		if(xMax < x) xMax = x;
		if(yMax < y) yMax = y;
		if(zMax < z) zMax = z;
		if(xMin > x) xMin = x;
		if(yMin > y) yMin = y;
		if(zMin > z) zMin = z;
	}
	/*
	for(int j = 0; j < countOfVertex; j++)
	{
		printf("%d: %f %f %f\n", j, points[j].x,points[j].y,points[j].z);
	}
	*/
	for(int j = 0; j < countOfFace; j++)
	{	//read each vertex
		fscanf(inStream,"%d %d %d %d ", &lineNum, &vertex1, &vertex2, &vertex3);
		//printf("%d %d %d\n", vertex1,vertex2,vertex3);
		face[j].vertex1 =  vertex1;
		face[j].vertex2 =  vertex2;
		face[j].vertex3 =  vertex3;
	}

	fclose(inStream);
}

// generate 12 triangles: 36 vertices and 36 colors

void generateGeometry( void )
{	
    //colorcube();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
    //glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    //glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );


	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );
     // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsBuf)) );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 0.0 ); // white background
}
void drawFile()
{
	float x1, y1, z1, x2, y2, z2;
	int tmp;
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	for(int i = 0, j = 0; i < countOfFace*3; i=i+3)
	{
		pointsBuf[i] = points[face[i/3].vertex1];
		pointsBuf[i+1] = points[face[i/3].vertex2];
		pointsBuf[i+2] = points[face[i/3].vertex3];
		colorsBuf[i] = color4( 0.0, 1.0, 0.0, 1.0 );
		colorsBuf[i+1] = color4( 0.0, 1.0, 0.0, 1.0 );
		colorsBuf[i+2] = color4( 0.0, 1.0, 0.0, 1.0 );

		x1 = pointsBuf[i].x - pointsBuf[i+1].x;
		y1 = pointsBuf[i].y - pointsBuf[i+1].y;
		z1 = pointsBuf[i].z - pointsBuf[i+1].z;
		x2 = pointsBuf[i].x - pointsBuf[i+2].x;
		y2 = pointsBuf[i].y - pointsBuf[i+2].y;
		z2 = pointsBuf[i].z - pointsBuf[i+2].z;

		//Normal Vectors of each face
		normalOfFace[i/3] = point4((y1*z2)-(z1*y2), (z1*x2)-(x1*z2), (x1*y2)-(y1*x2), 1.0);
		normalVecter[j] = point4(	(pointsBuf[i].x+pointsBuf[i+1].x+pointsBuf[i+2].x)/3, 
									(pointsBuf[i].y+pointsBuf[i+1].y+pointsBuf[i+2].y)/3, 
									(pointsBuf[i].z+pointsBuf[i+1].z+pointsBuf[i+2].z)/3, 1.0);
		normalVecterColorsBuf[j] = color4( 1.0, 1.0, 1.0, 1.0 );
		tmp = j;
		j++;
		normalVecter[j] = point4(	normalVecter[tmp].x+normalOfFace[i/3].x,
									normalVecter[tmp].y+normalOfFace[i/3].y,
									normalVecter[tmp].z+normalOfFace[i/3].z, 1.0);
		normalVecterColorsBuf[j] = color4( 1.0, 1.0, 1.0, 1.0 );
		j++;
		
	}
	//for 'R' event
	if(isYRotation == 1)
	{
		yRotationPointTrans();
	}
	//glBufferData( GL_ARRAY_BUFFER, sizeof(pointsBuf), pointsBuf, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, sizeof(pointsBuf) + sizeof(colorsBuf), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(pointsBuf), pointsBuf );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(pointsBuf), sizeof(colorsBuf), colorsBuf );

	glEnable( GL_DEPTH_TEST );
	
    glDrawArrays( GL_TRIANGLES, 0, countOfFace*3 );
	glDisable( GL_DEPTH_TEST ); 
	if(enableNormalVecter == 1)
	{
		drawNormalVectors( );
	}

	glFlush(); // force output to graphics hardware

	// use this call to double buffer
	glutSwapBuffers();

	
}
void drawNormalVectors( void )
{
	glBufferData( GL_ARRAY_BUFFER, sizeof(normalVecter), normalVecter, GL_STATIC_DRAW );

	//glBufferData( GL_ARRAY_BUFFER, sizeof(normalVecter) + sizeof(normalVecterColorsBuf), NULL, GL_STATIC_DRAW );
    //glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(normalVecter), normalVecter );
    //glBufferSubData( GL_ARRAY_BUFFER, sizeof(normalVecter), sizeof(normalVecterColorsBuf), normalVecterColorsBuf );
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_LINES, 0, countOfFace*2 ); 
	glDisable( GL_DEPTH_TEST ); 
	//glFlush();
	//glutSwapBuffers();
}

void displayFileInScreen( void )
{
	//printf("fileIndex = %d \t",fileIndex);
	if(fileIndex < 0)
	{
		fileIndex = 42;
	}
	if(fileIndex > 42)
	{
		fileIndex = 0;
	}
	//printf("%d \n",fileIndex);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
	readVertexAndFaceFromFile(fileIndex);

	//printf("Go through file!\n");
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

	/*
	printf("vex = %d\t face = %d\n", countOfVertex, countOfFace);
	printf("xMax = %f\n", xMax);
	printf("xMin = %f\n", xMin);
	printf("yMax = %f\n", yMax);
	printf("yMin = %f\n", yMin);
	printf("zMax = %f\n", zMax);
	printf("zMin = %f\n\n", zMin);
	*/
	modelMat = modelMat * Angel::Translate(-(xMax+xMin)/2 + xMove, -(yMax+yMin)/2 + yMove, -sqrt(pow(xMax-xMin,2)+pow(yMax-yMin,2)+pow(zMax-zMin,2)) + zMove) * Angel::RotateY(yRotate) * Angel::RotateX(0.0f);
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

    drawFile();
}
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

	readVertexAndFaceFromFile(fileIndex);
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0);

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
	/*
	printf("xMax = %f\n", xMax);
	printf("xMin = %f\n", xMin);
	printf("yMax = %f\n", yMax);
	printf("yMin = %f\n", yMin);
	printf("zMax = %f\n", zMax);
	printf("zMin = %f\n", zMin);
	*/
	modelMat = modelMat * Angel::Translate(-(xMax+xMin)/2, -(yMax+yMin)/2, -sqrt(pow(xMax-xMin,2)+pow(yMax-yMin,2)+pow(zMax-zMin,2))) * Angel::RotateY(20.0f) * Angel::RotateX(0.0f);
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

	drawFile();
	// you can implement your own buffers with textures
}
void move( void )
{
	//printf("direction = %d zMove = %f\n",direction, zMove);

	switch(direction)
	{
		case X_LEFT:
			step = (xMax-xMin)/50;
			xMove -= step;
			displayFileInScreen();
			glutPostRedisplay();
			break;

		case X_RIGHT:
			step = (xMax-xMin)/50;
			xMove += step;
			displayFileInScreen();
			glutPostRedisplay();
			break;

		case Y_UP:
			step = (yMax-yMin)/50;
			yMove += step;
			displayFileInScreen();
			glutPostRedisplay();
			break;

		case Y_DOWN:
			step = (yMax-yMin)/50;
			yMove -= step;
			displayFileInScreen();
			glutPostRedisplay();
			break;

		case Z_FRONT:
			step = (zMax-zMin)/50;
			zMove += step;
			displayFileInScreen();
			glutPostRedisplay();
			break;

		case Z_BACK:
			step = (zMax-zMin)/50;
			zMove -= step;
			displayFileInScreen();
			glutPostRedisplay();
			break;
		case Y_ROTATION:
			if(fileIndex % 2 == 0)
			{
				yRotate += 5;
			}
			else
			{
				yRotate -= 5;
			}
			if(yRotate >= 360 || yRotate <= -360)
			{
				yRotate = 0;
				fileIndex++;
			}
			if(fileIndex >= 43)
			{
				fileIndex = 0;
				direction = 0;
			}
			displayFileInScreen();
			glutPostRedisplay();
			break;
		default:
			break;
	}
}
void moveCtrl( void )
{
	stopFlag = !stopFlag;
	if(stopFlag)
	{
		move();
	}
	else
	{
		direction = 0;
		displayFileInScreen();
	}
}
void yRotationPointTrans( void )
{
	float xOffset = (xMax+xMin)/2;
	float zOffset = (zMax+zMin)/2;
	xMove = (xMax+xMin)/2;
	for(int i = 0; i < countOfFace*3; i++)
	{
		pointsBuf[i].x = pointsBuf[i].x - xOffset;
		pointsBuf[i].z = pointsBuf[i].z - zOffset;
	}
}
void variableReset( void )
{
	xMove = 0;
	yMove = 0;
	zMove = 0;
	direction = 0;
	step = 0;
	stopFlag = 0;
	yRotate = 0.0;
	isYRotation = 0;
}
//----------------------------------------------------------------------------
// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) 
	{
		case 'W':
			variableReset();
			displayFileInScreen();
			break;
		case 'N':
			variableReset();
			fileIndex++;
			displayFileInScreen();
			break;
		case 'P':
			variableReset();
			fileIndex--;
			displayFileInScreen();
			break;
		case 'x':
			if(isYRotation == 0)
			{
				direction = X_LEFT;
				moveCtrl();
			}
			break;
		case 'X':
			if(isYRotation == 0)
			{
				direction = X_RIGHT;
				moveCtrl();
			}
			break;
		case 'y':
			if(isYRotation == 0)
			{
				direction = Y_UP;
				moveCtrl();
			}
			break;
		case 'Y':
			if(isYRotation == 0)
			{
				direction = Y_DOWN;
				moveCtrl();
			}
			break;
		case 'z':
			if(isYRotation == 0)
			{
				direction = Z_FRONT;
				moveCtrl();
			}
			break;
		case 'Z':
			if(isYRotation == 0)
			{
				direction = Z_BACK;
				moveCtrl();
			}
			break;

		case 'R':
			isYRotation = 1;
			direction = Y_ROTATION;
			moveCtrl();
			break;
		case 'm':
			enableNormalVecter = !enableNormalVecter;
			displayFileInScreen();
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

    glutInitContextVersion( 3, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Color Cube - Hao Zhou" );

	// init glew
    glewInit();

    generateGeometry();

	// assign handlers
    glutDisplayFunc( displayFileInScreen );
    glutKeyboardFunc( keyboard );
	glutIdleFunc(move);
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
