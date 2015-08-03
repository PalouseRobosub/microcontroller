#include "Led.hpp"
#include "unistd.h"

int main(int argc, char *argv[])
{
	LED *light_display;
	PIXEL one, two;
	
	if(argc >= 2)
		light_display = new LED(argv[1]);
	else
		light_display = new LED();
  
	light_display->load_color_map("color.txt", 1);
	light_display->add_strip("fun");
	light_display->save_strip_map("strip.txt", 1);



	return 0;
}
