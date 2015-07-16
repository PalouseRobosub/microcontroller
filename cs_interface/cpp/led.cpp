#include "../../LED/led.h"
#include "led.hpp"

int main(int argc, char *argv[])
{



}

LED::send_all_color($`PIXEL color)
{
	uint8 led_buf[4];
	led_buf[0] = all;
	led_buf[1] = color.green;
	send(
}


int LED::add_color(std::string color, uint8_t red, uint8_t green, uint8_t blue, bool replace)
{
	if(check_color(color))
	{
		if(replace)
		{
			color_map[color] = {green, red, blue};
			return check_color(color);
		}
		return 0;
	}

	color_map.insert({color,{green, red, blue}});
	return check_color(color):
}

int check_color(std::string color)
{
	return color_map.count(color);
}
int check_color(std::string color, uint8_t red, uint8_t green, uint8_t blue)
{
	std::unordered_map<std::string,PIXEL>::const_iterator color_node = color_map.find(color);
	if(
}
int remove_color(std::string color);
int load_color_map(std::string file);
int save_color_map(std::string file);
