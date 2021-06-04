#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../src/Object.cpp"
#include "../src/Scene.cpp"

class Coronga : public Object {
public:
    Coronga(){
        int num_vertices = 40;
        float pi = M_PI;
        float angulo = 0.0;

        // Corpo do corona vertices
        for (int i = 0; i < num_vertices; i++){

            angulo += (2.0 * pi) / num_vertices;
            float x = 0.6 * cos(angulo);
            float y = 0.6 * sin(angulo);
            this->insertVertex({x, y, 0.0});
        }
        this->insertBuild({GL_TRIANGLE_FAN, 0, num_vertices, {0.5, 0.25, 0.55, 0.0}});


        float rot_angulo = 0.0;
        for (int i = 1; i <= 8; i++){

            rot_angulo += (2.0 * pi) / 8;
            for (int j = num_vertices * i; j < num_vertices * (i + 1); j++){

                angulo += (2.0 * pi) / num_vertices;
                float x = (0.7 + 0.1 * (i % 2) * (-1)) * cos(rot_angulo) + 0.1 * cos(angulo);
                float y = (0.7 + 0.1 * (i % 2) * (-1)) * sin(rot_angulo) + 0.1 * sin(angulo);
                this->insertVertex({x, y, 0.0});
            }
            this->insertBuild({GL_TRIANGLE_FAN, num_vertices*i, num_vertices, {0.6, 0.15, 0.15, 0.0}});
        }
        
    }
};

class Humano : public Object {
public:
    Humano(){

    // Cabeça
    int num_vertices = 40;
    float pi = M_PI;
    float angulo = 0.0;
    for (int i = 360; i < 360 + num_vertices; i++)
    {
        angulo += (2.0 * pi) / num_vertices;
        float x = 0.2 * cos(angulo);
        float y = 0.2 * sin(angulo);
        this->insertVertex({x, y, 0.0});
    }
    this->insertBuild({GL_TRIANGLE_FAN, 0, num_vertices, {0.6, 0.35, 0.25, 0.0}});

    // Corpo
    this->insertVertex({-0.13, -0.18, 0.0});
    this->insertVertex({-0.15, -0.48, 0.0});
    this->insertVertex({0.13, -0.18, 0.0});
    this->insertVertex({0.15, -0.48, 0.0});
    this->insertBuild({GL_TRIANGLE_STRIP, num_vertices, 4, {1, 1, 0.0, 0}});

    this->insertVertex({-0.145, -0.48, 0.0});
    this->insertVertex({-0.155, -0.58, 0.0});
    this->insertVertex({0.145, -0.48, 0.0});
    this->insertVertex({0.155, -0.58, 0.0});
    this->insertBuild({GL_TRIANGLE_STRIP, num_vertices+4, 4, {0, 0, 1, 0}});

    // Membros
    this->insertVertex({0.13, -0.18, 0.0});
    this->insertVertex({0.2, -0.45, 0.0});

    this->insertVertex({-0.13, -0.18, 0.0});
    this->insertVertex({-0.2, -0.45, 0.0});

    this->insertVertex({-0.1, -0.58, 0.0});
    this->insertVertex({-0.12, -0.7, 0.0});
    
    this->insertVertex({0.1, -0.58, 0.0});
    this->insertVertex({0.12, -0.7, 0.0});
    this->insertBuild({GL_LINES, num_vertices+8, 8, {0.6, 0.35, 0.25, 0.0}});
    }
};

