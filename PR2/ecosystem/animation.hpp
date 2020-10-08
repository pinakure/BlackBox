#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include <vector>
#include "surface.hpp"
#include "sprite.hpp"

#include "direction.hpp"

class Animation {
public:
	bool		bidirectional   = true;
	int			width			= 0;
	int			height			= 0;
	int			last_frame		= 0;
	double		current_frame	= 0;
	Direction	direction		= Direction::UP;
	std::vector<ALLEGRO_BITMAP*> frame;

	Animation();
	Animation(Sprite &sprite, int width, int height,int sx=0, int sy=0, int dx=1, int dy=1, bool vertical=false);
	int		extractFrames(Sprite &sprite, int source_x=0, int source_y=0, int dest_x=1, int dest_y=1, bool read_vertically = false);
	void	draw(Surface &surface, int x, int y, ALLEGRO_COLOR &color);
	void	qdraw(int x, int y);
	bool	run(double delta=1.0);
};

#endif 