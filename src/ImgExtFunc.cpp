#include "wx/colour.h"
#include "wx/image.h"

#include "ImgExtFunc.h"


bool get_bound_box(const wxImage *t_img, wxSize &t_size, wxPoint &t_point, int t_mode, const wxColour &t_color) {
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
    int bbox[4] = {t_img->GetWidth(), -1, 0, 0};
    int x, y, has_data;
    rgb_s mask_v = {t_color.Red(), t_color.Green(), t_color.Blue()};
    auto *data = (rgb_s *)((void *)t_img->GetData());
    unsigned char *alpha = t_img->GetAlpha();

    for (y = 0; y < t_img->GetHeight(); ++y) {
        has_data = 0;
        int yt = t_img->GetWidth() * y;
        for (x = 0; x < t_img->GetWidth(); ++x, ++yt) {
            if ((!t_mode && alpha && alpha[yt]) ||
                (t_mode && (data[yt] != mask_v))) {
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

    t_point.x = -bbox[0];
    t_point.y = -bbox[1];
    t_size.Set(bbox[2] - bbox[0], bbox[3] - bbox[1]);

    return true;
}

void img_to_heightmap(wxImage &t_img, int t_mode, const wxColour &t_color) {
    if (t_mode)
        t_img.SetMaskColour(t_color.Red(), t_color.Green(), t_color.Blue());

    t_img = wxImage(t_img.ConvertToGreyscale());
}
