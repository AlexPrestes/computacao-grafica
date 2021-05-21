#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../src/Object.cpp"
#include "../src/Scene.cpp"

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
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercício Prático 03", NULL, NULL);

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
 
    // Adicionando Cubo
    Cube cubo(0.4);
    cubo.translate({0.3, 0.0, 0.0});

    // Adicionando Piramide
    Pyramid piramide(0.4);
    piramide.translate({-0.3, 0.0, 0.0});

    // Vetor de poligonos que serão renderizados
    Scene cena(program);
    cena.insertObject(piramide);
    cena.insertObject(cubo);

    // Exibindo nossa janela
    glfwShowWindow(window);

    // configurações de profundidade
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);

        scene_event(window, &cena);
        
        cena.render();

        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}