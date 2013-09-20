// Starter program for HW 0. 
// Program draws a triangle. Study the program first
// Then modify the function generateGeometry to draw a two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

// Angel.h is homegrown include file that also includes glew and freeglut

#include "Angel.h"
#include "textfile.h"


// Number of points in polyline
int NumPoints = 3;

//----------------------------------------------------------------------------

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void drawPolylineFile(char *);

typedef vec2 point2;

using namespace std;

// Array for polyline
point2 points[3];
point2 points2[3];
GLuint program;
GLuint ProjLoc;

void generateGeometry( void )
{
	// ***************** Important note ***************** //
	// please refer to OpenGL documentation before coding
	// many old functions have been depricated
	// though they will probably still work on most machines others
	// will require compatiability mode to be used

	// Specifiy the vertices for a triangle
	// for 3d points use vec3 and change your vPosition attribute appropriately

	points[0] = point2( -0.0, -0.5 );
	points[1] = point2( 0.0, 0.5 );
	points[2] = point2( 0.5, -0.5 );

	points2[0] = point2( -0.5, 0.5 );
	points2[1] = point2( 0.0, 0.5 );
	points2[2] = point2( 0.0, -0.0 );
}


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
	//glBufferData( GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW );


}
void setWindow(GLfloat left,GLfloat right,GLfloat bottom,GLfloat top)
{
	
	ProjLoc = glGetUniformLocation( program, "Proj" );
	mat4 ortho = Ortho2D( left, right, bottom, top );
    glUniformMatrix4fv( ProjLoc, 1, GL_FALSE, ortho );
}

void setViewport(GLint x, GLint y, GLint width, GLint ehight)
{
	glViewport(x,y,width,ehight);
}

void drawPolylineFile(char *FileName)
{
	char line[256];
	float left, right, bottom, top;//char *fileName = "dino.txt";
	FILE *inStream;
	if((inStream = fopen(FileName, "rt")) == NULL) // Open The File
	{
		printf("no such file exist!");
		exit(0);
	}



	if(strcmp(FileName,"dino.dat")==0){
	left = 0;
	right = 640;
	bottom = 0;
	top = 480;
	}


	else {
		while(!feof(inStream))
		 {
			 memset(line, 0, 256);
			  fscanf(inStream, "%s", line);
				if(line[0] == '*')
					 break;
				 else
					 continue;
		  }
		
   
    // read in the four directions of extents
		 fscanf(inStream, "%f %f %f %f\n", &left, &top, &right, &bottom);
	//glClear(GL_COLOR_BUFFER_BIT);// Clear the screen
	 }
	GLint numpolys, numLines; 
	GLfloat	x, y;

	fscanf(inStream,"%d", &numpolys);			// read the number of polylines
	for(int j = 0; j < numpolys; j++)
	{	//read each polyline
		
		fscanf(inStream,"%d", &numLines);
		

	
		for (int i = 0; i < numLines; i++)
		{
			fscanf(inStream,"%f %f", &x, &y);
			points[i] = point2( x , y);
			
			
			
		}
		NumPoints = numLines;
		initGPUBuffers( );
		shaderSetup( );
		setWindow(left, right, bottom,top);
		glDrawArrays( GL_LINE_STRIP, 0, NumPoints ); 

		
		
	}
	glFlush();
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
	int i = 0;
	// TIP 1: remember to enable depth buffering when drawing in 3d

	// TIP 2: If you want to be sure your math functions are right, 
	// prototype your operations in Matlab or Mathematica first to verify correct behavior
	// both programs support some sort of runtime interface to C++ programs

	// TIP3: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	

	// TIP4: avoid using glTranslatex, glRotatex, push and pop
	// pass your own view matrix to the shader directly
	// refer to the latest OpenGL documentation for implementation details

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	////////////////////////////////////////////////////////////////
	// Begin from here
	////////////////////////////////////////////////////////////////


	points[0] = point2( -0.0, -0.5 );
	points[1] = point2( 0.0, 0.5 );
	points[2] = point2( 0.5, -0.5 );

	points2[0] = point2( -0.5, 0.5 );
	points2[1] = point2( 0.0, 0.5 );
	points2[2] = point2( 0.0, -0.0 );

	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	glDrawArrays( GL_LINE_LOOP, 0, NumPoints );    // draw the points, options: GL_LINE_STRIP, GL_POLYGON, GL_LINE_LOOP

	glBufferData( GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW );
	glDrawArrays( GL_POLYGON, 0, NumPoints );    // draw the points, options: GL_LINE_STRIP, GL_POLYGON, GL_LINE_LOOP
   
	////////////////////////////////////////////////////////////////
	// End of buffer dealing
	////////////////////////////////////////////////////////////////
    glFlush(); // force output to graphics hardware

}
//----------------------------------------------------------------------------
// file reader
void reader( void )
{
	char *content;
	content = textFileRead("dino.dat");
	printf("%s",content);
}


//----------------------------------------------------------------------------
// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 'p':
		reader();
		break;
	case 't':
		drawPolylineFile( "dino.dat");
		break;
	case 'e':
		printf("hello");
		break;
	case 'm':
		printf("hello");
		break;
	case 'd':
		printf("hello");
		break;
	case 'g':
		printf("hello");
		break;
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------
// mouse handler
void myMouse(int button, int state, int x, int y)
{
	static point2 corner[2];
	static int numCorners = 0; // initial value is 0
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		corner[numCorners].x = x;
		printf("x = %d\n",x);
		corner[numCorners].y = 480 - y; //flip y coord
		printf("y = %d\n",corner[numCorners].y);
		numCorners++;
	}
	if(numCorners == 2)
	{
		// draw rectangle or do whatever you planned to do
		point2 points[4] = {(corner[0].x, corner[0].y),
		(corner[1].x, corner[0].y),
		(corner[1].x, corner[1].y),
		(corner[0].x, corner[1].y)};
		glDrawArrays(GL_QUADS, 0, 4); 
		numCorners = 0;
	}
	else 
		if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
			glClear(GL_COLOR_BUFFER_BIT); // clear the window
	glFlush( );

	/*
	switch ( button ) 
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_UP) printf("hello ");
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN) printf("world!");
		break;
    case GLUT_MIDDLE_BUTTON:
        exit( EXIT_SUCCESS );
        break;
    }
	*/
}

//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );
    glutInitWindowSize( 640, 480 );

	//glutInitPosition(100,150);
    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    //glutInitContextVersion( 3, 3 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );

	// create GLUT window for drawing
    glutCreateWindow( "Starting App Of Hao Zhou" );

	// init glew
    glewInit();

    generateGeometry( );
    initGPUBuffers( );
    shaderSetup( );

	// assign handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutMouseFunc(myMouse);
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
