#ifndef Object_h_
#define Object_h_

#include <iostream>
#include <vector>

#include <GL/glew.h>

// estrutura do vertice
struct Vertex {
    float x, y, z;
};
// estrutura responsável por determinar as primitivas
struct Build {
    GLenum mode;
    GLint first;
    GLsizei count;
    std::vector<float> color;
};

// Classe Objeto
// classe que abstrai qualquer desenho/objeto
class Object {
    std::vector<Vertex> _vertices;
    std::vector<float> _vec_scale;
    std::vector<float> _vec_rotate;
    std::vector<float> _vec_translate;
    std::vector<float> _mat_transformation;
    std::vector<Build> _build;
public:
    Object();
    
    // insere um novo vertice ao objeto
    void insertVertex(Vertex );
    // insere um vetor de vertice ao objeto
    void insertVertex(std::vector<Vertex> );
    // retorna o vetor de vertices do objeto
    std::vector<Vertex> getVertex();
    // redefine a cor dos vertices do objeto
    void setColor(std::vector<GLfloat> );

    // insere uma primitiva
    void insertBuild(Build );
    // insere um vetor de primitivas
    void insertBuild(std::vector<Build> );
    // retorna o vetor de primitivas
    std::vector<Build> getBuild();
    
    // metodo de translação do poligonos
    void translate(std::vector<float> );
    
    // metodo de rotação por referencia
    void rotate(std::vector<float> );
    
    // metodo de escala por referencia
    void scale(std::vector<float> );
    // recalcula a matriz de transformação
    void setTransformationMatrix();
    // retorna a matriz de transformação
    std::vector<float> getTransformationMatrix();
    // sobre carga do operador + para junção de objetos
    Object operator+(const Object& );
    // função que multiplica matriz por matriz
    std::vector<float> matmul(const std::vector<float> ,const std::vector<float> );
    // função que multiplica matriz por vetor
    std::vector<float> matvecmul(const std::vector<float> ,const std::vector<float> );
};

// classe triangle: popula os vertices de um triangulo equilátero
class Triangle : public Object {
public:
    Triangle(float );
};

// classe rectangle: popula os vertices de um retângulo
class Rectangle : public Object {
public:
    Rectangle(float , float );
};

// classe circle: popula os vertices de um circulo
class Circle : public Object {
public:
    Circle(float );
};

// classe cube: popula os vertices de um cubo
class Cube : public Object {
public:
    Cube(float );
};

// classe pyramid: popula os vertices de um piramide
class Pyramid : public Object {
public:
    Pyramid(float );
};

#endif