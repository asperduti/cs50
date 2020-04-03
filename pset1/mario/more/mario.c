#include <cs50.h>
#include <stdio.h>

#define LEFT_RIGHT 1
#define RIGHT_LEFT 0

// Draws the fill part of the step
void draw_step_fill(int step_number, int height, char fill)
{
    for (int length = height - step_number; length < height; length++)
    {
        printf("%c", fill);
    }
}

// Draws the unfill part of the step
void draw_step_unfill(int step_number, int height, char unfill)
{
    for (int length = 1; length <= height - step_number; length++)
    {
        printf("%c", unfill);
    }
}

// Draws a step given the number of the step, the height, the chars used and the orientation
void draw_step(int step_number, int height, char unfill, char fill, int orientation)
{ 
    if (orientation == LEFT_RIGHT)
    {
        draw_step_unfill(step_number, height, unfill);
    }
    
    draw_step_fill(step_number, height, fill);
}

int main(void)
{
    int height = 0;
    while ((height < 1) || (height > 8))
    {
        height = get_int("Height: ");
    }
    
    for (int step = 1; step <= height; step++)
    {
        draw_step(step, height, ' ', '#', LEFT_RIGHT);
        printf("  ");
        draw_step(step, height, ' ', '#', RIGHT_LEFT);
        // End of step
        printf("\n");
    }
}
