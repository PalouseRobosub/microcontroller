#include <string>
#include <unordered_map>
#include <cstdint>
#include "../../LED/ledCommon.h"
#include "packetizer.h"

class LED{
	std::unordered_map<std::string, PIXEL> color_map;
	std::unordered_map<std::string, PIXEL[STRIPSIZE]> strip_map;



	public:
	int send_strip();
	int send_all_color(PIXEL color);
	int send_stip_color(PIXEL color);
	int send_dual_color(PIXEL color, PIXEL color);
	int send_refresh();

	int set_pixel(int strip, int pixel, PIXEL color);
	int set_pixel_inv(int strip, int offset, int interval, int PIXEL color);
	int set_strip(int strip, PIXEL color);
	int add_strip();
	int remove_strip();

	int add_color(std::string color, uint8_t red, uint8_t green, uint8_t blue, bool replace);
	int check_color(std::string color);
	int check_color(std::string color, uint8_t red, uint8_t green, uint8_t blue);
	int remove_color(std::string color);
	int load_color_map(std::string file);
	int save_color_map(std::string file);

}

