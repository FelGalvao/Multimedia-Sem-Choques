#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "glm.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**************************************
************* CONSTANTE PI ************
**************************************/

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/**************************************
* AUXILIARES CONVERSÃO GRAUS-RADIANOS *
**************************************/

#define rtd(x) (180 * (x) / M_PI)
#define dtr(x) (M_PI * (x) / 180)

#define DEBUG 1

/**************************************
********** VARIÁVEIS GLOBAIS **********
**************************************/
GLMmodel* pmodel = NULL;
float angulo = 0.0;
/* Número de objetos */
int num_obj = 150;

GLfloat random_obj[150][3];
GLfloat random_mont[60][3];

/*/ Ângulo para a direção da camera */
float angle = 0.0;

/* Representação da direção da camera */
float lx = 0.0f, lz = -1.0f, ly = 1.0f;

/* Posição da camera */
float x = 0.0f, z = 50.0f, y = 210.0f;
//float x = 0.0f, z =1100.0f, y = 825.0f;

GLboolean rodar_mundo = GL_FALSE;

/**************************************
*** INICIALIZAÇÃO DO AMBIENTE OPENGL **
**************************************/

void init(void)
{
  glEnable(GL_DEPTH_TEST);
}

/**************************************
** ESPAÇO PARA DEFINIÇÃO DAS ROTINAS **
****** AUXILIARES DE DESENHO ... ******
**************************************/

/* Função para posicionar a camera */
void pos_camera()
{
  //gluLookAt(x, y, z, x + lx, y - 0.15, z + lz, 0.0f, 1.0f, 0.0f);
  
  gluLookAt(x, y, z, x+lx, y - 0.15, z + lz, 0.0f, 1.0f, 0.0f);
 // gluLookAt(x, 30, z, x+lx,30, z+lz , 0.0f, 1.0f, 0.0f);// posicao mais perto do chao em direcao ao eixo y, no sentido do eixo z(sendo decrementado)
  //ly...par onde estar a olhar.. a modificar juntamente com as teclas
}

/* Função para mover a camera */
void move_camera()
{
  //z -= 10.0;//"velocidade"
  z -= 5.0;
//atualizar o x e z...2d do chao.....logica da bola(ping pong)
}

/* Função para definir o posicionamento dos objetos */
void random_objs()
{
  for (int i = 0; i < num_obj; i++)
  {
    if (i % 2 == 0)
      random_obj[i][0] = (rand() % 2000);
    else
      random_obj[i][0] = -(rand() % 2000);

    random_obj[i][1] = 5.0;

    random_obj[i][2] = -((rand() % 10000) + 100);
  }
}

/* Função para desenhar solo */
void desenhar_solo()
{
  glDisable(GL_LIGHTING);
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);

  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(-5000.0f, 0.0f, -10000.0f + z);

  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(5000.0f, 0.0f, -10000.0f + z);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(5000.0f, 0.0f, 500.0f +z );
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-5000.0f, 0.0f, 500.0f +z);

  glEnd();
  glEnable(GL_LIGHTING);
}

/* Função para desenhar pista */
void desenhar_pista()
{
  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_POLYGON);

 
  glVertex3f(-50.0f, 0.5f, -3550.0f);
  glVertex3f(-50.0f, 0.5f, 100.0f);
  glVertex3f(50.0f, 0.5f, 100.0f);
  glVertex3f(50.0f, 0.5f, -3550.0f);
 
  glEnd();
  glEnable(GL_LIGHTING);
}

