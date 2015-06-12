#include <vector>
#include <string>
#include "../../LED/led.h"
#include "packetizer.h"

class LED{
	std::vector<PIXEL[STRIPSIZE]> strips;
	std::vector<PIXEL> colors;
	std::vector<std::string> color_names;



	public:
	int send_strip();
	int send_all_color(PIXEL color);
	int send_stip_color(PIXEL color);
	int send_dual_color(PIXEL color, PIXEL color);
	int send_refresh();

	int set_pixel(int strip, int pixel, PIXEL color);
	int set_pixel_inv(int strip, int offset, ,int interval, int PIXEL color);
	int set_strip(int strip, PIXEL color);
	int add_strip();
	int remove_strip();



}

