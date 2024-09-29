#include <stdio.h>
#include <stdlib.h>
#define SDL_MAIN_HANDLED  // Prevents SDL from overriding the main function
#include <SDL2/SDL.h>

#include <time.h>

int window_width = 1200;
int window_height = 800;

// Preload the WAV file globally
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

void init_audio(const char* file) {
    // init audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize audio! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // load wav
    if (SDL_LoadWAV(file, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Open audio device
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        printf("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        return;
    }
}

void play_sound() {
    // Stop the current audio if any
    SDL_ClearQueuedAudio(deviceId);
    
    // Queue the audio data (non-blocking)
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);  // Start playing audio
}

int detect_collision(int pos_x, int pos_y, int rect_width, int rect_height) {
    if ((pos_x + rect_width) >= window_width || pos_x <= 0 || pos_y + rect_height >= window_height || pos_y <= 0) {
        play_sound();  // Play the sound on collision
        return 1;
    } else {
        return 0;
    }
}

void draw_moving_rectangle(SDL_Renderer* renderer, int speed, int start_pos_x, int start_pos_y, int rect_width, int rect_height) {
    int collision;
    int pos_x = start_pos_x;
    int pos_y = start_pos_y;
    int random_director = 1;
    int prev_direction = 0;  
    int frame_count = 0; 

    SDL_Color colors[7] = {
        {255, 0, 0, 255},       // Red
        {255, 165, 0, 255},     // Orange
        {255, 255, 0, 255},     // Yellow
        {0, 255, 0, 255},       // Green
        {0, 0, 255, 255},       // Blue
        {75, 0, 130, 255},      // Indigo
        {238, 130, 238, 255}    // Violet
    };
    int color_index = 0;

    while (1) {
        SDL_RenderClear(renderer);

        // Detect collision
        collision = detect_collision(pos_x, pos_y, rect_width, rect_height);

        if (collision == 1) {
            printf("[CD] collision detected!\n");

            do {
                random_director = (rand() % 4) + 1;
            } while (random_director == prev_direction);

            prev_direction = random_director;
        }
        
        switch (random_director) {
            case 1:  // Move right-up
                pos_x += speed;
                pos_y -= speed;
                break;
            case 2:  // Move left-down
                pos_x -= speed;
                pos_y += speed;
                break;
            case 3:  // Move right-down
                pos_x += speed;
                pos_y += speed;
                break;
            case 4:  // Move left-up
                pos_x -= speed;
                pos_y -= speed;
                break;
        }

        SDL_Rect rect = {pos_x, pos_y, rect_width, rect_height};

        // Change color every 60 frames
        if (frame_count % 60 == 0) {
            color_index++;
            if (color_index == 7) {
                color_index = 0; 
            }
        }

        SDL_SetRenderDrawColor(renderer, colors[color_index].r, colors[color_index].g, colors[color_index].b, colors[color_index].a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);

        frame_count++;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Delay for smooth animation
        SDL_Delay(1000 / 60);
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL

    printf("[PS] init video \n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    printf("[PS] creating window \n");

    SDL_Window* window = SDL_CreateWindow(
        "Particle Simulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height,
        SDL_WINDOW_SHOWN
    );

    printf("[PS] checking if window creation was successful \n");
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("[PS] creating renderer \n");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize audio and preload sound
    init_audio("hit.wav");

    printf("[PS] init main run loop \n");
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Call the draw_moving_rectangle function
        draw_moving_rectangle(renderer, 5, window_width / 2, window_height / 2, 200, 150);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(deviceId);  
    SDL_FreeWAV(wavBuffer);        
    SDL_Quit();

    return 0;
}
