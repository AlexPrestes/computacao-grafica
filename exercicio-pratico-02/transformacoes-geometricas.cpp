#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
 

// variaveis globais
struct vertex {
    float x, y;
};

struct build {
    GLenum mode;
    GLint first;
    GLsizei count;
    std::vector<GLfloat> color;
};

class Polygon {
    std::vector<vertex> _vertices;
    build _build;
    GLint _program;
public:
    Polygon(const GLint program){
        _program = program;
        };
    void LoadBuffer(){
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(std::vector<vertex>), &_vertices[0], GL_DYNAMIC_DRAW);

        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        GLint loc = glGetAttribLocation(_program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]), (void*) 0);

        // Associando variáveis do programa GLSL (Fragment Shaders) com nossos dados
        GLint loc_color = glGetUniformLocation(_program, "color");
        glUniform4f(loc_color, _build.color[0], _build.color[1], _build.color[2], _build.color[3]);
    };
    void InsertVertex(vertex vertice){
        _vertices.push_back(vertice);
    };
    void SetBuild(build construcao){
        _build = construcao;
    };
    void Draw(){
        LoadBuffer();
        glDrawArrays(_build.mode, _build.first, _build.count);
    };
    void SetColor(std::vector<GLfloat> _color){
        _build.color = _color;
    };
};

// GLSL para Vertex Shader
char* vertex_code =
"attribute vec2 position;                     \n"
"void main() {                                \n"
"    gl_Position = vec4(position, 0.0, 1.0);  \n"
"}                                            \n";

// GLSL para Fragment Shader
char* fragment_code =
"uniform vec4 color;                          \n"
"void main() {                                \n"
"    gl_FragColor = vec4(color);              \n"
"}                                            \n";


int main(){
 
    // inicializando o sistema de janelas
    glfwInit();

    // deixando a janela invisível, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
 
    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercicio 01", NULL, NULL);

    // tornando a janela como principal 
    glfwMakeContextCurrent(window);

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
 
    // Adicionando Triangulo
    Polygon triangulo(program);
    triangulo.InsertVertex({ 0.0,  0.1});
    triangulo.InsertVertex({-0.1, -0.1});
    triangulo.InsertVertex({ 0.1, -0.1});
    triangulo.SetBuild({GL_TRIANGLE_STRIP, 0, 3, {0.0, 1.0, 0.0, 1.0}});
 
    // Adicionando Quadrado
    Polygon quadrado(program);
    quadrado.InsertVertex({-0.1,  0.1});
    quadrado.InsertVertex({-0.1, -0.1});
    quadrado.InsertVertex({ 0.1,  0.1});
    quadrado.InsertVertex({ 0.1, -0.1});
    quadrado.SetBuild({GL_TRIANGLE_STRIP, 0, 4, {0.0, 1.0, 1.0, 1.0}});

    // Adicionando Circulo
    Polygon circulo(program);
    int num_vertices = 32;
    float pi = 3.14;
    float radius = 0.25;
    float angle = 0.0;
    float x, y;
    for(int i=0; i < num_vertices; i++){
	    angle += (2.0*pi)/num_vertices;
	    x = cos(angle)*radius;
	    y = sin(angle)*radius;
	    circulo.InsertVertex({x, y});
    }
    circulo.SetBuild({GL_TRIANGLE_FAN, 0, num_vertices, {0.0, 0.15, 0.15, 1.0}});

    Polygon linha(program);
    linha.InsertVertex({ 0.75, -0.25});
    linha.InsertVertex({-0.75, -0.25});
    linha.SetBuild({GL_LINES, 0, 2, {0.0, 0.0, 0.0, 1.0}});

    Polygon ponto(program);
    ponto.InsertVertex({ 0.75, 0.0});
    ponto.InsertVertex({-0.75, 0.0});
    ponto.SetBuild({GL_POINTS, 0, 2, {0.20, 0.20, 0.20, 1.0}});

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);

        circulo.Draw();
        quadrado.Draw();
        triangulo.Draw();
        linha.Draw();
        ponto.Draw();

        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
