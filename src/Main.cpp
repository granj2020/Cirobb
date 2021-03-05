/*************************************************************************
* Copyright (c) 2019-2021 Jonathan Peña
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Jonathan Peña makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty. 
**************************************************************************/

#include "Scene.h"

int numScene = 1;
bool pause = false;
bool InitScene = true;

real fps = 1.0f / 60.0f;

Scene scene(Vec2(0, -10.0f), 12, 4);

Circle cPrincipal(10); OBB bPrincipal(20, 20);

RigidBody* sPrincipal = new RigidBody(cPrincipal, Vec2(0, -100), 90 * RAD); //

RigidBody* cLocal = new RigidBody(Circle(15), Vec2(-23, 0), 0);
RigidBody* bLocal = new RigidBody(OBB(40, 24), Vec2(18, 0), -PI / 20.0f);

RigidBody* w1 = new RigidBody(OBB(4, 50), Vec2( 20, 0), 0);
RigidBody* w2 = new RigidBody(OBB(4, 50), Vec2(-20, 0), 0);
RigidBody* w3 = new RigidBody(OBB(40, 4), Vec2(0, -25), 0);
RigidBody* w4 = new RigidBody(OBB(40, 4), Vec2(0,  25), 0);


void DrawText(int x, int y, char *string)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h = glutGet(GLUT_WINDOW_HEIGHT);
  gluOrtho2D(0, w, h, 0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRasterPos2i(x, y);
  for (int i = 0; i < (int)strlen(string); i++) 
  {
  	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}



void DetectionCollision(RigidBody* p)
{
  cLocal->shape->DrawShape();
  bLocal->shape->DrawShape();
  sPrincipal->shape->DrawShape();
  
  Manifold m1(p, cLocal);
  Manifold m2(p, bLocal);
  
  if(m1.numContacts == 1) 
  {
    DrawPoint(m1.contacts[0].position, 2, 4);
    Vec2 value = m1.normal * m1.A->shape->radius; 
    DrawLine(m1.A->position + value , m1.A->position + value + m1.normal * m1.contacts[0].penetration);
  }
  
  if(m2.numContacts > 0) 
  {
    for(int i = 0; i < m2.numContacts; i++)
    {
      DrawPoint(m2.contacts[i].position, 2, 4);
      DrawLine(m2.contacts[i].position, m2.contacts[i].position - m2.normal * m2.contacts[i].penetration);
    }
  }
}



void LowFriction(void)
{
  RigidBody* b1 = new RigidBody(OBB(110.0, 2.0f), Vec2(0, -20), 0.0f); 
  b1->Static();
  
  OBB w2(50.0, 2.0f);
  
  RigidBody* b2 = new RigidBody(w2, Vec2(-30, 20), -15 * RAD); 
  b2->Static();
  
  RigidBody* b3 = new RigidBody(w2, Vec2(20, 10), 15 * RAD); 
  b3->Static();
  
  RigidBody* b4 = new RigidBody(w2, Vec2(-30, 0), -15 * RAD); 
  b4->Static();
  
  RigidBody* b5 = new RigidBody(Circle(2), Vec2(-43.6, 26.6), 0.0f); 
  b5->Dynamic(1.0f);
  b5->angularDamping = 0.1f;
  
  RigidBody* b6 = new RigidBody(OBB(4.0f, 4.0f), Vec2(-48.5, 28.5), -15 * RAD);
  b6->Dynamic(1.0f);
  b6->friction = 0.01f; 
  
  scene.Add(b1);
  scene.Add(b2);
  scene.Add(b3);
  scene.Add(b4);
  scene.Add(b5);
  scene.Add(b6);	
}



void Stacking(void)
{
  RigidBody* b1 = new RigidBody(OBB(110.0, 2.0f), Vec2(0, -20), 0.0f); 
  b1->Static();
  scene.Add(b1);
  
  for(int i = 0; i < 10; i++)
  {
    Circle cir(2);
    OBB obb(4, 4);
    
    RigidBody* b2 = new RigidBody(obb, Vec2(-18.0f, -17.0f + 4.0f * i), 0.0f); 
    b2->Dynamic(1.0f);
    
    RigidBody* b3 = new RigidBody(cir, Vec2(0.0f, -17.0f + 4.0f * i), 0.0f); 
    b3->Dynamic(1.0f);	
    
    RigidBody* b4 = nullptr;
    
    if(i & 1) // (i & 1) == (i % 2 == 0)
    {
      b4 = new RigidBody(obb, Vec2(18.0f, -17.0f + 4.0f * i), 0.0f); 
      b4->Dynamic(1.0f);	
    }
    else
    {
      b4 = new RigidBody(cir, Vec2(18, -17.0f + 4.0f * i), 0.0f); 
      b4->Dynamic(1.0f);	
    }
    
    scene.Add(b2);
    scene.Add(b3);
    scene.Add(b4);
  }
}



void Pyramid(void)
{
  RigidBody* b1 = new RigidBody(OBB(110.0, 2.0f), Vec2(0,-20), 0); 
  b1->Static();
  scene.Add(b1);
  
  int lv = 10; real wh = 4.0f;
  
  for(int i = 0; i < lv; i++)
  {
    for(int j = 0; j < lv - i; j++)
    {
      real x = (j * 2 + i - lv) * wh * 0.5f;
      real y = (i + 0.5f) * wh - 19.0f;
      
      RigidBody* b2 = new RigidBody(OBB(wh, wh), Vec2(x, y) , 0); 
      b2->Dynamic(1.0f);
      scene.Add(b2);
    }
  }
}



void AngryBirds(void)
{
  RigidBody* b1 = new RigidBody(OBB(110.0, 2.0f), Vec2(0, -20), 0.0f); 
  b1->Static();
  
  OBB w2(10.0, 3.0);
  
  RigidBody* b2 = new RigidBody(w2, Vec2(-15, -17.15), 20 * RAD); 
  b2->Static();
  
  RigidBody* b4 = new RigidBody(w2, Vec2(15, -17.15), -20 * RAD); 
  b4->Static();
  
  OBB w3(21.5, 2.0);
  RigidBody* b3 = new RigidBody(w3, Vec2(0, -15), 0.0f); 
  b3->Static();
  
  scene.Add(b1);
  scene.Add(b2);
  scene.Add(b3);
  scene.Add(b4);
  
  int lv = 5, sn = 3;
  
  for(int i = 0; i < sn; i++) // Column
  {
    for(int j = 0; j < lv; j++) // Levels
    {
      OBB wo1(0.8, 5.0);
      OBB wo2(5.0, 0.8);
      
      RigidBody* b5 = new RigidBody(wo1, Vec2(-10 + 8.0 * i, -11.5f + 5.8f * j), 0.0f); 
      b5->Dynamic(1.0f);
      scene.Add(b5);
      
      RigidBody* b6 = new RigidBody(wo1, Vec2(-5.8 + 8.0 * i,-11.5f + 5.8f * j), 0.0f); 
      b6->Dynamic(1.0);
      scene.Add(b6);
      
      RigidBody* b7 = new RigidBody(wo2, Vec2(-7.9 + 8.0 * i, -8.5 + 5.8f * j), 0.0f); 
      b7->Dynamic(1.0f);		
      scene.Add(b7);
    }
  }
}




// this physics engine does not support rotation away from the local center of mass.
// So this is a special scene that was built to roughly rotate 4 OBBs away from the local center of mass.
void RotaryBox(void)
{
  real angularVelocity = RAD * 0.1f / fps; // Angular Velocity
  
  Mat2 rot(w1->orientation);
  
  w1->angularVelocity = w2->angularVelocity = angularVelocity; 
  w3->angularVelocity = w4->angularVelocity = angularVelocity;
  
  w1->velocity = rot.Rotate(Cross(Vec2(20, 0),-angularVelocity));
  w2->velocity = rot.Rotate(Cross(Vec2(20, 0), angularVelocity));
  w3->velocity = rot.Rotate(Cross(Vec2(0, 25), angularVelocity));
  w4->velocity = rot.Rotate(Cross(Vec2(0, 25),-angularVelocity));
}



void FreeStyle(void)
{
  RigidBody* b1 = new RigidBody(OBB(110.0f, 4.0f), Vec2(0, -20), 0.0f);
  b1->Static();
  
  OBB w2(4.0f, 50.0f);
  
  RigidBody* b2 = new RigidBody(w2, Vec2(-60, 4), 10 * RAD);
  b2->Static();
  
  RigidBody* b3 = new RigidBody(w2, Vec2(60, 4), -10 * RAD);
  b3->Static();
  
  scene.Add(b1);
  scene.Add(b2);
  scene.Add(b3);
}


void (*TestScene[])(void) = {LowFriction, Stacking, Pyramid, AngryBirds, RotaryBox, FreeStyle};


char* SceneStrings[] = 
{
  "Scene 1-7: Collision Detection", 
  "Scene 2-7: Low Friction",
  "Scene 3-7: Stacking Boxes & Circles",
  "Scene 4-7: Pyramid", 
  "Scene 5-7: Angry Bird Style",
  "Scene 6-7: Rotary Box",
  "Scene 7-7: Free Style"
};


char* PositionStrings[] = 
{
  "Position Correction: None",
  "Position Correction: Baumgarte Stabilization", 
  "Position Correction: Non-Linear-Gauss-Seidel"
};


static void Start(void)
{
  if(InitScene)
  {
    InitScene = false;
    
    scene.Clear();
    
    sPrincipal->Dynamic(1);
    
    scene.Add(sPrincipal);
    
    if(numScene == 6) // Special Scene 6
    {
      w1->Static();
      w2->Static();
      w3->Static();
      w4->Static();
      
      scene.Add(w1);
      scene.Add(w2);
      scene.Add(w3);
      scene.Add(w4);
    }
    
    if(numScene > 1) TestScene[numScene - 2]();
  }
}



static void Update(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  
  DrawText(2, 16, SceneStrings[numScene - 1]);
  DrawText(2, 44, PositionStrings[Scene::CorrectionType]);
  DrawText(2, 72, pause ? "(P)AUSE: TRUE" : "(P)AUSE: FALSE");
  
  if(numScene == 1)
  {
  	DetectionCollision(sPrincipal);
  }
  else
  {
    if(numScene == 6) RotaryBox();
    
    if(!pause) scene.Step(fps);
    
    for(RigidBody* temp : scene.bodies) temp->shape->DrawShape();
    
    for(auto temp = scene.manifolds.begin(); temp != scene.manifolds.end(); temp++)
    {
      for(int i = 0; i < temp->second.numContacts; i++)
      {
        DrawPoint(temp->second.contacts[i].position, 2, 3);
        DrawPoint(temp->second.contacts[i].warmPoint, 1, 5);
      }
    }
  }
  
  glutSwapBuffers();
}



void MouseMotion(int x, int y)
{
  real _x =  0.05f * (x * 2.0f - glutGet(GLUT_WINDOW_WIDTH));
  real _y = -0.05f * (y * 2.0f - glutGet(GLUT_WINDOW_HEIGHT));
  
  sPrincipal->position.Set(_x, _y);
  sPrincipal->velocity.SetZero();
  sPrincipal->angularVelocity = 0.0f;
}



void Keyboard(unsigned char key, int x, int y)
{
  real _x =  0.05f * (x * 2.0f - glutGet(GLUT_WINDOW_WIDTH));
  real _y = -0.05f * (y * 2.0f - glutGet(GLUT_WINDOW_HEIGHT));
  
  switch (key)
  {
    case 'q': 
    case 'Q': sPrincipal->orientation += PI / 72;
    break;
    
    case 'e': 
    case 'E': sPrincipal->orientation -= PI / 72;
    break;
    
    case 'b': 
    case 'B':
    {
      OBB temp(Vec2(5, 5));
      RigidBody* b = new RigidBody(temp, Vec2(_x, _y), 0);
      b->Dynamic(1.0f);
      scene.Add(b);
    }
    break;
    
    case 'c':
    case 'C':
    {
      Circle temp(2.5f);
      RigidBody* b = new RigidBody(temp, Vec2(_x, _y), 0.0f);
      b->Dynamic(1.0f);
      scene.Add(b);
    }
    break;
    
    case 'w':
    case 'W':
    {
      if(sPrincipal->shape->type == circle)
      { 
        sPrincipal->shape = &bPrincipal;
        bPrincipal.body = sPrincipal;
      }
      else
      { 
        sPrincipal->shape = &cPrincipal;
        cPrincipal.body = sPrincipal;
      }
    }
    break;
    
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    {
      InitScene = true;
      numScene = int(key) - 48; 
      glutPostRedisplay();
    }
    break;
    
    case 'p':
    case 'P':
    pause = !pause;
    break;
    case char(32): // SPACE
    {
      Scene::CorrectionType = Scene::CorrectionType + 1 > 2 ? 0.0f : Scene::CorrectionType + 1; 
    }
    break;
  }
}



void Reshape(int width, int height)
{
  if(height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluPerspective(height * 0.05f, float(width) / height, 0, 1);
  glTranslatef(0.0f, 0.0f, -110);
}



int main(int args, char** argv)
{	
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(900, 700);
  glutCreateWindow("Ciroob Engine V1.1.4");
  glutDisplayFunc(Start);
  glutIdleFunc(Update);
  glutReshapeFunc(Reshape);
  glutMotionFunc(MouseMotion);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();
}
