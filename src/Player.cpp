#include "Player.h"
#include <GL/glut.h>

// Constantes
extern int width;
extern int height;

/**
 * Construtor do jogador
 */
Player::Player(float x, float y, float size, int lives)
    : GameObject(x, y, size, size), speed(8.0f), lives(lives), shot(nullptr) {
}

Player::~Player() {
    if (shot != nullptr) {
        delete shot;
    }
}

/**
 * Atualiza o estado do jogador
 */
void Player::update() {
    // Atualizar o tiro
    updateShot();
}

/**
 * Desenha o jogador
 */
void Player::draw() {
    if (!active) return;
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    
    // Desenha como um triângulo apontando para cima
    glBegin(GL_TRIANGLES);
        glVertex2f(0, this->height / 2); // Topo
        glVertex2f(-this->width / 2, -this->height / 2); // Base esquerda
        glVertex2f(this->width / 2, -this->height / 2);  // Base direita
    glEnd();
    
    glPopMatrix();
    
    // Desenha o tiro
    drawShot();
}

/**
 * Move o jogador para a esquerda
 */
void Player::moveLeft() {
    if (x > this->width / 2) {
        x -= speed;
    }
}

/**
 * Move o jogador para a direita
 */
void Player::moveRight() {
    if (x < ::width - this->width / 2) {
        x += speed;
    }
}

/**
 * Verifica se o jogador pode atirar
 */
bool Player::canShoot() const {
    return shot == nullptr || !shot->isActive();
}

/**
 * Dispara um projétil
 */
void Player::shoot() {
    if (canShoot()) {
        // Remove o tiro antigo se existir
        if (shot != nullptr) {
            delete shot;
        }
        // Cria novo projétil
        shot = new Projectile(x, y + this->height / 2 + 10, 15.0f, true);
    }
}

/**
 * Atualiza o projétil do jogador
 */
void Player::updateShot() {
    if (shot != nullptr && shot->isActive()) {
        shot->update();
    }
}

/**
 * Desenha o projétil do jogador
 */
void Player::drawShot() {
    if (shot != nullptr && shot->isActive()) {
        shot->draw();
    }
}

/**
 * Jogador perde uma vida
 */
void Player::loseLife() {
    lives--;
    if (lives > 0) {
        reset();
    } else {
        active = false;
    }
}

/**
 * Reseta a posição do jogador após perder vida
 */
void Player::reset() {
    x = ::width / 2;
    y = 50;
    // Remove o tiro ativo
    if (shot != nullptr) {
        delete shot;
        shot = nullptr;
    }
}
