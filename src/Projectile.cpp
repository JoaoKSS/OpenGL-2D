#include "Projectile.h"
#include <GL/glut.h>

// Constantes
extern int width;
extern int height;

/**
 * Construtor do projétil
 */
Projectile::Projectile(float x, float y, float speed, bool fromPlayer)
    : GameObject(x, y, 8, 20), speed(speed), fromPlayer(fromPlayer) {
}

/**
 * Atualiza a posição do projétil
 */
void Projectile::update() {
    if (!active) return;
    
    // Move o projétil
    y += speed;
    
    // Desativa se sair da tela
    if (y > ::height || y < 0) {
        active = false;
    }
}

/**
 * Desenha o projétil na tela
 */
void Projectile::draw() {
    if (!active) return;
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    // Cor branca para tiro do jogador, amarela para tiro dos aliens
    if (fromPlayer) {
        glColor3f(1.0f, 1.0f, 1.0f);  // Branco
    } else {
        glColor3f(1.0f, 1.0f, 0.0f);  // Amarelo
    }
    
    glBegin(GL_QUADS);
        glVertex2f(-this->width / 2, -this->height / 2);
        glVertex2f( this->width / 2, -this->height / 2);
        glVertex2f( this->width / 2,  this->height / 2);
        glVertex2f(-this->width / 2,  this->height / 2);
    glEnd();
    
    glPopMatrix();
}
