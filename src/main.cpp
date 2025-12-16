#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model &m, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void funTimer(double desiredFPS, double &t0);

Shaders shaders;
Model triangle;
Model cube;
Model plane;
Model torus;
Model cilindro;
Model esfera;

//functs to draw objects
void drawDrawer( glm::mat4 P, glm::mat4 V, glm::mat4 M, int cajonID);
void drawCajones( glm::mat4 P, glm::mat4 V, glm::mat4 M,float desCajon);
void drawMesita( glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCasa( glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCabina( glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPuerta( glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawVentilador( glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa(  glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(  glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBase( glm::mat4 P, glm::mat4 V, glm::mat4 M);


//CAMARA
glm::vec3 eye    = glm::vec3(-4.7, 4, -4.7);
glm::vec3 centerm= glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up     = glm::vec3(0.0, 1.0, 0.0);
glm::mat4 V = glm::lookAt(eye, centerm, up);

//windows size
int w  =1000;
int h = 1000;

float desZ=0.0;
float desCajon1=0.0;
float desCajon2=0.0;
float desCajon3=0.0;
float rotZ= 0.0;
float rotVentilador=0.0;
 
//Mover cajones
bool abrirCajon1=false;
bool abrirCajon2=false;
bool abrirCajon3=false;

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w ,h , "Proyecto final", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    glfwSetKeyCallback(window, funKey);

 // Entramos en el bucle de renderizado
    double t0 = glfwGetTime();
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
        funTimer(0.01, t0);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0, 1.0);
    glEnable(GL_CULL_FACE);

    shaders.initShaders("resources/shaders/vShader.glsl", "resources/shaders/fShader.glsl");

    triangle.initModel("resources/models/triangle.obj");
    cube.initModel("resources/models/cube.obj");
    plane.initModel("resources/models/plane.obj");
    torus.initModel("resources/models/torus.obj");
    cilindro.initModel("resources/models/cylinder.obj");
    esfera.initModel("resources/models/sphere.obj");
}   

void renderScene() {
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaders.useShaders();

    float fovy = 85.0;
    float nplane = 0.1;
    float fplane = 25.0;

    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy),aspect, nplane, fplane);

    V = glm::lookAt(eye, centerm, up);

    //Dibujamos la escena
    drawCasa(P,V,I);

    //Cajon1
    glm::mat4 T= glm::translate(I, glm::vec3(2.2, 0.0, -3.0));
    glm::mat4 R= glm::rotate(I, glm::radians(-27.0f), glm::vec3(0.0, 1.0, 0.0));
    drawDrawer(P,V,T*R, 1);

    //Cajon2
    T= glm::translate(I, glm::vec3(-3.1, 0.0, -3.6));
    R= glm::rotate(I, glm::radians(65.0f), glm::vec3(0.0, 1.0, 0.0));
    drawDrawer(P,V,T*R,2);

    //Cajon3
    T= glm::translate(I, glm::vec3(-3.1, 0.0, 2.5));
    R= glm::rotate(I, glm::radians(145.0f), glm::vec3(0.0, 1.0, 0.0));
    drawDrawer(P,V,T*R,3);
    
    T = glm::translate(I, glm::vec3(0.0, 4.8, 0.0));
    drawVentilador(P,V,T);

}

void drawObject(Model &m, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM", P*V*M);

    
    glEnable(GL_POLYGON_OFFSET_FILL);
    shaders.setVec3("uColor", color);
    m.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);  
    
    
    shaders.setVec3("uColor", glm::vec3(1.0, 1.0, 1.0));
    m.renderModel(GL_LINE);
    
}


void drawDrawer(glm::mat4 P, glm::mat4 V, glm::mat4 M,int cajonID) {
    drawMesita(P, V, M);
    
    if(cajonID==1 ){
        drawCajones(P, V, M, desCajon1);
    }
    else if(cajonID==2){
        drawCajones(P, V, M, desCajon2);
    }
    else if(cajonID==3){
        drawCajones(P, V, M, desCajon3);
    }
}

void drawCajones(glm::mat4 P, glm::mat4 V, glm::mat4 M, float desCajon) {
    
    
    // Colores
    glm::vec3 colorMadera = glm::vec3(0.6, 0.4, 0.2);
    glm::vec3 colorMetal  = glm::vec3(0.8, 0.8, 0.1); // Dorado/Latón

    //MEDIDAS CAJON
    float posY      = 1.1f;   // A qué altura del suelo está el cajón
    float extension = 0.8f;   // Distancia del centro a las paredes (Tamaño del cajón)
    float alto      = 0.3f;   // Altura de las paredes
    float grosor    = 0.03f;  // Grosor de la madera
    
    // Variables auxiliares para matrices
    glm::mat4 S_pared, T_pared;

    //CAJON DE ARRIBA
    S_pared = glm::scale(I, glm::vec3(extension, alto, grosor));
    // Frente
    T_pared = glm::translate(I, glm::vec3(0.0, posY, extension+ desCajon));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);
    // Atrás
    T_pared = glm::translate(I, glm::vec3(0.0, posY, -extension+ desCajon));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    S_pared = glm::scale(I, glm::vec3(grosor, alto, extension));
    // Derecha
    T_pared = glm::translate(I, glm::vec3(extension, posY, 0.0+ desCajon));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);
    // Izquierda
    T_pared = glm::translate(I, glm::vec3(-extension, posY, 0.0+desCajon));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    //base del cajon
    S_pared = glm::scale(I, glm::vec3(extension, grosor, extension));
    T_pared = glm::translate(I, glm::vec3(0.0, posY - (alto/2) + (grosor/2), 0.0+desCajon));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    //anilla 
    glm::mat4 S_anilla = glm::scale(I, glm::vec3(0.15, 0.15, 0.03)); 
    glm::mat4 T_anilla = glm::translate(I, glm::vec3(0 ,posY,extension+0.05+desCajon)); 
    glm::mat4 R_anilla = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    drawObject(torus, colorMetal, P, V, M * T_anilla * S_anilla); 


    //CAJON DE ABAJO
    posY      = 0.5f;   // A qué altura del suelo está el cajón
    S_pared = glm::scale(I, glm::vec3(extension, alto, grosor));
    // Frente
    T_pared = glm::translate(I, glm::vec3(0.0, posY, extension));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);
    // Atrás
    T_pared = glm::translate(I, glm::vec3(0.0, posY, -extension));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    S_pared = glm::scale(I, glm::vec3(grosor, alto, extension));
    // Derecha
    T_pared = glm::translate(I, glm::vec3(extension, posY, 0.0));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);
    // Izquierda
    T_pared = glm::translate(I, glm::vec3(-extension, posY, 0.0));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    //base del cajon
    S_pared = glm::scale(I, glm::vec3(extension, grosor, extension));
    T_pared = glm::translate(I, glm::vec3(0.0, posY - (alto/2) + (grosor/2), 0.0));
    drawObject(cube, colorMadera, P, V, M * T_pared * S_pared);

    //anilla 
    T_anilla = glm::translate(I, glm::vec3(0 ,posY,extension+0.05)); // Debajo del soporte
    drawObject(torus, colorMetal, P, V, M * T_anilla * S_anilla); 
}
  
