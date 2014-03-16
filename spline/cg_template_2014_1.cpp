//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül 
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
// Nev    : Takacs Robert
// Neptun : QECX4A
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

#include <stdio.h>// KISZED!!

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

const float PI = 3.14;
bool moved = false;
float tenzio = -0.5;

enum Status{
	putCP,
	ballMoving,
} status;



float minfloat(float a, float b){
	if (a < b){
		return a;
	}
	else{
		return b;
	}

}



//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
	float x, y, z;

	Vector() {
		x = y = z = 0;
	}
	Vector(float x0, float y0, float z0 = 0) {
		x = x0; y = y0; z = z0;
	}
	Vector operator*(float a) {
		return Vector(x * a, y * a, z * a);
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
	float operator/(const Vector& v) {
		return (x / v.x + y / v.y + z / v.z);
	}

	Vector operator%(const Vector& v) { 	// cross product
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() { return sqrt(x * x + y * y + z * z); }
	Vector operator/(float a) {
		return Vector(x / a, y / a, z / a);
	}
	Vector normalize() {
		/*if (Length() == 0.0) return Vector(0.0, 0.0, 0.0);
		return  *this * (1 / sqrt(x * x + y * y + z * z));*/
		return Vector(x / Length(), y / Length(), z / Length());
	}
};


void drawEllipse(Vector vector, float lambda, float radius){
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(vector.x, vector.y);
	float px, py = 0;

	for (float i = 0; i <= 361; i++){
		px = (lambda)* 1 * cosf(i *PI / 180.0f)* radius + vector.x;
		py = 1 * sinf(i*PI / 180.0f)* radius + vector.y;
		glVertex2f(px, py);
	}

	glEnd();

}

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
	float r, g, b;

	Color() {
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

struct ControlPoint{
	Vector pos;
	Vector velocity;
	Vector acceleration;
	float time;

	ControlPoint(){
		velocity = Vector();
		acceleration = Vector();

	}

	ControlPoint(Vector pos, float time){
		this->pos = Vector(pos.x, pos.y, 0.0f);
		this->time = time;
	}


};
enum MouseStates{
	B0CLK,
	B1CLK,
	B2CLK,
	B3CLK,
	mouseMoving,
};

struct Mouse{
	MouseStates mouseStates;
	Mouse(){
		mouseStates = B0CLK;

	}

	void mouseStatesplus(){
		switch (mouseStates)
		{
		case B0CLK: mouseStates = B1CLK;
			break;
		case B1CLK: mouseStates = B2CLK;
			break;
		case B2CLK: mouseStates = B3CLK;
			break;

		}

	};

	float mouseLastUP;
	ControlPoint mouseFirstdMoving;
	ControlPoint mouseLastValidMoving;
	ControlPoint mouseLastStuff;


};
Mouse mouse;

struct World{
	ControlPoint points[100];
	int numberOfPoints;
	Vector worldCenter;
	Vector velocity;
	float oldTime;
	float spacePush;

	World(){
		numberOfPoints = 0;
		worldCenter = Vector(150.0f, 350.0f, 0.0f);
		velocity = Vector(0.0f, 0.0f, 0.0f);
		oldTime = 0.0f;

	}

	void addCP(Vector pos, float time){
		if (numberOfPoints < 99){
			points[numberOfPoints] = ControlPoint(pos + worldCenter, time);
			numberOfPoints++;
		}
	}


	/*void drawSpline(){
		for (int i = 0; i <= numberOfPoints - 1; i++){
		float d[2] = { points[i].pos.x, points[i].pos.y };
		float c[2];
		float b[2];
		float a[2];
		float foox;
		float fooy;
		float vnext[2];


		if (i == 0){
		c[0] = 0;
		c[1] = 0;
		}
		else
		{
		c[0] = ((points[i+1].pos.x - points[i].pos.x) / (points[i+1].time - points[i].time) + (points[i].pos.x - points[i-1].pos.x) / (points[i].time - points[i-1].time)) * (1 - tenzio) / 2.0f;
		c[1] = ((points[i+1].pos.y - points[i].pos.y) / (points[i+1].time - points[i].time) + (points[i].pos.y - points[i-1].pos.y) / (points[i].time - points[i-1].time)) * (1 - tenzio) / 2.0f;
		}

		if (i == numberOfPoints - 1){
		vnext[0] = 0.0f;
		vnext[1] = 0.0f;
		}
		else{
		vnext[0] = ((points[i+2].pos.x - points[i+1].pos.x) / (points[i+2].time - points[i+1].time) + (points[i+1].pos.x - points[i].pos.x) / (points[i+1].time - points[i].time)) * (1 - tenzio) / 2.0f;
		vnext[1] = ((points[i+2].pos.y - points[i+1].pos.y) / (points[i+2].time - points[i+1].time) + (points[i+1].pos.y - points[i].pos.y) / (points[i+1].time - points[i].time)) * (1 - tenzio) / 2.0f;
		}
		b[0] = 3 * (points[i+1].pos.x - points[i].pos.x) / pow((points[i+1].time - points[i].time), 2.0) - (vnext[0] + 2 * c[0]) / (points[i+1].time - points[i].time);
		b[1] = 3 * (points[i+1].pos.y - points[i].pos.y) / pow((points[i+1].time - points[i].time), 2.0) - (vnext[1] + 2 * c[1]) / (points[i+1].time - points[i].time);

		a[0] = 2 * (points[i].pos.x - points[i+1].pos.x) / pow(points[i+1].time - points[i].time, 3.0) + (vnext[0] + c[0]) / pow(points[i+1].time - points[i].time, 2.0);
		a[1] = 2 * (points[i].pos.y - points[i+1].pos.y) / pow(points[i+1].time - points[i].time, 3.0) + (vnext[1] + c[1]) / pow(points[i+1].time - points[i].time, 2.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		for (float time = points[i].time; time <points[i+1].time; time += minfloat(1.0f, points[i+1].time - time)){
		foox = a[0] * pow(time - points[i].time, 3.0f) + b[0] * pow(time - points[i].time, 2) + c[0] * (time - points[i].time) + d[0];
		fooy = a[1] * pow(time - points[i].time, 3.0f) + b[1] * pow(time - points[i].time, 2) + c[1] * (time - points[i].time) + d[1];
		glVertex2f(foox, fooy);
		}
		glEnd();
		}
		}*/

	Vector calculate(int i, float time){

		Vector a0, a1, a2, a3, a4;
		a0 = points[i].pos;
		a1 = points[i].velocity;
		a2 = points[i].acceleration / 2.0f;
		a3 = (points[i + 1].pos * 4.0 - points[i].pos * 4.0) / pow(points[i + 1].time - points[i].time, 3.0f)
			- (points[i + 1].velocity + points[i].velocity * 3.0) / pow(points[i + 1].time - points[i].time, 2.0f)
			- (points[i].acceleration) / (points[i + 1].time - points[i].time);
		a4 = (points[i + 1].pos * -3.0 + points[i].pos * 3.0) / pow(points[i + 1].time - points[i].time, 4.0f)
			+ (points[i + 1].velocity + points[i].velocity * 2.0) / pow(points[i + 1].time - points[i].time, 3.0f)
			+ (points[i].acceleration * 0.5) / pow(points[i + 1].time - points[i].time, 2.0f);

		return a4 * pow(time - points[i].time, 4.0f)
			+ a3 * pow(time - points[i].time, 3.0f)
			+ a2 * pow(time - points[i].time, 2.0f)
			+ a1 * (time - points[i].time)
			+ a0;


	}

	void drawSpline(){

		setVel();
		setAcc();


		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		
		glVertex2f(points[0].pos.x, points[0].pos.y);
		glVertex2f(points[0].velocity.x + points[0].pos.x, points[0].velocity.y + points[0].pos.y);

		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex2f(points[0].pos.x, points[0].pos.y);
		glVertex2f(points[0].acceleration.x + points[0].pos.x, points[0].acceleration.y + points[0].pos.y);

		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_STRIP);

		for (int i = 0; i < numberOfPoints - 1; i++){

			for (float time = points[i].time; time < points[i + 1].time; time += (points[i + 1].time - points[i].time) / 100.0){
				Vector foo = calculate(i, time);
				glVertex2f(foo.x, foo.y);
			}
			

		}
		glEnd();
	}

	void setVel()
	{
		points[numberOfPoints - 1].velocity = Vector();
		for (int i = 1; i < numberOfPoints - 1; ++i)
		{
			Vector elso, mas;
			elso = (points[i].pos - points[i - 1].pos)  / (points[i].time - points[i - 1].time);
			mas = (points[i + 1].pos - points[i].pos) / (points[i + 1].time - points[i].time);
			points[i].velocity = (elso + mas) * 0.5;
			int asd;
		}
	}

	void setAcc()
	{
		for (int i = 0; i < numberOfPoints - 1; ++i)
		{
			Vector a2, a3, a4;
			a2 = points[i].acceleration / 2.0f ;
			a3 = (points[i + 1].pos * 4.0 - points[i].pos * 4.0) / pow(points[i + 1].time - points[i].time, 3.0f)
				- (points[i + 1].velocity + points[i].velocity * 3.0) / pow(points[i + 1].time - points[i].time, 2.0f)
				- (points[i].acceleration ) / (points[i + 1].time - points[i].time);
			a4 = (points[i + 1].pos * -3.0 + points[i].pos * 3.0) / pow(points[i + 1].time - points[i].time, 4.0f)
				+ (points[i + 1].velocity + points[i].velocity * 2.0) / pow(points[i + 1].time - points[i].time, 3.0f)
				+ (points[i].acceleration * 0.5 ) / pow(points[i + 1].time - points[i].time, 2.0f);

			points[i + 1].acceleration = a4 * 12.0 * pow(points[i + 1].time - points[i].time, 2.0f)
				+ a3 * 6.0 * (points[i + 1].time - points[i].time)
				+ a2 * 2.0;
		}
	}





	void drawControlPoints(){
		glColor3f(1.0f, 0.0f, 0.0f);
		for (int i = 0; i < numberOfPoints; i++){
			drawEllipse(points[i].pos, 1.0f, 1.0f);

		}
	}

	void drawBall(float timeNow){
		if (spacePush != 0.0){
			float dif = points[numberOfPoints-1].time - points[0].time;
			float mar = fmod(timeNow - spacePush, dif);

			float time = mar + points[0].time;

			int index = 0;
			for (int i = 0; points[i].time < time; i++)
				index = i;
			if (index > 0){

				int asder = 0;
			}

			Vector c = calculate(index, time);
			glColor3f(0.0f, 1.0f, 0.0f);
			drawEllipse(c, 1.0f, 1.0f);
			printf("drawBall %lf , %lf , %lf\n", c.x,c.y, time);
		
		}

	}

	void move(float time){


		if (time - (oldTime + 0,01) > 0.0 && velocity.Length() != 0.0){
			float distance = velocity.Length() *((time - oldTime) / 150.0);
			if (velocity.Length() != 0.0){
				printf("move vel length %lf\n", velocity.Length());
				float asd = velocity.Length();
			}
			oldTime += 0, 01;
			Vector asd = velocity.normalize();
			asd = asd *distance;
			printf("move diff %lf bb  %lf\n", asd.x , asd.y);
			worldCenter = worldCenter + asd;
			velocity = velocity *0.90;
			if (velocity.Length() < 0.1) velocity = Vector(0.0, 0.0, 0.0);

			
		}

	}
	void move2(float time){
		if (velocity.Length() < 0.1){
			velocity = Vector();
			return;
		}
		Vector surolodas(velocity / velocity.Length());
		surolodas = surolodas * -1.0f;
		float mu = 0.85;
		velocity = velocity * mu;

		/*Vector x;
		Vector a = velocity / time;
		if (time < velocity / a)
			x = worldCenter + velocity * time - a / 2 * time * time;
		else  x = worldCenter + velocity * (velocity / a) * 0.5 ;


		worldCenter = x;*/


		glutPostRedisplay();

	}
	


};
World world;



const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;

Vector convertWinToGl(int x, int y){
	return Vector((x - (screenWidth / 2.0f)) / 6.0f, -(y - screenHeight / 2.0f) / 6.0f);
}

Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
	glViewport(0, 0, screenWidth, screenHeight);
	status = putCP;
	mouse.mouseStates = B0CLK;
	world.oldTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-50.0f + world.worldCenter.x, 50.0f + world.worldCenter.x, -50.0f + world.worldCenter.y, 50.0f + world.worldCenter.y);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles


	world.drawControlPoints();
	world.drawSpline();
	if (status == ballMoving){
		world.drawBall(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	}

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	if (key == 'd'){
		glutPostRedisplay(); 		// d beture rajzold ujra a kepet
		world.worldCenter = Vector(150.0f, 350.0f, 0.0f);
	}
	if (key == ' ' && world.spacePush == 0.0){
		world.spacePush = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
		status = ballMoving;

	}
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

//button handling
void doMouse(){
	switch (mouse.mouseStates)
	{
	case B0CLK:
		break;
	case B1CLK: world.addCP(mouse.mouseLastStuff.pos, mouse.mouseLastStuff.time); mouse.mouseStates = B0CLK; glutPostRedisplay();
		break;
	case B2CLK:
		world.points[0].velocity = (mouse.mouseLastStuff.pos + world.worldCenter) - world.points[0].pos;
		printf(" 2katt\n"); mouse.mouseStates = B0CLK; glutPostRedisplay();//do sebességvektor
		break;
	case B3CLK:printf("3katt\n");
		world.points[0].acceleration = (mouse.mouseLastStuff.pos + world.worldCenter) - world.points[0].pos;
		mouse.mouseStates = B0CLK; glutPostRedisplay();//do gyorsulásvektor
		break;

	case mouseMoving:
		world.velocity = (mouse.mouseFirstdMoving.pos - mouse.mouseLastValidMoving.pos) * 0.3;
		//world.oldTime = glutGet(GLUT_ELAPSED_TIME); animacio miatt TODO

		break;

	}


}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
	//printf("%d,%d , katt\n", x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay(); 						 // Ilyenkor rajzold ujra a kepet
		//moved = false;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		//if (moved == false){
		if (mouse.mouseStates != mouseMoving){
			mouse.mouseLastUP = glutGet(GLUT_ELAPSED_TIME);
			mouse.mouseLastUP = mouse.mouseLastUP / 1000.0f;
			mouse.mouseStatesplus();
			mouse.mouseLastStuff = ControlPoint(convertWinToGl(x, y), mouse.mouseLastUP);
			printf("%lf, %lf, katt gl\n", convertWinToGl(x, y).x, convertWinToGl(x, y).y);
		}
		else{//mouse.mouseStates = mouse.mouseMoving ban vagyunk
			//mozgat
			doMouse();
			mouse.mouseStates = B0CLK;
		}

	}
	glutPostRedisplay();
}





// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	//moved = true;
	if (mouse.mouseStates != mouseMoving){
		doMouse();
		mouse.mouseFirstdMoving.time = time; //save mouse pos, time
		mouse.mouseFirstdMoving.pos = Vector(convertWinToGl(x, y));

	}
	mouse.mouseStates = mouseMoving;
	printf("%d,%d , mozog\n", x, y);

	//utolso pos mentes
	

	if (time - mouse.mouseFirstdMoving.time < 0.5){
		mouse.mouseLastValidMoving.pos = Vector(convertWinToGl(x, y));

	}
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle() {
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;		// program inditasa ota eltelt ido



	world.move(time);

	if (time - mouse.mouseLastUP > 0.3 && mouse.mouseStates != B0CLK && mouse.mouseStates != mouseMoving)
		doMouse();

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

