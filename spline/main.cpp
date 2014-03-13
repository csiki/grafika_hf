//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization fÃ¼ggvÃ©nyt kivÃ©ve, a lefoglalt adat korrekt felszabadÃ­tÃ¡sa nÃ©lkÃ¼l 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : <VEZETEKNEV(EK)> <KERESZTNEV(EK)>
// Neptun : <NEPTUN KOD>
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream> // TODO
#include <string> // TODO

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

//--------------------------------------------------------//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
   float x, y, z;

   Vector( ) { 
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) { 
	x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) { 
	return Vector(x * a, y * a, z * a); 
   }
   Vector operator/(float a) { 
	return Vector(x / a, y / a, z / a); 
   }
   Vector operator+(const Vector& v) {
 	return Vector(x + v.x, y + v.y, z + v.z); 
   }
   Vector operator-(const Vector& v) {
 	return Vector(x - v.x, y - v.y, z - v.z); 
   }
   float operator*(const Vector& v) { 	// dot product
	return (x * v.x + y * v.y + z * v.z); 
   }
   Vector operator%(const Vector& v) { 	// cross product
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }
   
   Vector normal()
   {
	   Vector tmp(x / Length(), y / Length(), z / Length());
		return tmp;
   }

   void tostring()
   {
	   std::cout << "x: " << x << " y: " << y;
   }
};
 
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
   float r, g, b;

   Color( ) { 
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) { 
	return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) { 
	return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b); 
   }
};

///////////////////////////////////////////////////////////////////
// CONSTANTS
///////////////////////////////////////////////////////////////////
const int screenWidth = 600;
const int screenHeight = 600;
const float PI = 3.1415926535f;

Color image[screenWidth*screenHeight];

///////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////
unsigned factorial(unsigned f)
{
    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}

unsigned nAlattAKaCseszem(unsigned n, unsigned k)
{
	return factorial(n) / (factorial(k) * factorial(n-k));
}

///////////////////////////////////////////////////////////////////
// CLASSES
///////////////////////////////////////////////////////////////////
bool ipad = false;
long spaceLenyomasIdeje = 0;

class Point
{
public:
	Vector pos;
	Vector velocity;
	Vector acc;
	float time;

	Point()
	{
		time = 0;
	}

	Point(Vector pos, float time)
	{
		this->pos = pos;
		this->time = time;
	}

	void draw(float weight)
	{
		glBegin(GL_POLYGON);
		for(float i = 0.0f; i < 2.0f * PI; i += PI / 8.0f)
		{
			glVertex2f(pos.x + cos(i) * weight, pos.y + sin(i) * weight);
		}
		glEnd( );
	}
};

class Spline
{
public:
	Point points[100];
	int pnum;

	Spline()
	{
		pnum = 0;
	}

	void drawPoints()
	{
		for (int i = 0; i < pnum; ++i)
		{
			points[i].draw(1);
		}
	}

	Vector egyenlet(int i, float t)
	{

		points[i].velocity.tostring();

		Vector a0, a1, a2, a3, a4;
		a0 = points[i].pos;
		a1 = points[i].velocity;
		a2 = points[i].acc;
		a3 = (points[i+1].pos * 4.0 - points[i].pos * 4.0) / pow(points[i+1].time - points[i].time, 3.0)
			- (points[i+1].velocity - points[i].velocity * 3.0) / pow(points[i+1].time - points[i].time, 2.0)
			- (points[i].acc) / (points[i+1].time - points[i].time);
		a4 = (points[i+1].pos * -3.0 + points[i].pos * 3.0) / pow(points[i+1].time - points[i].time, 4.0)
			+ (points[i+1].velocity + points[i].velocity * 2.0) / pow(points[i+1].time - points[i].time, 3.0)
			+ (points[i].acc * 0.5) / pow(points[i+1].time - points[i].time, 2.0);

		return a4 * pow(t - points[i].time, 4.0)
			+ a3 * pow(t - points[i].time, 3.0)
			+ a2 * pow(t - points[i].time, 2.0)
			+ a1 * (t - points[i].time)
			+ a0;
	}

	void spaceLenyomva()
	{
		for (int i = 1; i < pnum; ++i)
		{
			points[i].time -= points[0].time;
		}

		points[0].time = 0;
	}

	void draw()
	{
		setVel();
		setAcc();

		glColor3f(1,0,0);
		glBegin(GL_LINE_STRIP);
		//std::cout << floor(points[0].velocity.Length()) << std::endl;
		for (int i = 0; i < pnum - 1; ++i)
		{
			std::cout << "\\\\\\ index: " << i << " time: " << points[i].time << " vel: " << points[i].velocity.Length() << " acc: " << points[i].acc.Length() << "///" << std::endl;
			//std::cout << points[i].velocity.Length() << std::endl;
			float step = (points[i+1].time - points[i].time) / 100.0;
			for (float s = points[i].time; s < points[i+1].time; s += step)
			{
				Vector p = egyenlet(i, s);
				glVertex2d(p.x, p.y);
			}
		}
		glEnd();
	}

	void setVel()
	{
		points[pnum - 1].velocity = Vector();
		for (int i = 1; i < pnum - 1; ++i)
		{
			Vector elso, mas;
			elso = (points[i].pos - points[i-1].pos) * (1.0 / (points[i].time - points[i-1].time));
			mas = (points[i+1].pos - points[i].pos) * (1.0 / (points[i+1].time - points[i].time));
			points[i].velocity = (elso + mas) * 0.5;
		}
	}