void drawMesita(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Tabla superior
    glm::mat4 S_tapa = glm::scale(I, glm::vec3(1.0, 0.05, 1.0));
    glm::mat4 T_tapa = glm::translate(I, glm::vec3(0.0, 1.5, 0.0));
    drawObject(cube, glm::vec3(0.4, 0.2, 0.0), P, V, M * T_tapa * S_tapa);

    // Patas
    glm::mat4 S_pata = glm::scale(I, glm::vec3(0.1, 0.75, 0.1));
    glm::vec3 posPatas[4] = {
        glm::vec3(0.9, 0.75,  0.9),
        glm::vec3( 0.9, 0.75, -0.9),
        glm::vec3(-0.9, 0.75,  0.9),
        glm::vec3(-0.9, 0.75, -0.9)
    };
    for(int i=0; i<4; i++) {
        drawObject(cube, glm::vec3(0.4, 0.2, 0.0), P, V, M * glm::translate(I, posPatas[i]) * S_pata);
    }
}

void drawCasa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    drawCabina(P,V,M);
    drawPuerta(P,V,M);
}
void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
   // Escala del plano para cubrir el semi-lado
    const float ESCALA_X = 5.0f; 
    const float ESCALA_Y_SUELO = 0.1f;
    const float ESCALA_Z = 5.0f; 
    const float ESCALA_Z_PARED_FRONT= ESCALA_Z/2.0;
    const float ESCALA_X_PARED_LATERAL = ESCALA_X/2.0;
    
    // Posiciones de borde (La habitación tiene 10x10)
    const float POS_BORDE_X = 5.0f;
    const float POS_BORDE_Z = 5.0f;
    
    // Alturas
    const float ALTURA_HABITACION = 5.0f; // Usamos POS_TECHO_Y como altura total
    const float POS_PARED_Y_CENTRO = ALTURA_HABITACION / 2.0f; // 2.5 (Nuevo centro vertical)
    const float POS_TECHO_Y = 5.0f; 

    // Dimensión de la pared lateral
    const float ESCALA_LATERAL_REDUCIDA = ESCALA_Z / 2.0f; // 5.0 / 2.0 = 2.5
    
    // Ángulos de rotación
    const float ANG_90 = 90.0f;
    const float ANG_180 = 180.0f;
    
    // Colores
    const glm::vec3 COLOR_SUELO = glm::vec3(0.3, 0.3, 0.3);
    const glm::vec3 COLOR_PARED = glm::vec3(0.6, 0.6, 0.6);

    // Variables de trabajo
    glm::mat4 S;
    glm::mat4 T;
    glm::mat4 R;
    
    // --- SUELO ---
    S = glm::scale(I, glm::vec3(ESCALA_X, ESCALA_Y_SUELO, ESCALA_Z));
    T = glm::translate(I, glm::vec3(0.0, 0.0, 0.0));
    drawObject(plane, COLOR_SUELO, P, V, M * T * S);
    
    // --- PARED IZQUIERDA ---
    S = glm::scale(I, glm::vec3(ESCALA_X_PARED_LATERAL, ESCALA_Y_SUELO, ESCALA_Z));
    R = glm::rotate(I, glm::radians(-ANG_90), glm::vec3(0.0, 0.0, 1.0));
    T = glm::translate(I, glm::vec3(-POS_BORDE_X, POS_PARED_Y_CENTRO, 0.0)); // Y=2.5
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S);
    
    // --- PARED DERECHA ---
    const float POS_HUECO_Z_OFFSET = ESCALA_Z_PARED_FRONT / 2.0f;

    glm::mat4 S_pared_derecha_hueco = glm::scale(I, glm::vec3(ESCALA_X_PARED_LATERAL, ESCALA_Y_SUELO, POS_HUECO_Z_OFFSET)); 
    R = glm::rotate(I, glm::radians(ANG_90), glm::vec3(0.0, 0.0, 1.0));
    T = glm::translate(I, glm::vec3(POS_BORDE_X, POS_PARED_Y_CENTRO, - POS_PARED_Y_CENTRO - POS_HUECO_Z_OFFSET)); 
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S_pared_derecha_hueco);
    T = glm::translate(I, glm::vec3(POS_BORDE_X, POS_PARED_Y_CENTRO, POS_HUECO_Z_OFFSET + POS_PARED_Y_CENTRO));
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S_pared_derecha_hueco);

    const float ALTURA_DINTEL = ESCALA_Z_PARED_FRONT / 3.0f; 
    S = glm::scale(I, glm::vec3(ALTURA_DINTEL, ESCALA_Y_SUELO, ESCALA_Z- (2*POS_HUECO_Z_OFFSET)));
    R = glm::rotate(I, glm::radians(ANG_90), glm::vec3(0.0, 0.0, 1.0));
    T = glm::translate(I, glm::vec3(POS_BORDE_X, ALTURA_HABITACION - ALTURA_DINTEL , 0.0));
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S);
    T = glm::translate(I, glm::vec3(POS_BORDE_X,ALTURA_DINTEL, 0.0));
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S);


    // --- PARED FONDO (Centro Y = 2.5) ---
    S = glm::scale(I, glm::vec3(ESCALA_X, ESCALA_Y_SUELO, ESCALA_Z_PARED_FRONT));
    R = glm::rotate(I, glm::radians(ANG_90), glm::vec3(1.0, 0.0, 0.0));
    T = glm::translate(I, glm::vec3(0.0, POS_PARED_Y_CENTRO, -POS_BORDE_Z));
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S);
    
    // --- TECHO ---
    S = glm::scale(I, glm::vec3(ESCALA_X, ESCALA_Y_SUELO, ESCALA_Z));
    R = glm::rotate(I, glm::radians(ANG_180), glm::vec3(1.0, 0.0, 0.0));
    T = glm::translate(I, glm::vec3(0.0, POS_TECHO_Y, 0.0));
    drawObject(plane, COLOR_PARED, P, V, M * T * S);

    // --- PARED FRENTE (Centro Y = 2.5) ---
    R = glm::rotate(I, glm::radians(-ANG_90), glm::vec3(1.0, 0.0, 0.0));
    
    // Piezas laterales de la puerta
    const float ESCALA_PUERTA_X = ESCALA_X / 3.0f;
    const float POS_PUERTA_X = POS_BORDE_X / 1.5f;

    S= glm::scale(I, glm::vec3(ESCALA_PUERTA_X, ESCALA_Y_SUELO, ESCALA_Z_PARED_FRONT)); 
    T = glm::translate(I, glm::vec3(POS_PUERTA_X, POS_PARED_Y_CENTRO, POS_BORDE_Z));
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S); 
    T = glm::translate(I, glm::vec3(-POS_PUERTA_X, POS_PARED_Y_CENTRO, POS_BORDE_Z)); 
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S); 

    //encima de la puerta
    const float ESCALA_DINTEL_Z = ESCALA_Z_PARED_FRONT / 6.0f;
    S= glm::scale(I, glm::vec3(ESCALA_PUERTA_X, ESCALA_Y_SUELO, ESCALA_DINTEL_Z));
    T = glm::translate(I, glm::vec3(0, POS_TECHO_Y - (ESCALA_Z_PARED_FRONT/6), POS_BORDE_Z)); 
    drawObject(plane, COLOR_PARED, P, V, M * T * R * S);
}

