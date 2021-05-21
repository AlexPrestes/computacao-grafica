#include <cmath>
#include "../include/Object.h"

// Classe Objeto
// classe que abstrai qualquer desenho/objeto
Object::Object(){
    _vec_scale  = { 1.0, 1.0, 1.0 };

    _vec_rotate = { 0.0, 0.0, 0.0 };

    _vec_translate  = { 0.0, 0.0, 0.0 };

    _mat_transformation = { 1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 };
}
    
// insere um novo vertice ao objeto
void Object::insertVertex(Vertex vertice){
    this->_vertices.push_back(vertice);
}
// insere um vetor de vertice ao objeto
void Object::insertVertex(std::vector<Vertex> vertices){
    this->_vertices.insert(this->_vertices.end(), vertices.begin(), vertices.end());
}
// retorna o vetor de vertices do objeto
std::vector<Vertex> Object::getVertex(){
    return this->_vertices;
}
// redefine a cor dos vertices do objeto
void Object::setColor(std::vector<GLfloat> color){
    for(int i = 0; i < this->_build.size(); i++){
        this->_build[i].color.clear();
        this->_build[i].color.insert(this->_build[i].color.end(), color.begin(), color.end());
    }
}

// insere uma primitiva
void Object::insertBuild(Build build){
    this->_build.insert(this->_build.end(), build);
}
// insere um vetor de primitivas
void Object::insertBuild(std::vector<Build> build){
    this->_build.insert(this->_build.end(), build.begin(), build.end());
}
// retorna o vetor de primitivas
std::vector<Build> Object::getBuild(){
    return this->_build;
}
    
// metodo de translação do poligonos
void Object::translate(std::vector<float> vt_translate){
    for(int i = 0; i < 3; i++){
        this->_vec_translate[i] += vt_translate[i];
    }
    this->setTransformationMatrix();
}
    
// metodo de rotação por referencia
void Object::rotate(std::vector<float> vt_rotate){
    for(int i = 0; i < 3; i++){
        this->_vec_rotate[i] += vt_rotate[i];
    }
    this->setTransformationMatrix();
}

// metodo de escala por referencia
void Object::scale(std::vector<float> vt_scale){
    for(int i = 0; i < 3; i++){
        this->_vec_scale[i] += vt_scale[i];
    }
    this->setTransformationMatrix();
}
// recalcula a matriz de transformação
void Object::setTransformationMatrix(){
    std::vector<float> mat_transformation(16), mat_scale(16), mat_rotate_x(16),
    mat_rotate_y(16), mat_rotate_z(16), mat_translate(16);
    float cos, sin;
    
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

    cos = std::cos(this->_vec_rotate[2]), sin = std::sin(this->_vec_rotate[2]);
    mat_rotate_z = {  cos, -sin, 0, 0,
                      sin,  cos, 0, 0,
                        0,    0, 1, 0,
                        0,    0, 0, 1 };

    mat_scale = {   _vec_scale[0], 0, 0, 0,
                    0, _vec_scale[1], 0, 0,
                    0, 0, _vec_scale[2], 0,
                    0, 0, 0, 1 };

    mat_translate = {  1, 0, 0, _vec_translate[0],
                       0, 1, 0, _vec_translate[1],
                       0, 0, 1, _vec_translate[2],
                       0, 0, 0, 1 };

    mat_transformation = matmul(mat_translate,      mat_rotate_z);
    mat_transformation = matmul(mat_transformation, mat_rotate_y);
    mat_transformation = matmul(mat_transformation, mat_rotate_x);
    mat_transformation = matmul(mat_transformation, mat_scale);

    this->_mat_transformation = mat_transformation;
}
// retorna a matriz de transformação
std::vector<float> Object::getTransformationMatrix(){
    return this->_mat_transformation;
}
// sobre carga do operador + para junção de objetos
Object Object::operator+(const Object& obj){
    Object res_obj;

    res_obj.insertBuild(this->_build);

    for(Vertex v : this->_vertices){
        std::vector<float> vec = {v.x, v.y, v.z, 1.0};
        vec = matvecmul(this->_mat_transformation, vec);
        res_obj.insertVertex({vec[0], vec[1], vec[2]});
    }

    Build last_build = res_obj.getBuild()[res_obj.getBuild().size()-1];

    for(Vertex v : obj._vertices){
        std::vector<float> vec = {v.x, v.y, v.z, 1};
        vec = matvecmul(obj._mat_transformation, vec);
        res_obj.insertVertex({vec[0], vec[1], vec[2]});
    }

    for(Build build : obj._build){
        last_build = res_obj.getBuild()[res_obj.getBuild().size()-1];
        int first = last_build.first + last_build.count + build.first;
        res_obj.insertBuild({build.mode, first, build.count, build.color});
    }

    return res_obj;
}
// função que multiplica matriz por matriz
std::vector<float> Object::matmul(const std::vector<float> A,const std::vector<float> B){
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
// função que multiplica matriz por vetor
std::vector<float> Object::matvecmul(const std::vector<float> A,const std::vector<float> B){
    std::vector<float> C(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            C[i] += A[4*i + j] * B[j];
        }
    }
    return C;
}