/* Funções para desenhar montanhas */
void random_montanha(int x)
{

  float z = -1000.0f;
  float y_s = 10.0f, y_m = 50.0f, y_l = 75.0f;
  float x_s = 20.0f, x_l = 40.0f;

  float y_scales[3] = {y_s, y_m, y_l};
  float x_scales[2] = {x_s, x_l};

  random_mont[0][0] = 0.0f;
  random_mont[0][1] = 0.0f;
  random_mont[0][2] = z;

  random_mont[2][0] = x_l;
  random_mont[2][1] = 0.0f;
  random_mont[2][2] = z;

  random_mont[1][0] = (random_mont[0][0] + random_mont[2][0]) / 2;
  random_mont[1][1] = (rand() % 100) + 30.0f;
  random_mont[1][2] = z;

  for (int i = 3; i < x / 2; i = i + 3)
  {

    random_mont[i][0] = random_mont[i - 1][0];
    random_mont[i][1] = 0.0f;
    random_mont[i][2] = z;

    random_mont[i + 2][0] = random_mont[i - 1][0] + x_scales[1];
    random_mont[i + 2][1] = 0.0f;
    random_mont[i + 2][2] = z;

    random_mont[i + 1][0] = (random_mont[i][0] + random_mont[i + 2][0]) / 2;
    random_mont[i + 1][1] = (rand() % 100) + 30.0f;
    random_mont[i + 1][2] = z;
  }
}
void montanhas(float base, float height)
{
  GLUquadricObj *p = gluNewQuadric();
  gluQuadricDrawStyle(p, GL_LINES);

  glPushMatrix();
  glColor3ub(136, 0, 21);
  glTranslatef(0, 0, -50);
  glRotatef(-90, 1, 0, 0);
  glTranslatef(-5.0, -20.0, 40.0);
  gluCylinder(p, base, 50, height, 20, 20);
  glPopMatrix();
}
void desenhar_montanhas()
{
  float montY = -15.0;

  glPushMatrix();
  glTranslatef(0, montY, -5000);
  montanhas(100, 100);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(200, montY, -5000);
  montanhas(150, 200);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-200, montY, -5000);
  montanhas(175, 300);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-400, montY, -5000);
  montanhas(100, 150);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-700, montY, -5000);
  montanhas(200, 400);
  glPopMatrix();
}

/* Função para desenhar o céu */
void desenhar_ceu()
{
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);

  glColor3f(0.0, 0.6, 0.8);
  glVertex3f(-5000.0f, -1000.0f, -5500.0f + z);
  //glVertex3f(0.0f, -1000.0f, -5500.0f + z);
  glVertex3f(-5000.0f, 3000.0f, -5500.0f + z);
  //glVertex3f(0.0f, 3000.0f, -5500.0f + z);

  glColor3f(0.130, 0.280, 0.450);
  glVertex3f(5000.0f, 3000.0f, -5500.0f + z);
  //glVertex3f(0.0f, 3000.0f, -5500.0f + z);
  glVertex3f(5000.0f, -1000.0f, -5500.0f + z);
  //glVertex3f(0.0f, -1000.0f, -5500.0f + z);

  glEnd();
  glEnable(GL_LIGHTING);
}

void desenhar_Ceu_esquerdo(){
   glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);

  glColor3f(0.0, 0.6, 0.8);
  glVertex3f(-5000.0f, 0.0f, 0.0f);
  //glVertex3f(0.0f, -1000.0f, -5500.0f + z);
  glVertex3f(-5000.0f, 3000.0f, 0.0f);
  //glVertex3f(0.0f, 3000.0f, -5500.0f + z);

  glColor3f(0.130, 0.280, 0.450);
  glVertex3f(-5000.0f, 3000.0f, -5500.0f + z);
  //glVertex3f(0.0f, 3000.0f, -5500.0f + z);
  glVertex3f(-5000.0f, -1000.0f, -5500.0f + z);
  //glVertex3f(0.0f, -1000.0f, -5500.0f + z);

  glEnd();
  glEnable(GL_LIGHTING);
}

void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat cor[])
{

  glBegin(GL_POLYGON);
    glColor3fv(cor);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glVertex3fv(d);
  glEnd();
}

