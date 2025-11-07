#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Globais para os sons
Mix_Music *gMusicaFundo = NULL;
Mix_Chunk *gSomDisparo = NULL;
Mix_Chunk *gSomExplosao = NULL;

bool initAudio() {
    // Inicializa o SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("Falha ao inicializar SDL: %s\n", SDL_GetError());
        return false;
    }
    
    // Inicializa o SDL_mixer
    // (Frequencia, formato, canais, tamanho do chunk)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        printf("Falha ao inicializar SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    
    // Carrega os arquivos de som
    // Mix_LoadMUS para musicas longas (ex: .mp3, .ogg)
    gMusicaFundo = Mix_LoadMUS("assets/musica.mp3");
    
    // Mix_LoadWAV para sons curtos (ex: .wav)
    gSomDisparo = Mix_LoadWAV("assets/shot.wav");
    gSomExplosao = Mix_LoadWAV("assets/explosion.wav");

    if(gMusicaFundo == NULL || gSomDisparo == NULL || gSomExplosao == NULL) {
        printf("Falha ao carregar midia: %s\n", Mix_GetError());
        // Nao e critico, o jogo pode rodar sem som
    }

    // Toca a musica de fundo em loop (-1 = loop infinito)
    if(gMusicaFundo) {
        Mix_PlayMusic(gMusicaFundo, -1);
    }
    
    return true;
}

// Funcao para tocar um som curto
void playSound(Mix_Chunk *som) {
    if (som != NULL) {
        // Toca o som em um canal disponivel
        // (canal = -1 para o primeiro livre, 0 = nao repetir)
        Mix_PlayChannel(-1, som, 0);
    }
}

void cleanupAudio() {
    // Libera a memoria dos sons
    Mix_FreeMusic(gMusicaFundo);
    Mix_FreeChunk(gSomDisparo);
    Mix_FreeChunk(gSomExplosao);
    gMusicaFundo = NULL;
    gSomDisparo = NULL;
    gSomExplosao = NULL;
    
    // Fecha o mixer e o SDL
    Mix_CloseAudio();
    SDL_Quit();
}

// No seu main:
// main() {
//   ...
//   Init(argc, argv);
//   initAudio(); // Chame apos o Init do GLUT
//   ...
//   // Quando o jogador atirar:
//   // playSound(gSomDisparo);
//   // Quando um alien explodir:
//   // playSound(gSomExplosao);
//   ...
//   glutMainLoop();
//   cleanupAudio(); // Chame apos o glutMainLoop
//   return 0;
// }
