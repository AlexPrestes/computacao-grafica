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

class Matrix {
    std::vector<float> _storage;
    size_t _nrows, _ncols;

public:
    Matrix(size_t nrows = 0, size_t ncols = 0)
        : _storage(nrows * ncols), _nrows(nrows), _ncols(ncols) {}

    size_t nrows() const { return _nrows; }

    size_t ncols() const { return _ncols; }

    float &operator()(size_t i, size_t j){
        return _storage[i * _ncols + j];
    };

    float const &operator()(size_t i, size_t j) const {
        return _storage[i * _ncols + j];
    };

    void setValue(std::vector<float> const &v){
        _storage = v;
    };

    Matrix operator*(Matrix const &a){
        Matrix c(this->_nrows, a.ncols());
        for(auto i = 0; i < this->_nrows; i++){
            for(auto k = 0; k < this->_ncols; k++){
                for(auto j = 0; j < a.ncols(); j++){
                    c(i,j) += this->_storage[i*_ncols +k]*a(k,j);
                }
            }
        }

        return c;
    };
};

class Polygon {
    std::vector<vertex> _vertices;
    std::vector<vertex> _vref;
    build _build;
    GLint _program;
public:
    Polygon(const GLint program){
        _program = program;
        };
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

        // Associando variáveis do programa GLSL (Fragment Shaders) com nossos dados
        GLint loc_color = glGetUniformLocation(_program, "color");
        glUniform4f(loc_color, _build.color[0], _build.color[1], _build.color[2], _build.color[3]);
    };
    void insertVertex(vertex vertice){
        _vertices.push_back(vertice);
        _vref.push_back(vertice);
    };
    void setBuild(build construcao){
        _build = construcao;
    };
    void draw(){
        loadBuffer();
        glDrawArrays(_build.mode, _build.first, _build.count);
    };
    void setColor(std::vector<GLfloat> _color){
        _build.color = _color;
    };
    void translate(std::vector<float> offset){
        Matrix translacao(3,3);
        Matrix v(3,1);

        translacao.setValue({ 1, 0, offset[0],
                              0, 1, offset[1],
                              0, 0, 1 });

        for(int i = 0; i < _vertices.size(); i++){
            v.setValue({_vertices[i].x, _vertices[i].y, 1});
            v = translacao*v;
            _vertices[i].x = v(0,0);
            _vertices[i].y = v(1,0);
        };
    };
    void rotate(float theta){
        Matrix translacao1(3,3);
        Matrix rotacao(3,3);
        Matrix translacao2(3,3);
        Matrix v(3,1);


        rotacao.setValue({ std::cos(theta), -std::sin(theta), 0,
                           std::sin(theta),  std::cos(theta), 0, 
                                         0,                0, 1});

        for(int i = 0; i < _vertices.size(); i++){

            translacao1.setValue({  1, 0, -(_vertices[i].x-_vref[i].x),
                                    0, 1, -(_vertices[i].y-_vref[i].y),
                                    0, 0, 1 });
            translacao2.setValue({  1, 0, (_vertices[i].x-_vref[i].x),
                                    0, 1, (_vertices[i].y-_vref[i].y),
                                    0, 0, 1 });

            v.setValue({_vertices[i].x, _vertices[i].y, 1});

            v = rotacao*translacao1*v;

            _vref[i].x = v(0,0);
            _vref[i].y = v(1,0);

            v = translacao2*v;

            _vertices[i].x = v(0,0);
            _vertices[i].y = v(1,0);
        };
    };
    void scale(std::vector<float> coefficient){
        Matrix translacao1(3,3);
        Matrix escala(3,3);
        Matrix translacao2(3,3);
        Matrix v(3,1);


        escala.setValue({ coefficient[0],               0, 0,
                                       0,  coefficient[1], 0, 
                                       0,               0, 1});

        for(int i = 0; i < _vertices.size(); i++){

            translacao1.setValue({  1, 0, -(_vertices[i].x-_vref[i].x),
                                    0, 1, -(_vertices[i].y-_vref[i].y),
                                    0, 0, 1 });
            translacao2.setValue({  1, 0, (_vertices[i].x-_vref[i].x),
                                    0, 1, (_vertices[i].y-_vref[i].y),
                                    0, 0, 1 });

            v.setValue({_vertices[i].x, _vertices[i].y, 1});

            v = escala*translacao1*v;

            _vref[i].x = v(0,0);
            _vref[i].y = v(1,0);

            v = translacao2*v;

            _vertices[i].x = v(0,0);
            _vertices[i].y = v(1,0);
        };
    };
};

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

    std::vector<Polygon> objetos = {circulo, quadrado, triangulo, novo_obj};
    int idx_obj = 0;
    Polygon *obj_select = &objetos[idx_obj];

    std::vector<bool> key_buffer(100);

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }


        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && key_buffer[GLFW_KEY_E] == false){
            key_buffer[GLFW_KEY_E] = true;
            idx_obj++;
            idx_obj=idx_obj%objetos.size();

            obj_select = &objetos[idx_obj];
        }else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && key_buffer[GLFW_KEY_E] == true){
            key_buffer[GLFW_KEY_E] = false;
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && key_buffer[GLFW_KEY_Q] == false){
            key_buffer[GLFW_KEY_Q] = true;
            idx_obj--;
            idx_obj = (idx_obj<0)?objetos.size()-1:idx_obj;

            obj_select = &objetos[idx_obj];
        }else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && key_buffer[GLFW_KEY_Q] == true){
            key_buffer[GLFW_KEY_Q] = false;
        }


        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            obj_select->translate({0.0, 0.01});
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            obj_select->translate({0.0, -0.01});
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            obj_select->translate({0.01, 0.0});
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            obj_select->translate({-0.01, 0.0});
        }


        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            obj_select->rotate(-M_PI/24);
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            obj_select->rotate( M_PI/24);
        }


        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            obj_select->scale({1.1, 1.1});
        }
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            obj_select->scale({0.9, 0.9});
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);

        for(auto obj : objetos){
            obj.draw();
        }

        glfwSwapBuffers(window);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
