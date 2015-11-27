//----------------------------------------------------------
// An example using the IntelR JPEG Library:
// -- Decode a JPEG image from a JFIF file to general pixel buffer.
//----------------------------------------------------------
BOOL DecodeJPGFileToGeneralBuffer(LPCTSTR lpszPathName,int* width,int* height,BYTE** buffer)
{
BOOL bres;
IJLERR jerr;
DWORD x = 0; // pixels in scan line
DWORD y = 0; // number of scan lines
DWORD c = 0; // number of channels
DWORD wholeimagesize;
BYTE* pixel_buf = NULL;
// Allocate the IJL JPEG_CORE_PROPERTIES structure.
JPEG_CORE_PROPERTIES jcprops;
bres = TRUE;

__try
{
// Initialize the IntelR JPEG Library.
jerr = ijlInit(&jcprops);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}
// Get information on the JPEG image
// (i.e., width, height, and channels).
jcprops.JPGFile = const_cast<LPTSTR>(lpszPathName);
jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}
// Set up local data.
x = jcprops.JPGWidth;
y = jcprops.JPGHeight;
c = 3; // Decode into a 3 channel pixel buffer.
// Compute size of desired pixel buffer.
wholeimagesize = (x *y *c);
// Allocate memory to hold the decompressed image data.
pixel_buf = new BYTE [wholeimagesize];
if(NULL == pixel_buf)
{
bres = FALSE;
__leave;
}
// Set up the info on the desired DIB properties.
jcprops.DIBWidth = x;
jcprops.DIBHeight = y; // Implies a bottom-up DIB.
jcprops.DIBChannels = c;
jcprops.DIBColor = IJL_BGR;
jcprops.DIBPadBytes = 0;
jcprops.DIBBytes = pixel_buf;
// Set the JPG color space ... this will always be
// somewhat of an educated guess at best because JPEG
// is "color blind" (i.e., nothing in the bit stream
// tells you what color space the data was encoded from).
// However, in this example we assume that we are
// reading JFIF files which means that 3 channel images
// are in the YCbCr color space and 1 channel images are
// in the Y color space.
switch(jcprops.JPGChannels)
{
case 1:
{
jcprops.JPGColor = IJL_G;
break;
}
case 3:
{
jcprops.JPGColor = IJL_YCBCR;
break;
}
default:
{
// This catches everything else, but no
// color twist will be performed by the IJL.
jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
break;
}
}
// Now get the actual JPEG image data into the pixel buffer.
jerr = ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}

} // __try
__finally
{
if(FALSE == bres)
{
if(NULL != pixel_buf)
{
delete [] pixel_buf;
pixel_buf = NULL;
}
}
// Clean up the IntelR JPEG Library.
ijlFree(&jcprops);
*width = x;
*height = y;
//*nchannels = c;
*buffer = pixel_buf;
} // __finally
return bres;
} // DecodeJPGFileToGeneralBuffer()









