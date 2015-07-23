#ifndef __LED_HPP__
#define __LED_HPP__
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <unistd.h>
//#include "../../LED/ledCommon.h"
#include "Packetizer.hpp"

#define NUMLEDS 16
#define STRIPSIZE 8

	typedef struct Pixel
	{
		uint8_t green, red, blue;
	} PIXEL;

class LED{
	
	private:
	typedef enum {
		manual,
		all,
		strip,
		dual_all,
		dual_strip,
		set
	} ComCodes;

	std::unordered_map<std::string, PIXEL> color_map;
	std::unordered_map<std::string, PIXEL[STRIPSIZE]> strip_map;
	Packetizer *uc_stream;
	

	public:
	LED();
	LED(const char *port);
	~LED();

	int send_all_color(uint8_t red, uint8_t green, uint8_t blue);
	int send_strip();
	int send_all_color(PIXEL &color);
	int send_all_color(std::string &color);
	int send_strip_color(PIXEL &color, uint8_t strip);
	int send_strip_color(std::string &color, uint8_t strip);
	int send_dual_color(uint8_t primary_red, uint8_t primary_green, uint8_t primary_blue, uint8_t secondary_red, uint8_t secondary_green, uint8_t secondary_blue);
	int send_dual_color(PIXEL &primary_color, PIXEL &secondary_color);
	int send_dual_color(std::string &primary_color, std::string &secondary_color);
	int send_dual_strip(PIXEL &primary_color, PIXEL &secondary_color, uint8_t strip);
	int send_dual_strip(std::string primary_color, std::string secondary_color, uint8_t strip);
	int send_refresh();

	int set_pixel(const std::string &name, int pixel, const std::string &color);
	int set_pixel_inv(const std::string &name, int offset, int interval, const std::string &color);
	int set_strip(const std::string &name, const std::string color);

	int add_strip(const std::string &name);
	int remove_strip(const std::string &name);
	int check_strip(const std::string &name);
	int load_strip_map(const std::string &file, bool overwrite);
	int save_strip_map(const std::string &file, bool overwrite);

	int add_color(const std::string &color, uint8_t red, uint8_t green, uint8_t blue, bool replace);
	int check_color(const std::string color);
	int check_color(const std::string color, uint8_t &red, uint8_t &green, uint8_t &blue);
	int remove_color(const std::string color);
	int load_color_map(const std::string &file, bool overwrite);
	int save_color_map(const std::string &file, bool overwrite);

};

#endif
