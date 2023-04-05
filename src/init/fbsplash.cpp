#include <init/fbsplash.hpp>
#include <sys/printk.hpp>
#include <mm/string.hpp>
#include <init/kinfo.hpp>

static int isdigit(char ch) {
	if(ch == '0' ||
	   ch == '1' ||
	   ch == '2' ||
	   ch == '3' ||
	   ch == '4' ||
	   ch == '5' ||
	   ch == '6' ||
	   ch == '7' ||
	   ch == '8' ||
	   ch == '9') return 1;
	
	return 0;
}

struct ImageInfo {
	size_t Width;
	size_t Height;
	size_t RGBDepth;
} image;

struct PixelInfo {
	uint8_t R;
	uint8_t G;
	uint8_t B;
}__attribute__((packed));

static inline int JumpAlpha(uint8_t *buffer, size_t position) {
	for (;!isdigit(buffer[position]); position++);

	return position;
}


void LoadFBSplash(uintptr_t address, size_t size) {
	KInfo *info = GetInfo();

	if (!info->fbPresent) {
		PRINTK::PrintK("Framebuffer is not present, not printing splash.\r\n");
		return;
	}


	uint8_t *buffer = address;
	if(buffer[0] != 'P' && buffer[1] != '6') {
		PRINTK::PrintK("Invalid file format.\r\n");
		return;
	}	

	PRINTK::PrintK("Valid splashscreen file.\r\n");

	size_t position = JumpAlpha(buffer, 2);

	if (position >= size) return; /* The file that was provided is not valid or corrupt */

	char width[5];
	char height[5];
	char rgbdepth[4];

	for (int i = 0; position < size && i < 4 && isdigit(buffer[position]); position++, i++) {
		width[i] = buffer[position];
	}

	position = JumpAlpha(buffer, position);

	for (int i = 0; position < size && i < 4 && isdigit(buffer[position]); position++, i++) {
		height[i] = buffer[position];
	}
	
	position = JumpAlpha(buffer, position);

	for (int i = 0; position < size && i < 3 && isdigit(buffer[position]); position++, i++) {
		rgbdepth[i] = buffer[position];
	}


	width[4] = '\0';
	height[4] = '\0';
	rgbdepth[3] = '\0';

	image.Width = atoi(width);
	image.Height= atoi(height);
	image.RGBDepth = atoi(rgbdepth);

	PRINTK::PrintK("Image data\r\n"
		       " - Width:     %d\r\n"
		       " - Height:    %d\r\n"
		       " - RGB depth: %d\r\n",
		       image.Width,
		       image.Height,
		       image.RGBDepth);

	position++;

	Framebuffer *fb = info->framebuffer;
	uint32_t *fbData = (uint32_t*)fb->Address;
	
	PRINTK::PrintK("Printing to framebuffer of size %dx%d and shifts %d %d %d.\r\n", fb->Width, fb->Height, fb->RedShift, fb->GreenShift, fb->BlueShift);

	for (int y = 100; y < image.Height && y < fb->Height; y++) {
		for (int x = 0; x < image.Width && x < fb->Width; x++) {
			PixelInfo *pixel = (PixelInfo*)(buffer + position);
			
			uint32_t color = ((pixel->R << fb->RedShift) | (pixel->G << fb->GreenShift) | (pixel->B << fb->BlueShift));

			fbData[x + y * fb->Width] = color;

			position += sizeof(PixelInfo);
		}
	}
	

	return;
}
