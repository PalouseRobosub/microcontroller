#include "Led.hpp"

#define WAIT 300000000

int main(int argc, char *argv[])
{
	LED *light_display;
	PIXEL one, two;
	
	if(argc >= 2)
		light_display = new LED(argv[1]);
	else
		light_display = new LED();
	
	light_display->load_color_map("color_map.led", 1);
	light_display->check_color("red", one.red, one.green, one.blue);
	std::cout << std::hex << (int) one.red << " " << (int) one.green << " " << (int) one.blue << std::endl;
	light_display->check_color("green", one.red, one.green, one.blue);
	std::cout << std::hex << (int) one.red << " " << (int) one.green << " " << (int) one.blue << std::endl;
	light_display->check_color("blue", one.red, one.green, one.blue);
	std::cout << std::hex << (int) one.red << " " << (int) one.green << " " << (int) one.blue << std::endl;
	light_display->remove_color("blue");
	light_display->save_color_map("led_test", 1);

/*
	while(true)
	{
		std::cin >> one.red >> one.green >> one.blue;
		one.red = one.red | 0x80;
		one.green = one.green | 0x80;
		one.blue = one.blue | 0x80;

		std::cout << one.red << " " << one.green << " " << one.blue << std::endl;

		light_display->send_all_color(one);
		light_display->send_refresh();
	}
*/
	return 0;
}
