#include "Led.hpp"

LED::LED()
{
	uc_stream = new Packetizer("/dev/ttyUSB1");
}

LED::LED(const char *port)
{
	uc_stream = new Packetizer(port);
}

LED::~LED()
{
	delete uc_stream;
}

int LED::send_all_color(uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t msg[4];
	msg[0] = all;
	msg[1] = green | 0x80;
	msg[2] = red   | 0x80;
	msg[3] = blue  | 0x80;

	uc_stream->send(msg, 4);

	return 0;
}

int LED::send_all_color(PIXEL &color)
{
	uint8_t msg[4];
	msg[0] = all;
	msg[1] = color.green | 0x80;
	msg[2] = color.red   | 0x80;
	msg[3] = color.blue  | 0x80;
	
	uc_stream->send(msg, 4);

	return 0;
}

int LED::send_all_color(std::string &color)
{
	uint8_t msg[4];
	PIXEL pixel;

	if(check_color(color, pixel.red, pixel.green, pixel.blue));
	{
		msg[0] = all;
		msg[1] = pixel.green | 0x80;
		msg[2] = pixel.red   | 0x80;
		msg[3] = pixel.blue  | 0x80;

		return uc_stream->send(msg, 4);
	}

	return 0;
}

int LED::send_strip_color(PIXEL &color, uint8_t strip)
{
	return 0;
}

int LED::send_strip_color(std::string &color, uint8_t strip)
{
	return 0;
}

int LED::send_dual_color(uint8_t primary_red, uint8_t primary_green, uint8_t primary_blue, uint8_t secondary_red, uint8_t secondary_green, uint8_t secondary_blue)
{
	uint8_t msg[7];

	msg[0] = dual_all;
	
	msg[1] = primary_green | 0x80;
	msg[2] = primary_red   | 0x80;
	msg[3] = primary_blue  | 0x80; 

	msg[4] = secondary_green | 0x80;
	msg[5] = secondary_red   | 0x80;
	msg[6] = secondary_blue  | 0x80;

	return uc_stream->send(msg, 7);
}

int LED::send_dual_color(PIXEL &primary_color, PIXEL &secondary_color)
{
	uint8_t msg[7];

	msg[0] = dual_all;

	msg[1] = primary_color.green | 0x80;
	msg[2] = primary_color.red   | 0x80;
	msg[3] = primary_color.blue  | 0x80;

	msg[4] = secondary_color.green | 0x80;
	msg[5] = secondary_color.red   | 0x80;
	msg[6] = secondary_color.blue  | 0x80;

	return uc_stream->send(msg, 7);		
}

int LED::send_dual_color(std::string &primary_color, std::string &secondary_color)
{
	uint8_t msg[7];
	PIXEL primary_pixel, secondary_pixel;

	if(check_color(primary_color, primary_pixel.red, primary_pixel.green, primary_pixel.blue) & check_color(secondary_color, secondary_pixel.red, secondary_pixel.green, secondary_pixel.blue))
	{
		msg[0] = dual_all;

		msg[1] = primary_pixel.green | 0x80;
		msg[2] = primary_pixel.red   | 0x80;
		msg[3] = primary_pixel.blue  | 0x80;

		msg[4] = secondary_pixel.green | 0x80;
		msg[5] = secondary_pixel.red   | 0x80;
		msg[6] = secondary_pixel.blue  | 0x80;

		return uc_stream->send(msg, 7);		
	}
	return 0;
}

int LED::send_dual_strip(PIXEL &primary_color, PIXEL &secondary_color, uint8_t strip)
{
	return 0;
}

int LED::send_dual_strip(std::string primary_color, std::string secondary_color, uint8_t strip)
{
	return 0;
}

int LED::send_refresh()
{
	uint8_t msg[1];

	msg[0] = set;

	return uc_stream->send(msg, 1);
}

int LED::add_color(const  std::string &color, uint8_t red, uint8_t green, uint8_t blue, bool replace)
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
	return check_color(color);
}

int LED::check_color(const std::string color)
{
	return color_map.count(color);
}

int LED::check_color(const std::string color, uint8_t &red, uint8_t &green, uint8_t &blue)
{
	std::unordered_map<std::string,PIXEL>::const_iterator color_node = color_map.find(color);
	if(color_node == color_map.end() )
		return 0;

	red = color_node->second.red;
	green = color_node->second.green;
	blue = color_node->second.blue;

	return 1;

}

int LED::remove_color(const std::string color)
{
	return color_map.erase(color);
}

int LED::load_color_map(const std::string &file, bool overwrite)
{
	std::ifstream infile;
	std::string color;
	PIXEL pixel;
	infile.open(file);
	int red, green, blue;
	int colors_added = 0;
	
	if(infile.is_open())
	{
		while(!infile.eof())
		{
			infile >> color >> std::hex >> red >> std::hex >> green >> std::hex >> blue;
	
			colors_added += add_color(color, (uint8_t) red, (uint8_t) green, (uint8_t) blue, overwrite);
		}

		infile.close();
	}
	return colors_added;
}

int LED::save_color_map(const std::string &file, bool overwrite)
{
	std::ofstream outfile;

	if(!(access(file.c_str(), F_OK) != -1) | overwrite)
	{
		outfile.open(file, std::ofstream::out | (std::ofstream::trunc));
		if(outfile.is_open())
		{
			for(auto it = color_map.begin(); it != color_map.end(); ++it)
			outfile << it->first.c_str() << " " << std::hex << (int) it->second.red << " " << std::hex << (int) it->second.green << " " << std::hex << (int) it->second.blue << std::endl;
		}

		outfile.close();
		return 1;
	}
	return 0;
}