class Seringa : public Object {
public:
    Seringa(){
    this->insertVertex({-0.1,-0.01, 0.0});
    this->insertVertex({-0.1, 0.01, 0.0});
    this->insertVertex({ 0.1,-0.01, 0.0});
    this->insertVertex({ 0.1, 0.01, 0.0});

    this->insertVertex({-0.1 ,-0.01, 0.0});
    this->insertVertex({-0.08,-0.01, 0.0});
    this->insertVertex({-0.1 , 0.8 , 0.0});
    this->insertVertex({-0.08, 0.8 , 0.0});

    this->insertVertex({0.1 ,-0.01, 0.0});
    this->insertVertex({0.08,-0.01, 0.0});
    this->insertVertex({0.1 , 0.8 , 0.0});
    this->insertVertex({0.08, 0.8 , 0.0});
                    
    this->insertVertex({-0.2, 0.8 , 0.0});
    this->insertVertex({-0.2, 0.78, 0.0});
    this->insertVertex({ 0.2, 0.8 , 0.0});
    this->insertVertex({ 0.2, 0.78, 0.0});
                    
    this->insertVertex({-0.08, -0.01, 0.0});
    this->insertVertex({-0.08, 0.49, 0.0});
    this->insertVertex({0.08, -0.01, 0.0});
    this->insertVertex({0.08, 0.49, 0.0});
                    
    this->insertVertex({-0.05, -0.005, 0.0});
    this->insertVertex({ -0.05, -0.055, 0.0});
    this->insertVertex({0.05, -0.005, 0.0});
    this->insertVertex({0.05, -0.055, 0.0});

    this->insertVertex({-0.005, -0.005, 0.0});
    this->insertVertex({-0.005, -0.705, 0.0});
    this->insertVertex({0.005, -0.005, 0.0});
    this->insertVertex({0.005, -0.705, 0.0});
                    
    this->insertVertex({-0.08, 0.49, 0.0});
    this->insertVertex({-0.08, 0.51, 0.0});
    this->insertVertex({0.1, 0.49, 0.0});
    this->insertVertex({0.1, 0.51, 0.0});

    this->insertVertex({-0.03, 0.5, 0.0});
    this->insertVertex({-0.03, 0.98, 0.0});
    this->insertVertex({0.03, 0.5, 0.0});
    this->insertVertex({0.03, 0.98, 0.0});
                    
    this->insertVertex({-0.1, 0.97, 0.0});
    this->insertVertex({-0.1, 0.99, 0.0});
    this->insertVertex({0.1, 0.97, 0.0});
    this->insertVertex({0.1, 0.99, 0.0});


    this->insertBuild({GL_TRIANGLE_STRIP, 16, 4, {0.5, 1.0, 0.8, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 0, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 4, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 8, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 12, 4, {0.5, 0.6, 1.0, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 20, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 24, 4, {0.5, 0.6, 1.0, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 28, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 32, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 36, 4, {0.5, 0.6, 1.0, 1.0}});
    }
};

class Frasco : public Object {
public:
    Frasco(){
    this->insertVertex({-0.1, -0.01, 0.0});
    this->insertVertex({-0.1, 0.04, 0.0});
    this->insertVertex({0.1, -0.01, 0.0});
    this->insertVertex({0.1, 0.04, 0.0});
                    
    this->insertVertex({-0.1, -0.01, 0.0});
    this->insertVertex({-0.1, 0.48, 0.0});
    this->insertVertex({-0.08, -0.01, 0.0});
    this->insertVertex({-0.08, 0.48, 0.0});

    this->insertVertex({0.1, -0.01, 0.0});
    this->insertVertex({0.1, 0.48, 0.0});
    this->insertVertex({0.08, -0.01, 0.0});
    this->insertVertex({0.08, 0.48, 0.0});
                    
    this->insertVertex({-0.1, 0.5, 0.0});
    this->insertVertex({-0.1, 0.45, 0.0});
    this->insertVertex({0.1, 0.5, 0.0});
    this->insertVertex({0.1, 0.45, 0.0});
                    
    this->insertVertex({-0.1, -0.01, 0.0});
    this->insertVertex({-0.1, 0.39, 0.0});
    this->insertVertex({0.1, -0.01, 0.0});
    this->insertVertex({0.1, 0.39, 0.0});
                    
    this->insertVertex({-0.08, 0.5, 0.0});
    this->insertVertex({-0.08, 0.55, 0.0});
    this->insertVertex({0.08, 0.5, 0.0});
    this->insertVertex({ 0.08, 0.55, 0.0});
                    
    this->insertVertex({-0.1, 0.55, 0.0});
    this->insertVertex({-0.1, 0.75, 0.0});
    this->insertVertex({0.1, 0.55, 0.0});
    this->insertVertex({0.1, 0.75, 0.0});

    this->insertBuild({GL_TRIANGLE_STRIP, 16, 4, {0.5, 1.0, 0.8, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 0, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 4, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 8, 4, {0.5, 0.6, 1.0, 1.0}});
    this->insertBuild({GL_TRIANGLE_STRIP, 12, 4, {0.5, 0.6, 1.0, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 20, 4, {0.5, 0.6, 1.0, 1.0}});

    this->insertBuild({GL_TRIANGLE_STRIP, 24, 4, {0.7, 0.9, 1.0, 1.0}});

    }
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void scene_event(GLFWwindow *window, Scene *scene){
    // Evento para selecionar o poligono
    // Pressione E para avançar ou Q para retornar pelo vetor de poligonos
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        scene->nextObject();
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        scene->prevObject();
    }
    // Evento das teclas de translação
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.00, 0.01, 0.00} );
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.00,-0.01, 0.00} );
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.01, 0.00, 0.00} );
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        scene->currentObject()->translate( {-0.01, 0.00, 0.00} );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.00, 0.00, 0.01} );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.00, 0.00,-0.01} );
    }

    // Evento das teclas de rotação
    if(glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.00, 0.00,-0.01 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.00, 0.00, 0.01 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
        scene->currentObject()->rotate( {-0.01, 0.00, 0.00 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.01, 0.00, 0.00 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.00, 0.01, 0.00 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.00,-0.01, 0.00 } );
    }

    // Evento das teclas de escala
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        scene->currentObject()->scale( { 0.05, 0.05, 0.05} );
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        scene->currentObject()->scale( {-0.05,-0.05,-0.05} );
    }
};

