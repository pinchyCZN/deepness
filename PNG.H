// png-loader

#include <malloc.h>
#include "png\png.h"

// loads 24bit rgb image (with alphachannel) to 32bit argb buffer
void loadpng32(char *file_name, int& nWidth, int&nHeight, unsigned int*& buffer)  /* We need to open the file */
{
   png_structp png_ptr;
   png_infop info_ptr;
   unsigned int sig_read = 0;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;
   FILE *fp;

   // open file
   if ((fp = fopen(file_name, "rb")) == NULL) return;

   // install error handlers
//   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, loadstop, loadstop, loadstop);
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   // no file?
   if (png_ptr == NULL)
   {
      fclose(fp);
      return;
   }

   // Allocate/initialize the memory for image information.
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      return;
   }

   // Set error handling if you are using the setjmp/longjmp method
   if (setjmp(png_ptr->jmpbuf))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      fclose(fp);
      /* If we get here, we had a problem reading the file */
      return;
   }

   // Set up the input control if you are using standard C streams
   png_init_io(png_ptr, fp);

   // If we have already read some of the signature
   png_set_sig_bytes(png_ptr, sig_read);
   png_read_info(png_ptr, info_ptr);
   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

nWidth = width;
nHeight = height;
buffer = new unsigned int[width*height];

   // tell libpng to strip 16 bit/color files down to 8 bits/color
   png_set_strip_16(png_ptr);

   // Strip alpha bytes from the input data without combining with th background (not recommended).
//   png_set_strip_alpha(png_ptr);

   // Extract multiple pixels with bit depths of 1, 2, and 4 from a single byte into separate bytes (useful for paletted and grayscale images).
   png_set_packing(png_ptr);

   // Change the order of packed pixels to least significant bit first (not useful if you are using png_set_packing)
   png_set_packswap(png_ptr);

   // Expand paletted colors into true RGB triplets
   if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_expand(png_ptr);

   // Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand(png_ptr);

   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_expand(png_ptr);

/*
   png_color_16 my_background, *image_background;
   if (png_get_bKGD(png_ptr, info_ptr, &image_background)) png_set_background(png_ptr, image_background, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
   else png_set_background(png_ptr, &my_background, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
*/

   // set gamma
   float screen_gamma = 2.2f;
   png_set_gamma(png_ptr, screen_gamma, 0.50);

   // invert monocrome files to have 0 as white and 1 as black
//   png_set_invert_mono(png_ptr);


   // flip the RGB pixels to BGR (or RGBA to BGRA)
//   png_set_bgr(png_ptr);

   // swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR)
//  png_set_swap_alpha(png_ptr);

   // swap bytes of 16 bit files to least significant byte first
   png_set_swap(png_ptr);

   // Add filler (or alpha) byte (before/after each RGB triplet) */
   png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

   // Turn on interlace handling
   int number_passes = png_set_interlace_handling(png_ptr);

   // Optional call to gamma correct and add the background to the palette and update info structure
   png_read_update_info(png_ptr, info_ptr);

   png_bytep row_pointers;
   row_pointers= (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));

   // The other way to read images - deal with interlacing
   for (int pass = 0; pass < number_passes; pass++)
   {
      for (unsigned int y = 0; y < height; y++)
      {
         png_read_rows(png_ptr, &row_pointers, NULL, 1);

		     unsigned char *src;
		     src= (unsigned char*)row_pointers;

		     for (unsigned int x=0;x<width;x++)
		     {
			     int r= *src++;
			     int g= *src++;
			     int b= *src++;
			     int a= *src++;
			     buffer[y*width+x]= (a<<24)+(b<<16)+(g<<8)+r;
		     }
      }
   }



   // read rest of file, and get additional chunks in info_ptr - REQUIRED
   png_read_end(png_ptr, info_ptr);

   // clean up after the read, and free any memory allocated - REQUIRED
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

   // close the file
   fclose(fp);

   // that's it
   return;
}

