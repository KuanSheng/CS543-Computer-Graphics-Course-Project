// Starter program for HW 0. 
// Program draws a triangle. Study the program first
// Then modify the function generateGeometry to draw a two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

// Angel.h is homegrown include file that also includes glew and freeglut

#include <stdlib.h>
#include <time.h>
#include "Angel.h"

//----------------------------------------------------------------------------

// remember to prototype
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void tEventHandler( void );
void drawPolylineFile(char *);
void setWindow(GLfloat ,GLfloat ,GLfloat ,GLfloat );
void drawPolylineFile(char *);
void drawMode(int , int , int , int);
void myMouse(int , int , int , int );
void mouseMotion(int, int, GLint  , GLint  );
void deleteHandler( int , int , GLint  , GLint);
void gingerbreadMan( void );

typedef vec2 point2;

using namespace std;

// Number of points in polyline
int NumPoints = 3;
static int isBKeyPressed = 0;
static int hasPrepoint = 0;
static int polylineIndex = 0;
static int pointIndex = 0;
// Array for polyline
point2 points[3000];
point2 points2[3]; // for test use
static point2 pointsForDrawMode[2];
static point2 pointsForPolyline[100][100];


static int countOfPointsForPolyline[100] = { 0 };
static int nearestPointIndex = 0; // for m event

static int nearestPolylineIndex = 0; // for d event, store polyline index
static int nearestPolylinePointIndex = 0; // for d event, store point index in above polyline

//initialize for gingerbread man 
static point2 startPoint = point2(115, 121);
const int M = 40;
const int L = 3;
const int iterationTimes = 100;

GLuint program;
GLuint ProjLoc;
static char fileName[10][20] = {
						 "dino.dat", 
						 "birdhead.dat", 
						 "dragon.dat", 
						 "house.dat", 
						 "knight.dat", 
						 "rex.dat", 
						 "scene.dat", 
						 "usa.dat", 
						 "vinci.dat",
						 "vinci.dat"
						};

void initGPUBuffers( void )
{
	// Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
}

void setWindow(GLfloat left,GLfloat right,GLfloat bottom,GLfloat top)
{
	
	ProjLoc = glGetUniformLocation( program, "Proj" );
	mat4 ortho = Ortho2D( left, right, bottom, top );
    glUniformMatrix4fv( ProjLoc, 1, GL_FALSE, ortho );
}

void drawPolylineFile(char *FileName)
{
	char line[256];
	GLfloat left, right, bottom, top;
	FILE *inStream;
	GLint countOfPolylines, countOfPointsForPolyline; 
	GLfloat	x, y;

	if((inStream = fopen(FileName, "rt")) == NULL) // Open The File
	{
		printf("File does not exist!");
		exit(0);
	}

	//Deal with dino.dat file
	if(strcmp(FileName,"dino.dat")==0 || strcmp(FileName,"hello.dat")==0)
	{
		left = 0;
		right = 640;
		bottom = 0;
		top = 480;
	}
	else 
	{
		while(!feof(inStream))
		{
			//Just go through comments
			memset(line, 0, 256);
			fscanf(inStream, "%s", line);
			if(line[0] == '*')
				break;
			else
				continue;
		}
		// read in the four directions of extents
		fscanf(inStream, "%f %f %f %f\n", &left, &top, &right, &bottom);
	 }
	
	// read point count for each polyline
	fscanf(inStream,"%d", &countOfPolylines);			
	for(int j = 0; j < countOfPolylines; j++)
	{	//read each polyline
		fscanf(inStream,"%d", &countOfPointsForPolyline);
		for (int i = 0; i < countOfPointsForPolyline; i++)
		{
			fscanf(inStream,"%f %f", &x, &y);
			points[i] = point2( x , y);
		}
		setWindow(left, right, bottom,top);
		glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
		glDrawArrays( GL_LINE_STRIP, 0, countOfPointsForPolyline ); 
		glFlush();
	}
	fclose(inStream);
}