void drawmodel(void)
{
  
    if (!pmodel) {
        pmodel = glmReadOBJ("Objetos/al.obj");
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}


void esfera(){
  glColor3f(0.0,1.0,1.0); 
  glPushMatrix();
  //glTranslatef(x,y,z);
      glTranslatef(0, 0, z);
        glutSolidSphere(2000, 100,100);
        //glutSolidSphere(10000, 10,10); //nao aparece, na mesma, o boneco
        glPopMatrix();

}


void cubo()
{
  GLfloat vertices[][3] = { 
                {-5000, -2000, 500+z}, 
                {5000, -2000,500+z}, 
                {5000,-2000, -3000.0f + z}, 
                {-5000, -2000, -3000.0f + z}, 
                {-5000, 3000,500  +z},  
                {5000,3000,500 +z }, 
                {5000, 3000, -3000.0f + z}, 
                {-5000, 3000, -3000.0f + z}
                };

  GLfloat cores[][3] = {{0.0,1.0,1.0},
                        {1.0,0.0,0.0},
                        {1.0,1.0,0.0}, 
                        {0.0,1.0,0.0}, 
                        {1.0,0.0,1.0}, 
                        {0.0,0.0,1.0}, 
                        {1.0,1.0,1.0}};

  desenhaPoligono(vertices[0],vertices[1],vertices[5],vertices[4],cores[0]);
  desenhaPoligono(vertices[1],vertices[2],vertices[6],vertices[5],cores[1]);
  desenhaPoligono(vertices[2],vertices[6],vertices[7],vertices[3],cores[2]);
  desenhaPoligono(vertices[3],vertices[7],vertices[4],vertices[0],cores[3]);
  desenhaPoligono(vertices[7],vertices[4],vertices[5],vertices[6],cores[4]);
  //desenhaPoligono(vertices[5],vertices[4],vertices[0],vertices[1],cores[5]);
}

/* Função para desenhar o sol */
void desenhar_sol()
{
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glColor3f(1.0f, 1.0f, 0.0f);
  glTranslatef(300, 700, -4000 + z);
  glutSolidSphere(50, 30, 20);
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/* Funções para desenhar nuvens */
void nuvens2()
{
  glPushMatrix();

  glColor3ub(224, 224, 224);
  glTranslatef(0, 20, -40);
  glutSolidSphere(50, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(0, 20, 0);
  glutSolidSphere(20, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(10, 15, 10);
  glutSolidSphere(30, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(10, 45, -30);
  glutSolidSphere(30, 20, 20);
  glPopMatrix();
}
void nuvens()
{
  glPushMatrix();

  glColor3ub(224, 224, 224);
  glTranslatef(0, 20, -40);
  glutSolidSphere(10, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(0, 20, -30);
  glutSolidSphere(5, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(-10, 30, -30);
  glutSolidSphere(10, 30, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(0, 25, -30);
  glutSolidSphere(10, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(0, 10, -30);
  glutSolidSphere(10, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(0, 20, -50);
  glutSolidSphere(10, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(-20, 20, -50);
  glutSolidSphere(20, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glColor3ub(224, 224, 224);
  glTranslatef(-15, 20, -10);
  glutSolidSphere(20, 20, 20);
  glPopMatrix();
}
void desenhar_nuvens(float rZ)
{
  float mcolor[] = {1.0f, 0.0f, 0.0f, 1.0f};
  GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0};
  glMaterialfv(GL_LEFT, GL_AMBIENT_AND_DIFFUSE, mcolor);
  glMaterialfv(GL_LEFT, GL_SPECULAR, qaWhite);
  glMaterialf(GL_LEFT, GL_SHININESS, 20);

  for (int i = 0; i < num_obj; i++)
  {
    glPushMatrix();

    glTranslatef(random_obj[i][0] + 50, 1100, random_obj[i][2] + 100 + rZ);

    nuvens2();
    
    glPopMatrix();
  }
}

/* Funções para desenhar árvores */
void arvores()
{
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(0.0, 20.0, -40.0);
  glRotatef(90, 1, 0, 0);
  glutSolidCone(3, 40, 40, 20);
  glTranslatef(0.0, -20.0, 40.0);
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.0, 0.8, 0.0);
  glTranslatef(0.0, 20.0, -40.0);
  glRotatef(-90, 1, 0, 0);
  glutSolidCone(7, 10, 40, 20);
  glTranslatef(0.0, -20.0, 40.0);
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.0, 0.7, 0.0);
  glTranslatef(0.0, 13.0, -40.0);
  glRotatef(-90, 1, 0, 0);
  glutSolidCone(10, 10, 40, 20);
  glTranslatef(0.0, -20.0, 40.0);
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.0, 0.6, 0.0);
  glTranslatef(0.0, 4.0, -40.0);
  glRotatef(-90, 1, 0, 0);
  glutSolidCone(15, 12, 40, 20);
  glTranslatef(0.0, -20.0, 40.0);
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.0, 0.6, 0.0);
  glTranslatef(0.0, -5.0, -40.0);
  glRotatef(-90, 1, 0, 0);
  glutSolidCone(20, 16, 5, 5);
  glTranslatef(0.0, -20.0, 40.0);
  glPopMatrix();
}
void desenhar_arvores(float rZ)
{
  for (int i = 0; i < num_obj; i++)
  {
    glPushMatrix();

    glTranslatef(random_obj[i][0] + 50, 10, random_obj[i][2] + 100 + rZ);
    glScaled(3, 3, 3);
    arvores();
    glPopMatrix();
  }
}

/**************************************
***** CALL BACKS DE JANELA/DESENHO ****
**************************************/

/* Callback para redimensionar janela */
void reshape(int width, int height)
{
  if (height == 0)
    height = 1;
  float ratio = width * 1.0 / height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  
  gluPerspective(45.0f, ratio, 0.1f, 10000.0f); //projecao (angulo,aspect, zNear tava 0.1, zFar tava60000)
  //gluPerspective(45.0f, ratio, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //gluLookAt(0.0, 0.0, 5.0,0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}



void display(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();


//z=-200;  se puser, para
  /* Posicionar a camera */
  pos_camera();
  move_camera();


//glTranslatef(x,y,z);//translacao para a camera//modificam o eixo (ponto inicial)

//glRotatef(angulo, 0, 1, 0);

    //esfera();
    desenhar_solo();
    desenhar_pista();

    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(180,0,1,0);
    glScalef(0.2,0.2,0.2);
    drawmodel();
    glPopMatrix();  

//if(z<=-3500.0 && z>=-4000.0 && !rodar_mundo){  //se calhar, melhor fazer individualmente

  //glRotatef(-90, 0, 1, 0);
  //rodar_mundo = GL_TRUE; // 
  //}
  /* Desenhar objetos */
 // if(z <= 0 && z>-3400.0){// if(z >= -4000){ //tudo comeca em {0,0,0}
   // desenhar_pista();
  //}
  //esfera();
  //cubo();
  //desenhar_solo();
  // desenhar_pista();
 //if(z<=-3500.0 && z>=-4000.0){
 //desenhar_ceu();
 //glPushMatrix();
   //glColor3f(0.0, 0.8, 0.0);
   //glTranslatef(0.0, 20.0, -40.0);
   //glTranslatef(5.0, 0.0, -3550);
  // glTranslatef(5.0, 0.0, -2000);
 //glRotatef(-90, 0, 1, 0);
 //desenhar_pista();
   // glPopMatrix();
   //glutSolidCone(7, 10, 40, 20);
  
  //glRotatef(90, 0, 1, 0);
  //glTranslatef(50.0, 0.0, -3500.0);
 

// }
  desenhar_montanhas();
 // desenhar_ceu();
 
  desenhar_sol();

if(x <-50 && x >50 && z < -6200){
    desenhar_arvores(0);
    
  }
  if (z < -6200)
  {
    desenhar_nuvens(-6000);
    desenhar_arvores(-6000);
    
  }
  else
  {
    desenhar_nuvens(0);
    //desenhar_arvores(0);
    }

    

    

  glutSwapBuffers();

}

/**************************************
******** CALLBACKS TIME/IDLE **********
**************************************/

/* Callback Idle */
void idle(void)
{
  /* Ações a tomar quando o GLUT está idle */

  /* Redesenhar o ecrã */
  // glutPostRedisplay();
}

/* Callback de temporizador */
void timer(int value)
{
  /* Acções do temporizador ...
     Não colocar aqui primitivas OpenGL de desenho glBegin, glEnd, etc. */

  glutTimerFunc(10, timer, 0);

  /* Redesenhar o ecrã (invoca o callback de desenho) */
  glutPostRedisplay();
}

/**************************************
********* CALLBACKS TECLADO ***********
**************************************/

/* Callback para interação via teclado (carregar na tecla) */
void processNormalKeys(unsigned char key, int xx, int yy)
{
  switch (key)
  {
  case 13: // Tecla Enter
    glutFullScreen();
    break;
  case 27: // Tecla Esc
    exit(0);
  }

  if (DEBUG)
    printf("Carregou na tecla %c\n", key);
}

void processSpecialKeys(int key, int xx, int yy)
{
  /* Ações sobre outras teclas especiais
      GLUT_KEY_F1 ... GLUT_KEY_F12
      GLUT_KEY_UP
      GLUT_KEY_DOWN
      GLUT_KEY_LEFT
      GLUT_KEY_RIGHT
      GLUT_KEY_PAGE_UP
      GLUT_KEY_PAGE_DOWN
      GLUT_KEY_HOME
      GLUT_KEY_END
      GLUT_KEY_INSERT */

  float fraction = 0.05f;

  switch (key)
  {

  case GLUT_KEY_LEFT:
    
    // if(angle >=-0.78 ){//???
     angle -= 0.01f;
//angulo--;
     lx = sin(angle);
    lz = -cos(angle);
    //}
    break;
  case GLUT_KEY_RIGHT:
   
    //if(angle <= 0.78 ){//???? em radianos
     angle += 0.01f;
     //angulo++;
    lx = sin(angle);//para onde estar a olhar...agora mudar a posicao
    lz = -cos(angle);
   // }
    break;
  case GLUT_KEY_UP:
    if(y <= 8000 ){ // condicao para nao ultrapassar o limite de y pra cima
    lx += lx * fraction;
    lz += lz * fraction;
        }
    break;
  case GLUT_KEY_DOWN:
  if( y >=-1500 ){
    lx -= lx * fraction;
    lz -= lz * fraction;
  }
    break;
  }

  if (DEBUG)
    printf("Carregou na tecla %c\n", key);
}

/**************************************
********* CALLBACKS MENU ***********
**************************************/

void menu(int op)
{

  switch (op)
  {
  case 0:
    glutFullScreen();
    break;
  case 1:
  case 'Q':
  case 'q':
    exit(0);
  }
}

/**************************************
************ ILUMINAÇÃO ***************
**************************************/

void lighting()
{

  glEnable(GL_COLOR_MATERIAL);
  GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8, 1.0f};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};
  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glShadeModel(GL_SMOOTH);

  glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

/**************************************
************ FUNÇÃO MAIN **************
**************************************/

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1366, 768);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  if (glutCreateWindow("Projeto Multimédia I") == GL_FALSE)
    exit(1);

  /* Inicialização */
  init();
  random_objs();
  random_montanha(50);

  /* Registar callbacks do GLUT */

  /* callbacks de janelas/desenho */
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);

  /* Callbacks de teclado */
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);

  /* Menu */
  glutCreateMenu(menu);
  glutAddMenuEntry("Fullscreen", 0);
  glutAddMenuEntry("Exit", 'q');
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  /* Callbacks timer/idle */
  glutTimerFunc(10, timer, 0);
  //glutIdleFunc(idle);

  /* Ligar luzes */
  lighting();

  /* Começar loop */
  glutMainLoop();

  return 0;
}
