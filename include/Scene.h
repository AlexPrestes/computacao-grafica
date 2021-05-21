#ifndef Scene_h_
#define Scene_h_

#include <cmath>
#include "../include/Object.h"

// Classe responsável por controlar os objetos em renderização
class Scene {
    std::vector<Object> _objects;
    size_t _id_current_obj;
    GLint _program;
public:
    Scene(const GLint&);
    // Insere um objeto na cena
    void insertObject(Object&);
    // Define o proximo objeto
    void nextObject();
    // Define o objeto anterior
    void prevObject();
    // Aponto para o objeto atual
    Object *currentObject();

    // loadBuffer: recarrega o buffer para cada objeto e desenha
    void loadBuffer(Object *);
    // desenha todos os objetos da cena
    void render();
};

#endif