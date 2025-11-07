#include "Alien.h"
#include <GL/glut.h>

/**
 * Construtor do alienígena
 */
Alien::Alien(float x, float y, float size, int points)
    : GameObject(x, y, size, size), points(points) {
}

/**
 * Atualiza o alienígena
 */
void Alien::update() {
}

/**
 * Desenha o alienígena
 */
void Alien::draw() {
    if (!active) return;
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    
    // Desenha um alien
    // Corpo
    glBegin(GL_QUADS);
        glVertex2f(-this->width / 2, -this->height / 2);
        glVertex2f( this->width / 2, -this->height / 2);
        glVertex2f( this->width / 2,  this->height / 2);
        glVertex2f(-this->width / 2,  this->height / 2);
    glEnd();
    
    // Olhos
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Olho esquerdo
    glBegin(GL_QUADS);
        glVertex2f(-this->width / 3, -this->height / 6);
        glVertex2f(-this->width / 6, -this->height / 6);
        glVertex2f(-this->width / 6,  this->height / 6);
        glVertex2f(-this->width / 3,  this->height / 6);
    glEnd();
    
    // Olho direito
    glBegin(GL_QUADS);
        glVertex2f( this->width / 6, -this->height / 6);
        glVertex2f( this->width / 3, -this->height / 6);
        glVertex2f( this->width / 3,  this->height / 6);
        glVertex2f( this->width / 6,  this->height / 6);
    glEnd();
    
    glPopMatrix();
}

/**
 * Move o alienígena por um delta especificado
 */
void Alien::move(float dx, float dy) {
    x += dx;
    y += dy;
}
