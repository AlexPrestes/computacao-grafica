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

// Classe Poligonos
// classe que abstai qualquer desenho/objeto/poligono
class Polygon {
    std::vector<vertex> _vertices;
    std::vector<float> _mat_scale;
    std::vector<float> _vec_rotate;
    std::vector<float> _mat_rotate;
    std::vector<float> _mat_translate;
    build _build;
    GLint _program;
public:
    Polygon(const GLint program){
        _program = program;
        _mat_scale = {      1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 };

        _vec_rotate = { 0.0, 0.0, 0.0 };

        _mat_rotate = {     1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 };

        _mat_translate = {  1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 };
        };
    // loadBuffer: recarrega o buffer para cada objeto 
    void loadBuffer(){
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(std::vector<vertex>), &_vertices[0], GL_DYNAMIC_DRAW);

        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        GLint loc = glGetAttribLocation(_program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]), (void*) 0);

        std::vector<float> mat_transformation;
        mat_transformation = matmul(_mat_translate, _mat_rotate);
        mat_transformation = matmul(mat_transformation, _mat_scale);

        GLint loc_mat = glGetUniformLocation(_program, "mat_transformation");
        glUniformMatrix4fv(loc_mat, 1, GL_TRUE, &mat_transformation[0]);

        // Associando variáveis do programa GLSL (Fragment Shaders) com nossos dados
        GLint loc_color = glGetUniformLocation(_program, "color");
        glUniform4f(loc_color, _build.color[0], _build.color[1], _build.color[2], _build.color[3]);
    };
    
    // inseri um novo vertice ao objeto
    void insertVertex(vertex vertice){
        _vertices.push_back(vertice);
    };
    
    // define as configurações de renderizaçao dos vertices
    void setBuild(build construcao){
        _build = construcao;
    };
    
    // renderiza os vertices
    void draw(){
        loadBuffer();
        glDrawArrays(_build.mode, _build.first, _build.count);
    };
    
    // define a cor
    void setColor(std::vector<GLfloat> _color){
        _build.color = _color;
    };
    
    // metodo de translação do poligonos
    void translate(std::vector<float> vt_translate){
        this->_mat_translate[3]  += vt_translate[0];
        this->_mat_translate[7]  += vt_translate[1];
        this->_mat_translate[11] += vt_translate[2];
    };
    
    // metodo de rotação por referencia
    void rotate(std::vector<float> vt_rotate){
        for(int i = 0; i < 4; i++){
            this->_vec_rotate[i] += vt_rotate[i];
        }
        float cos = std::cos(this->_vec_rotate[0]), sin = std::sin(this->_vec_rotate[0]);
        this->_mat_rotate[0] =  cos;
        this->_mat_rotate[1] = -sin;
        this->_mat_rotate[4] =  sin;
        this->_mat_rotate[5] =  cos;
    };
    
    // metodo de escala por referencia
    void scale(std::vector<float> vt_scale){
        this->_mat_scale[0]  += vt_scale[0];
        this->_mat_scale[5]  += vt_scale[1];
        this->_mat_scale[10] += vt_scale[2];
    };

    std::vector<float> matmul(const std::vector<float> A,const std::vector<float> B){
        std::vector<float> C(16);
        for(int i = 0; i < 4; i++){
            for(int k = 0; k < 4; k++){
                for(int j = 0; j < 4; j++){
                    C[4*i + k] += A[4*i + j] * B[4*j + k];
                }
            }
        }
        return C;
    }
};

// classe triangulo: popula os vertices de um triangulo equilatero
class Triangle : public Polygon {

public:
    Triangle(const GLint program, float l) : Polygon(program){
        float h = l*std::sqrt(3)/2;
        this->insertVertex({ 0.0, 2*h/3});
        this->insertVertex({ l/2, -h/3 });
        this->insertVertex({-l/2, -h/3 });
        this->setBuild({GL_TRIANGLE_STRIP, 0, 3, {0.0, 0.0, 0.0, 1.0}});
    };
};

// classe retangulo: popula os vertices de um retangulo ou quadrado
class Rectangle : public Polygon {

public:
    Rectangle(const GLint program, float b, float h) : Polygon(program){
    this->insertVertex({-b/2,  h/2});
    this->insertVertex({-b/2, -h/2});
    this->insertVertex({ b/2,  h/2});
    this->insertVertex({ b/2, -h/2});
    this->setBuild({GL_TRIANGLE_STRIP, 0, 4, {0.0, 0.0, 0.0, 1.0}});
    };
};

