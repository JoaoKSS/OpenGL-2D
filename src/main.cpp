/**
 * TP03 - Space Invaders
 * Implementação usando OpenGL (legacy) e GLUT
 * 
 * Este jogo segue os princípios de Programação Orientada a Objetos (POO)
 * com classes para cada entidade e um sistema de gerenciamento de estado.
 */

#include <GL/glut.h>
#include <stdio.h>
#include "Game.h"

// Constantes da janela (não const para serem visíveis em outros arquivos)
int width = 800;
int height = 600;

// Ponteiro global para o jogo
Game* game = nullptr;

/**
 * Callback de display - desenha tudo na tela
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (game != nullptr) {
        game->draw();
    }
    
    glutSwapBuffers();
}

/**
 * Callback de update - atualiza a lógica do jogo
 */
void update(int value) {
    if (game != nullptr) {
        game->update();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

/**
 * Callbacks de teclado
 */
void handleKeysDown(unsigned char key, int x, int y) {
    if (game != nullptr) {
        game->handleKeyDown(key);
    }
    
    // ESC para sair
    if (key == 27) {
        exit(0);
    }
}

void handleKeysUp(unsigned char key, int x, int y) {
    if (game != nullptr) {
        game->handleKeyUp(key);
    }
}

void handleSpecialKeysDown(int key, int x, int y) {
    if (game != nullptr) {
        game->handleSpecialKeyDown(key);
    }
}

void handleSpecialKeysUp(int key, int x, int y) {
    if (game != nullptr) {
        game->handleSpecialKeyUp(key);
    }
}

/**
 * Callback de reshape - ajusta a projeção quando a janela é redimensionada
 */
void reshape(int w, int h) {
    // Previne divisão por zero
    if (h == 0) h = 1;
    
    // Usa a janela inteira para renderizar
    glViewport(0, 0, w, h);
    
    // Configura a matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Define o sistema de coordenadas 2D
    gluOrtho2D(0.0, width, 0.0, height);
    
    // Volta para a matriz ModelView
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Inicializa o GLUT e OpenGL
 */
void initGL(int argc, char** argv) {
    glutInit(&argc, argv);
    
    // Usa Double Buffer (para animação) e RGB
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("TP03 - Space Invaders (OOP)");
    
    // Cor de fundo (preto)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

/**
 * Registra os callbacks do GLUT
 */
void registerCallbacks() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    
    // Callbacks de teclado (baseado em estado)
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutSpecialFunc(handleSpecialKeysDown);
    glutSpecialUpFunc(handleSpecialKeysUp);
    
    // Ignora repetição automática de teclas
    glutIgnoreKeyRepeat(1);
}

/**
 * Função principal
 */
int main(int argc, char** argv) {
    printf("===========================================\n");
    printf("  Space Invaders - TP03 Computacao Grafica\n");
    printf("===========================================\n");
    printf("Iniciando jogo...\n\n");
    
    // Inicializa GLUT e OpenGL
    initGL(argc, argv);
    
    // Cria e inicializa o jogo PRIMEIRO
    game = new Game(width, height);
    Game::setInstance(game);
    game->init();
    
    // Registra callbacks
    registerCallbacks();
    
    printf("Jogo iniciado! Boa sorte!\n\n");
    
    // Entra no loop principal do GLUT
    glutMainLoop();
    
    // Limpeza (nunca alcançado por causa do glutMainLoop)
    delete game;
    
    return 0;
}
