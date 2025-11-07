#include "GameObject.h"

/**
 * Construtor base para todos os objetos do jogo
 */
GameObject::GameObject(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), active(true) {
}

GameObject::~GameObject() {
}

/**
 * Define a posição do objeto
 */
void GameObject::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

/**
 * Define se o objeto está ativo
 */
void GameObject::setActive(bool active) {
    this->active = active;
}

/**
 * Detecção de colisão AABB
 * Retorna true se este objeto está colidindo com outro
 * 
 * Dois retângulos A e B NÃO colidem se:
 * - A está totalmente à esquerda de B (A.x_max < B.x_min)
 * - A está totalmente à direita de B (A.x_min > B.x_max)
 * - A está totalmente acima de B (A.y_min > B.y_max)
 * - A está totalmente abaixo de B (A.y_max < B.y_min)
 * 
 * Se nenhuma dessas condições for verdadeira, eles estão colidindo.
 */
bool GameObject::checkCollision(const GameObject* other) const {
    if (!active || !other->active) {
        return false;
    }
    
    // Calcula os limites da bounding box deste objeto
    float this_x_min = x - width / 2;
    float this_x_max = x + width / 2;
    float this_y_min = y - height / 2;
    float this_y_max = y + height / 2;
    
    // Calcula os limites da bounding box do outro objeto
    float other_x_min = other->x - other->width / 2;
    float other_x_max = other->x + other->width / 2;
    float other_y_min = other->y - other->height / 2;
    float other_y_max = other->y + other->height / 2;
    
    // Verifica se NÃO estão colidindo
    if (this_x_max < other_x_min || this_x_min > other_x_max ||
        this_y_max < other_y_min || this_y_min > other_y_max) {
        return false;
    }
    
    // Se nenhuma das condições acima for verdadeira, estão colidindo
    return true;
}
