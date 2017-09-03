enum{X_AXIS, Y_AXIS, Z_AXIS};

struct point{
	float x;
	float y;
	float z;
};

struct face{
	int vtx[3];
};

struct model{
	point	*obj_points;
	face	*obj_faces;
	int		vertices;
	int		faces;
};
//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void modelPlanet(int i);

void Keyboard(unsigned char key, int x, int y);

void KeyboardSpecial(int key, int x, int y);

void ReadFile(model*);

void createStars();