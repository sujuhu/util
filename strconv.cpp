#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if (defined _MSC_VER) || (defined __MINGW32__)
#pragma warning(disable:4996)
#else
#include <locale.h>
#include <iconv.h>
#endif

extern "C" int _mbstowcs(wchar_t* wcs, char*mbs, size_t max_wc_count)
{
#if (defined _MSC_VER) || (defined __MINGW32__)
  return (int)mbstowcs(wcs, mbs, max_wc_count);
#else
  iconv_t cd = iconv_open("UCS-2", "ASCII");
  if(cd == (iconv_t)(-1)) {
    return -1;
  } else {  
    char* in = mbs;
    size_t in_len = strlen(mbs);
    wchar_t* out = wcs;
    size_t out_len = (size_t)(max_wc_count * sizeof(wchar_t));

    //out_len = iconv(cd, &in, &in_len, NULL, NULL);
    //printf("before in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out);
    int conv_cch = iconv(cd , &in, &in_len, (char**)&out, &out_len);
    if ( -1 == conv_cch) {
    //    printf("after in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out );
    //    printf("iconv failed %d", errno);

    }
    //printf("after in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out );

    //printf("mbstowcs total_size = %d %S rest = %d\n", NewSize, out, rest);
    iconv_close(cd);
    return conv_cch;
  }
#endif
}

extern "C" int _wcstombs(char *mbs, const wchar_t* wcs, size_t max_mbs_count)
{
#if (defined _MSC_VER) || (defined __MINGW32__)
  return (int)wcstombs(mbs, wcs, max_mbs_count);
#else
  iconv_t cd = iconv_open("ASCII", "UCS-2");
  if(cd == (iconv_t)(-1)) {
    return -1;
  } else {  
    char* in = (char*)wcs;
    size_t in_len = wcslen(wcs) * sizeof(wchar_t);
    char* out = mbs;
    size_t out_len = max_mbs_count;

    //out_len = iconv(cd, &in, &in_len, NULL, NULL);
    //printf("before in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out);
    int conv_cch = iconv(cd , &in, &in_len, (char**)&out, &out_len);
    if ( -1 == conv_cch) {
    //    printf("after in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out );
    //    printf("iconv failed %d", errno);

    }
    //printf("after in len = %d, in = %s, out_len = %d, out = %S\n", in_len , in, out_len, out );

    //printf("mbstowcs total_size = %d %S rest = %d\n", NewSize, out, rest);
    iconv_close(cd);
    return conv_cch;
  }
#endif
  return -1;
}