	void setAcc()
	{
		for (int i = 0; i < pnum - 1; ++i)
		{
			Vector a2, a3, a4;
			a2 = points[i].acc;
			a3 = (points[i+1].pos * 4.0 - points[i].pos * 4.0) / pow(points[i+1].time - points[i].time, 3.0)
				- (points[i+1].velocity - points[i].velocity * 3.0) / pow(points[i+1].time - points[i].time, 2.0)
				- (points[i].acc) / (points[i+1].time - points[i].time);
			a4 = (points[i+1].pos * -3.0 + points[i].pos * 3.0) / pow(points[i+1].time - points[i].time, 4.0)
				+ (points[i+1].velocity + points[i].velocity * 2.0) / pow(points[i+1].time - points[i].time, 3.0)
				+ (points[i].acc * 0.5) / pow(points[i+1].time - points[i].time, 2.0);

			points[i+1].acc = a4 * 12.0 * pow(points[i+1].time - points[i].time, 2.0)
				+ a3 * 6.0 * (points[i+1].time - points[i].time)
				+ a2 * 2.0;
		}
	}

	void runningCircle()
	{
		float t = fmod(glutGet(GLUT_ELAPSED_TIME) / 1000.0 - spaceLenyomasIdeje / 1000.0, points[pnum-1].time);
		int index = 0;

		for (int i = 0; points[i].time < t; ++i)
		{
			index = i;
		}

		Vector kor = egyenlet(index, t);
		
		Point p(egyenlet(index, t), 1);
		glColor3f(0,1,0);
		p.draw(5);
	}

	void addPoint(Point p)
	{
		points[pnum++] = p;
	}
};

class Window
{
public:
	float left, right, bot, top;
	Vector speed;

	Window()
	{
		left = 100.0;
		right = 200.0;
		bot = 300.0;
		top = 400.0;
	}

	float getLeft()
	{
		return left;
	}

	float getRight()
	{
		return right;
	}

	float getTop()
	{
		return top;
	}

	float getBot()
	{
		return bot;
	}

	void setSpeed(Vector v)
	{
		speed = v;
	}

	Vector getSpeed()
	{
		return speed;
	}

	void move(long prev, long now)
	{
		for (int i = prev; i < now; i += 10)
		{
			long diff = now - i;
			long slice = (diff > 10) ? 10 : diff;

			if (ipad)
				this->draw(slice);
		}
	}

	void draw(long slice)
	{
		if (speed.Length() < 0.1)
		{
			speed.x = 0;
			speed.y = 0;
			ipad = false;
			return;
		}

		left += speed.x * (slice / 40.0f);
		right += speed.x * (slice / 40.0f);
		top += speed.y * (slice / 40.0f);
		bot += speed.y * (slice / 40.0f);

		Vector surl(speed / speed.Length());
		surl = surl * -1.0;
		float mu = 0.85;
		speed = speed * mu;

		glutPostRedisplay();
	}

	void ortho()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(left, right, bot, top);
	}

	Vector convertMouse(int x, int y)
	{
		//float _x = x - (screenWidth / 2.0) / 6.0 - left;
		float _x = ((float) x / (screenWidth / 100.0f)) + left;
		float _y = ((float) (screenHeight - y) / (screenHeight / 100.0f)) + bot;

		return Vector(_x, _y);
	}
};

///////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////

enum ClkState {B1CLK, B2CLK, B3CLK, ERR};

Window win;
long balKattintasIdeje = 0;
int kattintasX, kattintasY;
bool spaceLenyomva = false;
Spline g;
int lastX, lastY;
long elozoIdo;
ClkState aktualisAllapot = B1CLK;
bool ballenyomva = false;


///////////////////////////////////////////////////////////////////
// GL FUNCTIONS
///////////////////////////////////////////////////////////////////
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) { 
	glViewport(0, 0, screenWidth, screenHeight);

    for(int Y = 0; Y < screenHeight; Y++)
		for(int X = 0; X < screenWidth; X++)
			image[Y*screenWidth + X] = Color(0.4,0.4,0.4);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
	
	// ablak pozicio allitasa
	g.drawPoints();
	g.draw();
	win.ortho();

	// kirajzolas

    glutSwapBuffers();
}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	if (key == ' ' && !spaceLenyomva)
	{
		spaceLenyomva = true;
		spaceLenyomasIdeje = glutGet(GLUT_ELAPSED_TIME);
		g.spaceLenyomva();
	}
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
	Vector katt(win.convertMouse(x,y));

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (!ballenyomva)
		{
			g.addPoint(Point(katt, glutGet(GLUT_ELAPSED_TIME)));
			glutPostRedisplay();
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ballenyomva = false;
		balKattintasIdeje = glutGet(GLUT_ELAPSED_TIME);
		kattintasX = katt.x;
		kattintasY = katt.y;
	}
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{
	ballenyomva = true;
	Vector katt(win.convertMouse(x,y));
	long ido = glutGet(GLUT_ELAPSED_TIME);
	float t = (ido - balKattintasIdeje) / 1000.0;
	if (t <= 0.5)
	{
		//float xx = katt.x / 3 + win.getLeft();
		//float yy = (600.0 - katt.y) / 3 + win.getBot();

		//Vector elmozdulas(xx - kattintasX, yy - kattintasY);

		Vector elmozdulas( (kattintasX - katt.x), (kattintasY - katt.y));

		win.setSpeed(elmozdulas * (-0.333));
		ipad = false;
	}
	else
	{
		ipad = true;
	}
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( )
{
	long aktIdo = glutGet(GLUT_ELAPSED_TIME);
	win.move(elozoIdo, aktIdo);
	elozoIdo = aktIdo;

	glutPostRedisplay();
}


// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}