// GLSL para Vertex Shader
char* vertex_code =
"attribute vec3 position;                                         \n"
"uniform mat4 mat_transformation;                                 \n"
"void main() {                                                    \n"
"    gl_Position = mat_transformation * vec4(position, 1.0);      \n"
"}                                                                \n";

// GLSL para Fragment Shader
char* fragment_code =
"uniform vec4 color;              \n"
"void main() {                    \n"
"    gl_FragColor = vec4(color);  \n"
"}                                \n";

int main(){
 
    // inicializando o sistema de janelas
    glfwInit();

    // deixando a janela invisível, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
 
    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(800, 800, "Trabalho Prático 01", NULL, NULL);

    // tornando a janela como principal
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    // inicializando Glew (para lidar com funções OpenGL)
    GLint GlewInitResult = glewInit();
    std::cout << "GlewStatus: " << glewGetErrorString(GlewInitResult) << std::endl;

    // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Associando nosso código-fonte GLSL aos slots solicitados
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compilando o Vertex Shader e verificando erros
    glCompileShader(vertex);

    GLint isCompiled = 0;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(vertex, infoLength, NULL, info);

        std::cerr << "Erro de compilação no Vertex Shader." << std::endl;
        std::cerr << "--> " << &info << std::endl;
    }
    

    // Compilando o Fragment Shader e verificando erros
    glCompileShader(fragment);

    isCompiled = 0;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(fragment, infoLength, NULL, info);

        std::cerr << "Erro de compilação no Fragment Shader." << std::endl;
        std::cerr << "--> " << &info << std::endl;
    }

    // Associando os programas compilado ao programa principal
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linkagem do programa e definindo como default
    glLinkProgram(program);
    glUseProgram(program);

    // Adicionando Coronga
    Coronga coronga;
    coronga.translate({-0.3, 0.0, 0.0});
    coronga.scale({-0.8, -0.8, 0.0});

    // Adicionando Humano
    Humano humano;
    humano.translate({0.3, 0.3, 0.0});

    Seringa seringa;
    seringa.translate({0.3, -0.3, 0.0});

    Frasco frasco;
    frasco.translate({0.3, -0.3, 0.0});

    // Vetor de poligonos que serão renderizados
    Scene cena1(program);
    Scene cena2(program);
    cena1.insertObject(coronga);
    cena1.insertObject(humano);
    cena2.insertObject(seringa);
    cena1.insertObject(frasco);

    // Exibindo nossa janela
    glfwShowWindow(window);

    // configurações de profundidade
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);

        scene_event(window, &cena2);
        
        cena1.render();
        cena2.render();

        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}