void shaderSetup( void )
{
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
	
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background

}

//----------------------------------------------------------------------------
// this is where the drawing should happen


void display( void )
{
	/*
	 TIP 1: remember to enable depth buffering when drawing in 3d

	 TIP 2: If you want to be sure your math functions are right, 
	 prototype your operations in Matlab or Mathematica first to verify correct behavior
	 both programs support some sort of runtime interface to C++ programs

	 TIP3: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	

	 TIP4: avoid using glTranslatex, glRotatex, push and pop
	 pass your own view matrix to the shader directly
	 refer to the latest OpenGL documentation for implementation details
	*/

    //glClear( GL_COLOR_BUFFER_BIT );     // clear the window, Comment it, otherwise, when left click, there will be obvious page refresh
	////////////////////////////////////////////////////////////////
	// Begin from here
	////////////////////////////////////////////////////////////////
	int thumbIndex = 0;

	for(thumbIndex = 0; thumbIndex < 10; thumbIndex++)
	{
		glViewport(64*thumbIndex,432,64 ,48);
		drawPolylineFile(fileName[thumbIndex]);
	}
	glViewport(0, 0 , 640 , 480);
	drawPolylineFile("hello.dat");
	////////////////////////////////////////////////////////////////
	// End of buffer dealing
	////////////////////////////////////////////////////////////////
	
}
//----------------------------------------------------------------------------
// keyboard t event handler
void tEventHandler( void )
{
	int x = 0;
	int y = 0;
	int randNum;
	srand(time(NULL));
	for(y = 0; y < 6; y++)
	{
		for(x = 0; x < 6; x++)
		{
			randNum = rand()%10;
			glViewport(106*(5-x)+20,364-(62*y),64 ,48);
			drawPolylineFile(fileName[randNum]);
		}
	}
}
//----------------------------------------------------------------------------
// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	int randNum;
	isBKeyPressed = 0; // Just in case
    switch ( key ) 
	{
		case 'p':
			srand(time(NULL));
			//generate a random number to select a random file
			randNum = rand()%10;
			glClear( GL_COLOR_BUFFER_BIT );
			display();
			glViewport(32, 0 , 576 , 432);
			drawPolylineFile(fileName[randNum]);
		
			break;
		case 't':
			//glViewport(32, 0 , 576 , 432);
			//drawPolylineFile("dino.dat");
			glClear( GL_COLOR_BUFFER_BIT );
			display();
			tEventHandler();
			break;
		case 'e':
			glClear(GL_COLOR_BUFFER_BIT); // clear the window
			display();
			polylineIndex = 0;
			isBKeyPressed = 0;
			hasPrepoint = 0;
			// Begin of drawing mode
			glutMouseFunc(drawMode); //After this, you must reset: glutMouseFunc(myMouse);
			// End of drawing mode
			break;
  		case 'm':
			glutMouseFunc(mouseMotion);
			//glutMotionFunc(mouseMotion);//Take care of the mouse location

			break;
		case 'd':
			glutMouseFunc(deleteHandler);
			break;
		case 'g':
			glClear(GL_COLOR_BUFFER_BIT); // clear the window
			gingerbreadMan();
			break;
		case 'b':
			//a issue in E keyboard
			isBKeyPressed = 1;
			glutMouseFunc(drawMode);
			break;
		case 033:
			exit( EXIT_SUCCESS );
			break;
		default:
			isBKeyPressed = 0;
    }
}
//----------------------------------------------------------------------------
// gingerbreadMan handler
void gingerbreadMan( void )
{
	point2 p = startPoint;
	for(int j = 0; j < iterationTimes; j++)
	{
		for(int i = 0; i < 3000; i++)
		{
			points[i].x = M*(1+2*L)-p.y+abs(p.x-L*M);
			points[i].y = p.x;
		
			p = points[i];
		}
		glViewport(0, 0 , 640 , 480);
		setWindow(0, 640, 0, 480);
		glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
		glDrawArrays( GL_POINTS, 0, 3000 ); 
		glFlush();
	}

}
//----------------------------------------------------------------------------
// delete handler
void deleteHandler( int button, int state, GLint  x, GLint y)
{
	switch ( button ) 
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_UP) 
			{
				
				GLfloat distance = abs(pointsForPolyline[0][0].x-x) + abs(pointsForPolyline[0][0].y-(480-y)); 
				for(int j = 0; j <= polylineIndex; j++)
				{
					for(int i = 0; i < countOfPointsForPolyline[j]; i++)
					{
						//printf("Distance = %f\n",abs(pointsForPolyline[polylineIndex][i].x-x) + abs(pointsForPolyline[polylineIndex][i].y-y));
						GLfloat temp_distance = abs(pointsForPolyline[j][i].x-x) + abs(pointsForPolyline[j][i].y-(480-y));
						if(distance > temp_distance)
						{
							distance = temp_distance;
							nearestPolylineIndex = j; 
							nearestPolylinePointIndex = i;
						}
					}
				}
				if(nearestPolylinePointIndex > 0)
				{
					pointsForPolyline[nearestPolylineIndex][nearestPolylinePointIndex] = pointsForPolyline[nearestPolylineIndex][nearestPolylinePointIndex-1];
				}
				else if(nearestPolylinePointIndex == 0 && countOfPointsForPolyline[nearestPolylineIndex]>1)
				{
					pointsForPolyline[nearestPolylineIndex][nearestPolylinePointIndex] = pointsForPolyline[nearestPolylineIndex][nearestPolylinePointIndex+1];
				}

				//Update the display (clean the screen & re-render it)
				glClear( GL_COLOR_BUFFER_BIT );
				display();
				glViewport(0, 0 , 640 , 480);
				setWindow(0, 640, 0, 480);
				//printf("size = %d\n",sizeof(pointsForDrawMode));
				for(int i =0; i <= polylineIndex; i++)
				{
					glBufferData( GL_ARRAY_BUFFER, sizeof(pointsForPolyline[0]), pointsForPolyline[i], GL_STATIC_DRAW );
					glDrawArrays( GL_LINE_STRIP, 0, countOfPointsForPolyline[i] ); 
					glFlush();
				}
			}
			/*
			if(state == GLUT_UP) 
			{
				pointsForPolyline[polylineIndex][nearestPointIndex].x = x;
				pointsForPolyline[polylineIndex][nearestPointIndex].y = 480 - y;
				//printf("Up point = %d\t%d\n",x,y);

				//Update the display (clean the screen & re-render it)
				glClear( GL_COLOR_BUFFER_BIT );
				display();
				glViewport(0, 0 , 640 , 480);
				setWindow(0, 640, 0, 480);
				//printf("size = %d\n",sizeof(pointsForDrawMode));
				for(int i =0; i <= polylineIndex; i++)
				{
					glBufferData( GL_ARRAY_BUFFER, sizeof(pointsForPolyline[0]), pointsForPolyline[i], GL_STATIC_DRAW );
					glDrawArrays( GL_LINE_STRIP, 0, countOfPointsForPolyline[i] ); 
					glFlush();
				}
			}
			*/
			break;
	}
}
//----------------------------------------------------------------------------
// drawing handler
void mouseMotion( int button, int state, GLint  x, GLint y)
{
	switch ( button ) 
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) 
			{
				
				GLfloat distance = abs(pointsForPolyline[polylineIndex][0].x-x) + abs(pointsForPolyline[polylineIndex][0].y-(480-y)); 
				/*
				for(int i = 0; i < pointIndex; i++)
				{
					printf("pointIndex=%d\t%f\t%f\n",i,pointsForPolyline[polylineIndex][i].x,pointsForPolyline[polylineIndex][i].y);
				}
				*/
				for(int i = 1; i < pointIndex; i++)
				{
					//printf("Distance = %f\n",abs(pointsForPolyline[polylineIndex][i].x-x) + abs(pointsForPolyline[polylineIndex][i].y-y));
					GLfloat temp_distance = abs(pointsForPolyline[polylineIndex][i].x-x) + abs(pointsForPolyline[polylineIndex][i].y-(480-y));
					if(distance > temp_distance)
					{
						distance = temp_distance;
						nearestPointIndex = i;
					}
				}
				//printf("\n\nDown point = %d\t%d\n",x,y);
				//printf("Nearest point index = %d\n", nearestPointIndex);
			}
			if(state == GLUT_UP) 
			{
				pointsForPolyline[polylineIndex][nearestPointIndex].x = x;
				pointsForPolyline[polylineIndex][nearestPointIndex].y = 480 - y;
				//printf("Up point = %d\t%d\n",x,y);

				//Update the display (clean the screen & re-render it)
				glClear( GL_COLOR_BUFFER_BIT );
				display();
				glViewport(0, 0 , 640 , 480);
				setWindow(0, 640, 0, 480);
				//printf("size = %d\n",sizeof(pointsForDrawMode));
				for(int i =0; i <= polylineIndex; i++)
				{
					glBufferData( GL_ARRAY_BUFFER, sizeof(pointsForPolyline[0]), pointsForPolyline[i], GL_STATIC_DRAW );
					glDrawArrays( GL_LINE_STRIP, 0, countOfPointsForPolyline[i] ); 
					glFlush();
				}
			}
			
			
			
			break;
	}
	
	
}
//----------------------------------------------------------------------------
// drawing handler
void drawMode(int button, int state, int x, int y)
{
	switch ( button ) 
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) 
			{

				if(hasPrepoint == 0 || isBKeyPressed == 1)
				{
					//pointsForDrawMode[0] = point2( x , 480 - y); 
					pointsForDrawMode[1] = point2( x , 480 - y); 
					countOfPointsForPolyline[polylineIndex] = pointIndex;
					if(hasPrepoint != 0) 
					{
						polylineIndex++;
					}
					hasPrepoint = 1; 
					pointIndex = 0;
					
					pointsForPolyline[polylineIndex][pointIndex] = point2( x , 480 - y);
					pointIndex++;
				}
				else
				{
					pointsForPolyline[polylineIndex][pointIndex] = point2( x , 480 - y);
					pointIndex++;
					pointsForDrawMode[0] = pointsForDrawMode[1];
					pointsForDrawMode[1] = point2( x , 480 - y); 
					glViewport(0, 0 , 640 , 480);
					setWindow(0, 640, 0, 480);
					glBufferData( GL_ARRAY_BUFFER, sizeof(pointsForDrawMode), pointsForDrawMode, GL_STATIC_DRAW );
					glDrawArrays( GL_LINE_STRIP, 0, 2 ); 
					glFlush();
				}
				isBKeyPressed = 0;
			}
	}
}

//----------------------------------------------------------------------------
// mouse handler
void myMouse(int button, int state, int x, int y)
{
	int index = 0;
	switch ( button ) 
	{
		case GLUT_LEFT_BUTTON:
			// Judge whether the mouse point is clicked on thumb images
			if(state == GLUT_DOWN && y <= 48) 
			{
				index = x/64;
				glClear( GL_COLOR_BUFFER_BIT );
				display();
				glViewport(32, 0 , 576 , 432);
				drawPolylineFile(fileName[index]);
				//printf("x = %d\t",x);
				//printf("y = %d\n",y);
			};
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) glClear(GL_COLOR_BUFFER_BIT); // clear the window
			glFlush( );
			break;
		case GLUT_MIDDLE_BUTTON:
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
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );
    glutInitWindowSize( 640, 480 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    //glutInitContextVersion( 3, 3 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );

	// create GLUT window for drawing
    glutCreateWindow( "Starting App Of Hao Zhou" );

	// init glew
    glewInit();
    initGPUBuffers( );
    shaderSetup( );

	// assign default handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutMouseFunc(myMouse);

    glutMainLoop();
    return 0;
}