// classe circulo: popula os vertices de um circulo
class Circle : public Polygon {

public:
    Circle(const GLint program, float radius) : Polygon(program){
    int num_vertices = 32;
    float pi = M_PI;
    float angle = 0.0;
    float x, y;
    for(int i=0; i < num_vertices; i++){
	    angle += (2.0*pi)/num_vertices;
	    x = cos(angle)*radius;
	    y = sin(angle)*radius;
	    this->insertVertex({x, y});
    }
    this->setBuild({GL_TRIANGLE_FAN, 0, num_vertices, {0.0, 0.0, 0.0, 1.0}});
    };
};

// GLSL para Vertex Shader
char* vertex_code =
"attribute vec2 position;                                         \n"
"uniform mat4 mat_transformation;                                 \n"
"void main() {                                                    \n"
"    gl_Position = mat_transformation * vec4(position, 0.0, 1.0); \n"
"}                                                                \n";

// GLSL para Fragment Shader
char* fragment_code =
"uniform vec4 color;              \n"
"void main() {                    \n"
"    gl_FragColor = vec4(color);  \n"
"}                                \n";

int key_event(GLFWwindow *window, std::vector<Polygon> &objetos, int idx_obj){
        // ponteiro para o poligono selecionado
        Polygon *obj_select = &objetos[idx_obj];

        glfwPollEvents();

        // Evento para fechar a janela pressionando ESC
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Evento para selecionar o poligono
        // Pressione E para avançar ou Q para retornar pelo vetor de poligonos
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            idx_obj++;
            idx_obj=idx_obj%objetos.size();

            obj_select = &objetos[idx_obj];
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            idx_obj--;
            idx_obj = (idx_obj<0) ? objetos.size()-1 : idx_obj;

            obj_select = &objetos[idx_obj];
        }
        // Evento das teclas de translação
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            obj_select->translate( { 0.00, 0.01, 0.00} );
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            obj_select->translate( { 0.00,-0.01, 0.00} );
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            obj_select->translate( { 0.01, 0.00, 0.00} );
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            obj_select->translate( {-0.01, 0.00, 0.00} );
        }

        // Evento das teclas de rotação
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            obj_select->rotate( { 0.01 } );
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            obj_select->rotate( { -0.01 } );
        }

        // Evento das teclas de escala
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            obj_select->scale( { 0.05, 0.05 } );
        }
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            obj_select->scale( {-0.05,-0.05 } );
        }

        return idx_obj;
};

int main(){
 
    // inicializando o sistema de janelas
    glfwInit();

    // deixando a janela invisível, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
 
    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercicio 02", NULL, NULL);

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
    Triangle triangulo(program, 0.2);
    triangulo.setColor({0.0, 0.15, 0.25, 1.0});
 
    // Adicionando Quadrado
    Rectangle quadrado(program, 0.1, 1.0);
    quadrado.setColor({0.3, 0.15, 0.25, 1.0});

    // Adicionando Circulo
    Circle circulo(program, 0.25);
    circulo.setColor({1.0, 0.55, 0.25, 1.0});

    // Adicionando um objeto estranho
    Polygon novo_obj(program);
    novo_obj.insertVertex({ 0.0, 0.5 });
    novo_obj.insertVertex({ 0.1, 0.1 });
    novo_obj.insertVertex({ 0.5, 0.2 });
    novo_obj.insertVertex({ 0.5,-0.2 });
    novo_obj.insertVertex({ 0.0,-0.5 });
    novo_obj.insertVertex({-0.5,-0.2 });
    novo_obj.insertVertex({-0.5, 0.2 });
    novo_obj.setBuild({GL_TRIANGLE_STRIP, 0, 7, {0.6, 0.4, 0.7, 1.0}});

    // Vetor de poligonos que serão renderizados
    std::vector<Polygon> objetos = {circulo, quadrado, triangulo, novo_obj};
    // indice do poligono selecionado
    int idx_obj = 0;

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window)){

        idx_obj = key_event(window, objetos, idx_obj);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);

        // Loop que renderiza os poligonos
        for(auto obj : objetos){
            obj.draw();
        }

        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
