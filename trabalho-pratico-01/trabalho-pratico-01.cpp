#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// variaveis globais
struct vertex {
    float x, y, z;
};

struct build {
    GLenum mode;
    GLint first;
    GLsizei count;
    std::vector<float> color;
};

// Classe Poligonos
// classe que abstai qualquer desenho/objeto/poligono
class Object {
    std::vector<vertex> _vertices;
    std::vector<float> _vec_scale;
    std::vector<float> _vec_rotate;
    std::vector<float> _vec_translate;
    std::vector<float> _mat_transformation;
    std::vector<build> _build;
public:
    Object(){
        _vec_scale  = { 1.0, 1.0, 1.0 };

        _vec_rotate = { 0.0, 0.0, 0.0 };

        _vec_translate  = { 0.0, 0.0, 0.0 };

        _mat_transformation = {  1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1 };
        };
    
    // inseri um novo vertice ao objeto
    void insertVertex(vertex vertice){
        _vertices.push_back(vertice);
    };
    void insertVertex(std::vector<vertex> vertices){
        _vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
    };

    std::vector<vertex> getVertex(){
        return _vertices;
    }

    void setColor(std::vector<GLfloat> color){
        for(int i = 0; i < _build.size(); i++){
            this->_build[i].color.clear();
            this->_build[i].color.insert(this->_build[i].color.end(), color.begin(), color.end());
        }
    }
    // define as configurações de renderização dos vertices
    void insertBuild(build construcao){
        _build.insert(_build.end(), construcao);
    };
    void insertBuild(std::vector<build> construcoes){
        _build.insert(_build.end(), construcoes.begin(), construcoes.end());
    };
    std::vector<build> getBuild(){
        return _build;
    }
    
    // metodo de translação do poligonos
    void translate(std::vector<float> vt_translate){
        for(int i = 0; i < 3; i++){
            this->_vec_translate[i] += vt_translate[i];
        }
        this->setTransformationMatrix();
    };
    
    // metodo de rotação por referencia
    void rotate(std::vector<float> vt_rotate){
        for(int i = 0; i < 3; i++){
            this->_vec_rotate[i] += vt_rotate[i];
        }
        this->setTransformationMatrix();
    };
    
    // metodo de escala por referencia
    void scale(std::vector<float> vt_scale){
        for(int i = 0; i < 3; i++){
            this->_vec_scale[i] += vt_scale[i];
        }
        this->setTransformationMatrix();
    };

    void setTransformationMatrix(){
        std::vector<float> mat_transformation(16), mat_scale(16), mat_rotate_x(16),
        mat_rotate_y(16), mat_rotate_z(16), mat_translate(16);
        float cos, sin;

        cos = std::cos(this->_vec_rotate[2]), sin = std::sin(this->_vec_rotate[2]);
        mat_rotate_z = {  cos, -sin, 0, 0,
                          sin,  cos, 0, 0,
                            0,    0, 1, 0,
                            0,    0, 0, 1 };
        
        cos = std::cos(this->_vec_rotate[0]), sin = std::sin(this->_vec_rotate[0]);
        mat_rotate_x = {    1,   0,    0, 0,
                            0, cos, -sin, 0,
                            0, sin,  cos, 0,
                            0,   0,    0, 1 };
        
        cos = std::cos(this->_vec_rotate[1]), sin = std::sin(this->_vec_rotate[1]);
        mat_rotate_y = {  cos,  0, sin, 0,
                            0,  1,   0, 0,
                         -sin,  0, cos, 0,
                            0,  0,   0, 1 };

        mat_scale = {_vec_scale[0], 0, 0, 0,
                     0, _vec_scale[1], 0, 0,
                     0, 0, _vec_scale[2], 0,
                     0, 0, 0, 1 };

        mat_translate = {  1, 0, 0, _vec_translate[0],
                           0, 1, 0, _vec_translate[1],
                           0, 0, 1, _vec_translate[2],
                           0, 0, 0, 1 };

        mat_transformation = matmul(mat_translate,      mat_rotate_x);
        mat_transformation = matmul(mat_transformation, mat_rotate_y);
        mat_transformation = matmul(mat_transformation, mat_rotate_z);
        mat_transformation = matmul(mat_transformation, mat_scale);

        this->_mat_transformation = mat_transformation;
    }

    std::vector<float> getTransformationMatrix(){
        return this->_mat_transformation;
    }

    Object operator+(const Object& obj){
        Object res_obj;

        res_obj.insertBuild(_build);

        for(vertex v : _vertices){
            std::vector<float> vec = {v.x, v.y, v.z, 1.0};
            vec = matvecmul(_mat_transformation, vec);
            res_obj.insertVertex({vec[0], vec[1], vec[2]});
        }

        build last_build = res_obj.getBuild()[res_obj.getBuild().size()-1];

        for(vertex v : obj._vertices){
            std::vector<float> vec = {v.x, v.y, v.z, 1};
            vec = matvecmul(obj._mat_transformation, vec);
            res_obj.insertVertex({vec[0], vec[1], vec[2]});
        }

        for(auto build : obj._build){
            last_build = res_obj.getBuild()[res_obj.getBuild().size()-1];
            int first = last_build.first + last_build.count + build.first;
            res_obj.insertBuild({build.mode, first, build.count, build.color});
        }

        return res_obj;
    }

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
    
    std::vector<float> matvecmul(const std::vector<float> A,const std::vector<float> B){
        std::vector<float> C(4);
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                C[i] += A[4*i + j] * B[j];
            }
        }
        return C;
    }
};

class Scene {
    std::vector<Object> _objects;
    size_t _id_current_obj;
    GLint _program;
public:
    Scene(const GLint &program){
        _program = program;
        _id_current_obj = 0;
    }

