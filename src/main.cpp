#include <SDL2/SDL.h>
#include <mmatrix.hpp>

#define WIDTH 640
#define HEIGHT 640

#define SCALEFACTOR 5

int main() {
	
	SDL_Window * window = 0L;
	SDL_Renderer * renderer = 0L;
	SDL_Event e;

	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_CreateWindowAndRenderer( WIDTH, HEIGHT, 0, &window, &renderer );

	bool running = true;

	SDL_SetRenderDrawColor( renderer, 55, 55, 55, 255 );
	SDL_RenderClear( renderer );
		
	// Declaration of rotation and projection matrices
	float angle = 0;	

	float projection[6] = {
		1, 0, 0,
		0, 1, 0
	};

	float length = 500;
	float points[8][3] = {
		{ -length, -length, 0 },
		{ length, -length, 0 },
		{ -length, length, 0 },
		{ length, length, 0 },
		{ -length, -length, length * 2 },
		{ length, -length, length * 2 },
		{ -length, length, length * 2 },
		{ length, length, length * 2 }
	};

	MMatrix m_projection( projection, 2, 3 );

	while ( running == true ) {

		SDL_SetRenderDrawColor( renderer, 55, 55, 55, 255 );
		SDL_RenderClear( renderer );

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

		SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

		SDL_Point points_array[8];

		// Apply rotation and projection matrix
		for ( int i = 0; i < 8; i++ ) {
			
			MMatrix m_point( points[i], 3, 1 );

			// Rotation
			MMatrix * rotated;
			rotated = m_rotation_x.mult( &m_point );
			rotated = m_rotation_y.mult( rotated );
			rotated = m_rotation_z.mult( rotated );

			MMatrix * projected = m_projection.mult( rotated );

			float x = projected->getValue( 0, 0 ) * 0.2 + WIDTH / 2;
			float y = projected->getValue( 0, 1 ) * 0.2 + HEIGHT / 2;

			SDL_Point point = { (int)x, (int)y };
			points_array[i] = point;

		}

		// Drawing Lines ( like every healthy person does )
		for ( int i = 0; i < 4; i++ ) {
			SDL_RenderDrawLine( renderer, points_array[i].x, points_array[i].y, points_array[i + 4].x, points_array[i + 4].y );
		}

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
