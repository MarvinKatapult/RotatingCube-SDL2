#include <SDL2/SDL.h>
#include <mmatrix.hpp>

#define WIDTH 640
#define HEIGHT 640

int main() {
	
	SDL_Window * window = 0L;
	SDL_Renderer * renderer = 0L;
	SDL_Event e;

	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_CreateWindowAndRenderer( WIDTH, HEIGHT, 0, &window, &renderer );

    SDL_SetWindowTitle( window, "Rotating Cube" );

    // Declaration of starting points
	float length = 0.5;
	float points[8][3] = {
		{ -length, -length, -length },
		{ length, -length, -length },
		{ -length, length, -length },
		{ length, length, -length },
		{ -length, -length, length },
		{ length, -length, length },
		{ -length, length, length },
		{ length, length, length }
	};

	float angle = 0;	
	bool running = true;

	while ( running == true ) {

        // Draw Background
		SDL_SetRenderDrawColor( renderer, 25, 25, 25, 255 );
		SDL_RenderClear( renderer );

        // Rotation matrices
		float rotationX[9] = {
			1, 0, 0,
			0, cos( angle ), -sin( angle ),
			0, sin( angle ), cos( angle )	
		};

		float rotationY[9] = {
			cos( angle ), 0, sin( angle),
			0, 1, 0,
			-sin( angle ), 0, cos( angle )	
		};

		float rotationZ[9] = {
			cos( angle ), -sin( angle ), 0,
			sin( angle ), cos( angle ), 0,	
			0, 0, 1,
		};

		MMatrix m_rotation_x( rotationX, 3, 3 );
		MMatrix m_rotation_y( rotationY, 3, 3 );
		MMatrix m_rotation_z( rotationZ, 3, 3 );

		// Input handeling
		while ( SDL_PollEvent( &e ) ) {
			// Quit
			if ( e.type == SDL_QUIT ) running = false;
		}		


		SDL_Point points_array[8];

		// Apply rotation and projection matrix
		for ( int i = 0; i < 8; i++ ) {
			
			MMatrix m_point( points[i], 3, 1 );

			// Rotation
			MMatrix rotatedX = m_rotation_x.mult( &m_point );
			MMatrix rotatedY = m_rotation_y.mult( &rotatedX );
			MMatrix rotatedZ = m_rotation_z.mult( &rotatedY );

            // Projection
            float distance = sin( angle ) + 2;
            float z = 1 / (distance - rotatedZ.getValue( 2 ) );

            // for ortogonal projection
            // float z = 1;

            float projection[6] = {
                z, 0, 0,
                0, z, 0
            };

            MMatrix m_projection( projection, 2, 3 );

		    MMatrix projected = m_projection.mult( &rotatedZ );

            // Scale point up
            projected.mult( 800 );

			float x = projected.getValue( 0, 0 ) * 0.2 + WIDTH / 2 + sin( angle * 0.5 ) * 100;
			float y = projected.getValue( 0, 1 ) * 0.2 + HEIGHT / 2;

			SDL_Point point = { (int)x, (int)y };
			points_array[i] = point;

		}

		// Drawing Lines ( like every healthy person should )
		SDL_SetRenderDrawColor( renderer, 255, 0, 255, 255 );
		for ( int i = 0; i < 4; i++ ) {
			SDL_RenderDrawLine( renderer, points_array[i].x, points_array[i].y, points_array[i + 4].x, points_array[i + 4].y );
		}

        // Could use some refactoring
		SDL_RenderDrawLine( renderer, points_array[0].x, points_array[0].y, points_array[1].x, points_array[1].y );
		SDL_RenderDrawLine( renderer, points_array[0].x, points_array[0].y, points_array[2].x, points_array[2].y );
		SDL_RenderDrawLine( renderer, points_array[2].x, points_array[2].y, points_array[3].x, points_array[3].y );
		SDL_RenderDrawLine( renderer, points_array[1].x, points_array[1].y, points_array[3].x, points_array[3].y );

		SDL_RenderDrawLine( renderer, points_array[4].x, points_array[4].y, points_array[5].x, points_array[5].y );
		SDL_RenderDrawLine( renderer, points_array[4].x, points_array[4].y, points_array[6].x, points_array[6].y );
		SDL_RenderDrawLine( renderer, points_array[6].x, points_array[6].y, points_array[7].x, points_array[7].y );
		SDL_RenderDrawLine( renderer, points_array[5].x, points_array[5].y, points_array[7].x, points_array[7].y );

		angle += 0.01;

		SDL_RenderPresent( renderer );

		SDL_Delay( 5 );

	}	

	// Quit
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