    void insertObject(Object &obj){
        _objects.insert(_objects.end(), obj);
    }

    void nextObject(){
        _id_current_obj = (_id_current_obj+1)%_objects.size();
    }

    void prevObject(){
        _id_current_obj = _id_current_obj == 0 ? _objects.size()-1 : _id_current_obj-1;
    }

    Object *currentObject(){
        return &_objects[_id_current_obj];
    }

    // loadBuffer: recarrega o buffer para cada objeto 
    void loadBuffer(Object *obj){
        for(build bld : obj->getBuild()){
            GLuint buffer;
            glGenBuffers(0, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);

            // Abaixo, nós enviamos todo o conteúdo da variável vertices.
            glBufferData(GL_ARRAY_BUFFER, bld.count * sizeof(std::vector<vertex>), &obj->getVertex()[bld.first], GL_DYNAMIC_DRAW);

            // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
            GLint loc = glGetAttribLocation(_program, "position");
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(obj->getVertex()[0]), (void*) 0);

            GLint loc_mat = glGetUniformLocation(_program, "mat_transformation");
            glUniformMatrix4fv(loc_mat, 1, GL_TRUE, &obj->getTransformationMatrix()[0]);

            // Associando variáveis do programa GLSL (Fragment Shaders) com nossos dados
            GLint loc_color = glGetUniformLocation(_program, "color");
            glUniform4f(loc_color, bld.color[0], bld.color[1], bld.color[2], bld.color[3]);

            glDrawArrays(bld.mode, 0, bld.count);

            glDisableVertexAttribArray(loc);
        }
    }

    void draw(Object *obj){
        for(auto build : obj->getBuild()){
            glDrawArrays(build.mode, build.first, build.count);
        }
    }

    void render(){
        for(auto &obj : _objects){
            loadBuffer(&obj);
            //draw(&obj);
        }
    }
};

// classe triangulo: popula os vertices de um triangulo equilátero
class Triangle : public Object {

public:
    Triangle(float l) : Object() {
        float h = l*std::sqrt(3)/2;
        this->insertVertex({ 0.0, 2*h/3, 0.0});
        this->insertVertex({ l/2, -h/3, 0.0 });
        this->insertVertex({-l/2, -h/3, 0.0 });
        this->insertBuild(build({GL_TRIANGLE_STRIP, 0, 3, {0.2, 0.3, 0.0, 1.0}}));
    };
};

// classe retângulo: popula os vertices de um retângulo ou quadrado
class Rectangle : public Object {

public:
    Rectangle(float b, float h) : Object(){
    this->insertVertex({-b/2,  h/2, 0.0});
    this->insertVertex({-b/2, -h/2, 0.0});
    this->insertVertex({ b/2,  h/2, 0.0});
    this->insertVertex({ b/2, -h/2, 0.0});
    this->insertBuild(build({GL_TRIANGLE_STRIP, 0, 4, {0.2, 0.3, 0.0, 1.0}}));
    };
};

// classe circulo: popula os vertices de um circulo
class Circle : public Object {

public:
    Circle(float radius) : Object(){
    int num_vertices = 32;
    float pi = M_PI;
    float angle = 0.0;
    float x, y;
    for(int i=0; i < num_vertices; i++){
	    angle += (2.0*pi)/num_vertices;
	    x = cos(angle)*radius;
	    y = sin(angle)*radius;
	    this->insertVertex({x, y, 0.0});
    }
    this->insertBuild(build({GL_TRIANGLE_FAN, 0, num_vertices, {0.2, 0.3, 0.0, 1.0}}));
    };
};


class Cube : public Object {
public:
    Cube(float L) : Object(){
        Object cube;

        Rectangle face_1(L, L);
        Rectangle face_2(L, L);
        Rectangle face_3(L, L);
        Rectangle face_4(L, L);
        Rectangle face_5(L, L);
        Rectangle face_6(L, L);

        face_1.translate({0.00, 0.00, L/2});
        face_1.setColor({1.0, 0.0, 0.0, 1.0});

        face_2.translate({0.00, 0.00,-L/2});
        face_2.setColor({0.0, 1.0, 0.0, 1.0});

        face_3.translate({0.00, L/2, 0.00});
        face_3.rotate({M_PI/2, 0.00, 0.00});
        face_3.setColor({0.0, 0.0, 1.0, 1.0});

        face_4.translate({0.00,-L/2, 0.00});
        face_4.rotate({M_PI/2, 0.00, 0.00});
        face_4.setColor({1.0, 1.0, 0.0, 1.0});

        face_5.translate({-L/2, 0.00, 0.00});
        face_5.rotate({0.00, M_PI/2, M_PI/2});
        face_5.setColor({1.0, 0.0, 1.0, 1.0});

        face_6.translate({L/2, 0.00, 0.00});
        face_6.rotate({0.00, M_PI/2, M_PI/2});
        face_6.setColor({0.0, 1.0, 1.0, 1.0});

        cube = face_1 + face_2 + face_3 + face_4 + face_5 + face_6;

        this->insertVertex(cube.getVertex());
        this->insertBuild(cube.getBuild());
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
        scene->currentObject()->translate( { 0.00, 0.00,-0.01} );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        scene->currentObject()->translate( { 0.00, 0.00, 0.01} );
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
        scene->currentObject()->rotate( { 0.00,-0.01, 0.00 } );
    }
    if(glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS){
        scene->currentObject()->rotate( { 0.00, 0.01, 0.00 } );
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
"    gl_Position = mat_transformation * vec4(position, 1.0); \n"
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
 
    // Adicionando Cubo
    Cube cubo(0.4);

    // Vetor de poligonos que serão renderizados
    Scene cena(program);
    cena.insertObject(cubo);

    // Exibindo nossa janela
    glfwShowWindow(window);
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