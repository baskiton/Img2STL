//
// Created by baskiton on 08.05.2021.
//

#include "wx/colour.h"
#include "wx/image.h"

#include "ImgExtFunc.h"


bool get_bound_box(const wxImage *img, wxSize &size, wxPoint &point, int m_mode, const wxColour &m_color) {
    struct rgb_s {
        unsigned char r,
                g,
                b;
        bool operator!=(const rgb_s &v) const {
            return r != v.r ||
                   g != v.g ||
                   b != v.b;
        }
    };
    int bbox[4] = {img->GetWidth(), -1, 0, 0};
    int x, y, has_data;
    rgb_s mask_v = {m_color.Red(), m_color.Green(), m_color.Blue()};
    auto *data = (rgb_s *)((void *)img->GetData());
    unsigned char *alpha = img->GetAlpha();

    for (y = 0; y < img->GetHeight(); ++y) {
        has_data = 0;
        int yt = img->GetWidth() * y;
        for (x = 0; x < img->GetWidth(); ++x, ++yt) {
            if ((!m_mode && alpha && alpha[yt]) ||
                (m_mode && (data[yt] != mask_v))) {
                has_data = 1;
                if (x < bbox[0])
                    bbox[0] = x;
                if (x >= bbox[2])
                    bbox[2] = x + 1;
            }
        }
        if (has_data) {
            if (bbox[1] < 0)
                bbox[1] = y;
            bbox[3] = y + 1;
        }
    }

    if (bbox[1] < 0)
        return false;

    point.x = -bbox[0];
    point.y = -bbox[1];
    size.Set(bbox[2] - bbox[0], bbox[3] - bbox[1]);

    return true;
}

void img_to_heightmap(wxImage &img, int m_mode, const wxColour &m_color) {
//    wxImage tmp(fn.GetFullPath());
//    wxImage *ret;

    /*
    if (a_crop) {
        wxSize sz;
        wxPoint pt;
        if (get_bound_box(&tmp, sz, pt))
            tmp.Resize(sz, pt);
    }   // */

    if (m_mode)
        img.SetMaskColour(m_color.Red(), m_color.Green(), m_color.Blue());

    img = wxImage(img.ConvertToGreyscale());
}