// classe triangulo: popula os vertices de um triangulo equilátero
Triangle::Triangle(float l) : Object() {
    float h = l*std::sqrt(3)/2;
    this->insertVertex({ 0.0, 2*h/3, 0.0});
    this->insertVertex({ l/2, -h/3, 0.0 });
    this->insertVertex({-l/2, -h/3, 0.0 });
    this->insertBuild(Build({GL_TRIANGLE_STRIP, 0, 3, {0.2, 0.3, 0.0, 1.0}}));
}

// classe retângulo: popula os vertices de um retângulo
Rectangle::Rectangle(float b, float h) : Object(){
    this->insertVertex({-b/2,  h/2, 0.0});
    this->insertVertex({-b/2, -h/2, 0.0});
    this->insertVertex({ b/2,  h/2, 0.0});
    this->insertVertex({ b/2, -h/2, 0.0});
    this->insertBuild(Build({GL_TRIANGLE_STRIP, 0, 4, {0.2, 0.3, 0.0, 1.0}}));
}

// classe circulo: popula os vertices de um circulo
Circle::Circle(float radius) : Object(){
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
    this->insertBuild(Build({GL_TRIANGLE_FAN, 0, num_vertices, {0.2, 0.3, 0.0, 1.0}}));
}

// classe cubo: popula os vertices de um cubo
Cube::Cube(float L) : Object(){
    Object cube;

    // Faces do cubo
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
    face_5.rotate({0.00, M_PI/2, 0.00});
    face_5.setColor({1.0, 0.0, 1.0, 1.0});

    face_6.translate({L/2, 0.00, 0.00});
    face_6.rotate({0.00, M_PI/2, 0.00});
    face_6.setColor({0.0, 1.0, 1.0, 1.0});
    // junta as faces pra formar um cubo
    cube = face_1 + face_2 + face_3 + face_4 + face_5 + face_6;
    // insere 
    this->insertVertex(cube.getVertex());
    this->insertBuild(cube.getBuild());
}

// classe cubo: popula os vertices de um cubo
Pyramid::Pyramid(float L) : Object(){
    Object pyramid;

    Rectangle face_1(L, L);
    Triangle face_2(L);
    Triangle face_3(L);
    Triangle face_4(L);
    Triangle face_5(L);

    float theta = atan(1/sqrt(2));
    float tv = sqrt(2)*L/6;
    float th = L/3;

    face_1.rotate({ M_PI/2, 0.00, 0.00});
    face_1.setColor({0.0, 0.0, 1.0, 1.0});

    face_2.rotate({ theta, 0.00, 0.00});
    face_2.translate({0.00, tv, -th});
    face_2.setColor({0.0, 1.0, 0.0, 1.0});

    face_3.rotate({-theta, 0.00, 0.00});
    face_3.translate({0.00, tv, th});
    face_3.setColor({1.0, 0.0, 0.0, 1.0});

    face_4.rotate({-theta, M_PI/2, 0.00});
    face_4.translate({th, tv, 0.00});
    face_4.setColor({1.0, 0.0, 1.0, 1.0});

    face_5.rotate({-theta, -M_PI/2, 0.00});
    face_5.translate({-th, tv, 0.00});
    face_5.setColor({1.0, 1.0, 0.0, 1.0});

    pyramid = face_1 + face_2 + face_3 + face_4 + face_5;

    this->insertVertex(pyramid.getVertex());
    this->insertBuild(pyramid.getBuild());
}