void drawPuerta(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S_puerta = glm::scale(I, glm::vec3(1.7, 2.1, 0.1));
    glm::mat4 T_puerta = glm::translate(I, glm::vec3(0.0, (5-(5/6)) /2 +0.1 , 5.01));
    drawObject(cube, glm::vec3(0.5, 0.2, 0.0), P, V, M * T_puerta * S_puerta);

    ///pomo interior
    glm::mat4 S_pomo = glm::scale(I, glm::vec3(0.2, 0.2, 0.03));
    glm::mat4 T_pomo = glm::translate(I, glm::vec3(-0.9, 2.5 , 4.9));
    drawObject(torus, glm::vec3(0.8, 0.8, 0.1), P, V, M * T_pomo * S_pomo);
}   


void drawVentilador( glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 R = glm::rotate(I, glm::radians(rotVentilador), glm::vec3(0.0, 1.0, 0.0));
    drawHelice(P,V,M*R);
    drawBase(P,V,M);
}

void drawAspa(  glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    float tamanoAspa = 1.1;
    glm::mat4 T = glm::translate(I, glm::vec3(tamanoAspa, 0.0, 0.0));
    glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.09, tamanoAspa, 0.3));
    drawObject(cilindro, glm::vec3(1.0, 0.0, 0.0), P,V, M*T*R*S);
}

