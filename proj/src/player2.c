#include "player2.h"
#include <stdlib.h> 

void updatePlayer2AI(Player *player2, Ball *ball, int counter) {
    int player2X = player2->x;
    int player2Y = player2->y;
    int ballX = ball->x;
    int ballY = ball->y;

    int deltaX = ballX - player2X;
    int deltaY = ballY - player2Y;



    // Prioritize movement along the axis with the largest difference
    if(player2->state == MOVE || player2->state == STOP){
        player2->state = MOVE;
        
        if (abs(deltaX) > abs(deltaY)) {
            if (deltaX < 0) {
                player2->movement = LEFT_PLAYER;
                player2->direction = LEFTD;
            } else if (deltaX > 0) {
                player2->movement = RIGHT_PLAYER;
                player2->direction = RIGHTD;
            }
        } else {
            if (deltaY < 0) {
                player2->movement = UP_PLAYER;
            } else if (deltaY > 0) {
                player2->movement = DOWN_PLAYER;
            }
        }
    }
    

    
    // Check for collision between player2 and the ball
    // Get the current hit limits of player2
    int x_min, x_max, y_min, y_max;
    get_current_hit_limits(player2, &x_min, &x_max, &y_min, &y_max);

    // Check if the ball is within the hit limits of player2
    if ((ballX >= x_min-20) && (ballX <= x_max + 20) && (ballY >= y_min-20) && (ballY <= y_max + 20)) {
            // If there is a collision, trigger a hit animation for player2
            
            player2->state = HIT;
    }

    updatePlayerMovementsTimer(player2, counter);
    
    
}
