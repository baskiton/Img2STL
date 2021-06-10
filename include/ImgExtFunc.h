#ifndef IMG2STL_IMGEXTFUNC_H_
#define IMG2STL_IMGEXTFUNC_H_

#include "wx/image.h"
#include "wx/colour.h"


bool get_bound_box(const wxImage *t_img, wxSize &t_size, wxPoint &t_point, int t_mode, const wxColour &t_color);
void img_to_heightmap(wxImage &t_img, int t_mode, const wxColour &t_color);

#endif // IMG2STL_IMGEXTFUNC_H_