void drawHelice(  glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 Rz = glm::rotate(I, glm::radians(360.0f/5.0f), glm::vec3(0.0, 1.0, 0.0));
    drawAspa(P,V,M);
    drawAspa(P,V,M*Rz);
    drawAspa(P,V,M*Rz*Rz);
    drawAspa(P,V,M*Rz*Rz*Rz);
    drawAspa(P,V,M*Rz*Rz*Rz*Rz);
}
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.15, 0.3, 0.15));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.19, 0.0));
    drawObject(cilindro, glm::vec3(0.5, 0.5, 0.5), P, V, M*T*S);

    T = glm::translate(I, glm::vec3(0.0, -0.15, 0.0));
    S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
    drawObject(esfera, glm::vec3(0.5, 0.5, 0.5), P, V, M * T*S);
}


void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key){
        case GLFW_KEY_UP:
            if(abrirCajon1){
                if(desCajon1>0.1)  desCajon1-= 0.05;
            }else if(abrirCajon2){
                if(desCajon2>0.1)  desCajon2-= 0.05;
            }else if(abrirCajon3){
                if(desCajon3>0.1)  desCajon3-= 0.05;
            }
            break;
        case GLFW_KEY_DOWN:
            if(abrirCajon1){
                if(desCajon1<0.9)  desCajon1+= 0.05;
            }else if(abrirCajon2){
                if(desCajon2<0.9)  desCajon2+= 0.05;
            }else if(abrirCajon3){
                if(desCajon3<0.9)  desCajon3+= 0.05;
            }
            break;
        case GLFW_KEY_2: //camara principal
            abrirCajon1=false;
            abrirCajon2=false;
            abrirCajon3=false;
            eye    = glm::vec3(-4.7, 4.0 , -4.7);
            centerm= glm::vec3(0.0, 0.0, 0.0);
            up     = glm::vec3(0.0, 1.0, 0.0);
            break;
        case GLFW_KEY_3: //cajon1
            abrirCajon2=false;
            abrirCajon3=false;
            abrirCajon1=true;
            eye    = glm::vec3(1.4, 2.0, -1.4);
            centerm= glm::vec3(2.2, 0.0, -3.0);
            up     = glm::vec3(0.0, 1.0, 0.0);
            break;
        case GLFW_KEY_4: //cajon2
            abrirCajon1=false;
            abrirCajon3=false;
            abrirCajon2=true;
            eye    = glm::vec3(-1.5, 2.0, -1.5);
            centerm= glm::vec3(-3.1, 0.0, -3.6);
            up     = glm::vec3(0.0, 1.0, 0.0);
            break;
        case GLFW_KEY_5: //cajon3
            abrirCajon1=false;
            abrirCajon2=false;  
            abrirCajon3=true;
            eye    = glm::vec3(-2.1, 2.4, 1.1);
            centerm= glm::vec3(-3.1, 0.0, 2.5);
            up     = glm::vec3(0.0, 1.0, 0.0);
            break;
        case GLFW_KEY_1: //mirar puerta
            abrirCajon1=false;
            abrirCajon2=false;
            abrirCajon3=false;
            eye    = glm::vec3(0.0, 3, 1.0);
            centerm= glm::vec3(0.0, 2.5, 2.5);
            up     = glm::vec3(0.0, 1.0, 0.0);
            break;

    }
}

void funTimer(double desiredFPS, double &t0) {
    double t1 = glfwGetTime();
    double dt = t1 - t0;
    if (dt > desiredFPS) {
        rotVentilador += 3.6;
        t0 = t1;
    }
}