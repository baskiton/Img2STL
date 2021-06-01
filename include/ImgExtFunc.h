//
// Created by baskiton on 08.05.2021.
//

#ifndef IMG2STL_IMGEXTFUNC_H
#define IMG2STL_IMGEXTFUNC_H


bool get_bound_box(const wxImage *img, wxSize &size, wxPoint &point, int m_mode, const wxColour &m_color);
void img_to_heightmap(wxImage &img, int m_mode, const wxColour &m_color);

#endif //IMG2STL_IMGEXTFUNC_H
