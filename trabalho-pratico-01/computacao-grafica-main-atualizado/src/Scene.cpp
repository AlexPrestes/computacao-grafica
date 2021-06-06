#include <cmath>
#include "../include/Scene.h"

// Classe responsável por controlar os objetos em renderização
Scene::Scene(const GLint &program){
    this->_program = program;
    this->_id_current_obj = 0;
}

void Scene::insertObject(Object &obj){
    this->_objects.insert(this->_objects.end(), obj);
}

void Scene::nextObject(){
    this->_id_current_obj = (this->_id_current_obj+1)%this->_objects.size();
}

void Scene::prevObject(){
    this->_id_current_obj = this->_id_current_obj == 0 ? this->_objects.size()-1 : this->_id_current_obj-1;
}

Object *Scene::currentObject(){
    return &this->_objects[this->_id_current_obj];
}

// loadBuffer: recarrega o buffer para cada objeto 
void Scene::loadBuffer(Object *obj){
    for(Build bld : obj->getBuild()){
        GLuint buffer;
        glGenBuffers(0, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, bld.count * sizeof(std::vector<Vertex>), &obj->getVertex()[bld.first], GL_DYNAMIC_DRAW);

        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        GLint loc = glGetAttribLocation(this->_program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(obj->getVertex()[0]), (void*) 0);

        GLint loc_mat = glGetUniformLocation(this->_program, "mat_transformation");
        glUniformMatrix4fv(loc_mat, 1, GL_TRUE, &obj->getTransformationMatrix()[0]);

        // Associando variáveis do programa GLSL (Fragment Shaders) com nossos dados
        GLint loc_color = glGetUniformLocation(this->_program, "color");
        glUniform4f(loc_color, bld.color[0], bld.color[1], bld.color[2], bld.color[3]);

        glDrawArrays(bld.mode, 0, bld.count);

        glDisableVertexAttribArray(loc);
    }
}

void Scene::render(){
    for(auto &obj : this->_objects){
        loadBuffer(&obj);
    }
}