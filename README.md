# Description
📀
just a simple little cute DVD player

![video](DVD_SIMULATOR.mp4)

##### Helpful code:

Draws the rectangle & moves it
```
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
```
collision system

```
int detect_collision(int pos_x, int pos_y, int rect_width, int rect_height) {
    if ((pos_x + rect_width) >= window_width || pos_x <= 0 || pos_y + rect_height >= window_height || pos_y <= 0) {
        play_sound();  // Play the sound on collision
        return 1;
    } else {
        return 0;
    }
}
```


