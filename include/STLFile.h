#ifndef IMG2STL_STLFILE_H_
#define IMG2STL_STLFILE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "wx/file.h"
#include "wx/filename.h"
#include "wx/string.h"

#include "Geometry.h"


struct __attribute__((packed)) stl_bin {
    uint8_t hdr[80];
    uint32_t tri_cnt;
    Polygon3<float> pgs[];

    static void *operator new(std::size_t sz, uint32_t cnt) {
        return ::operator new(sz + sizeof(Polygon3<float>) * cnt);
    }

    static void operator delete(void* ptr, std::size_t sz) {
        ::operator delete(ptr);
    }
};

class STLFile {
public:
    enum file_type {
        ASCII = 0,
        BIN
    };

    explicit STLFile(const char *t_name = "");
    ~STLFile() { m_polygons_list.clear(); };

    void add_triangle(const Polygon3<float> &t_polygon);
    void add_stl(const STLFile &t_stl);

    void add(const Polygon3<float> &t_polygon) { add_triangle(t_polygon); }
    void add(const STLFile &t_stl) { add_stl(t_stl); }

    STLFile &operator+=(const Polygon3<float> &t_polygon) {
        add_triangle(t_polygon);
        return *this;
    }

    STLFile &operator+=(const STLFile &t_stl) {
        add_stl(t_stl);
        return *this;
    }

    void clear() { m_polygons_list.clear(); }

    void to_ascii(wxString &result);
    void to_bin(stl_bin **t_result, std::size_t &t_size);

    [[nodiscard]] std::size_t polygons() const { return m_polygons_list.size(); }

    void set_header(const wxString &str);

    void save_file(file_type f_type, const wxFileName &filename);

private:
    std::string m_name; // name (for ASCII; without whitespaces) or header (for BIN; <= 80 symbols)
    std::vector<Polygon3<float>> m_polygons_list;

};


#endif //   IMG2STL_STLFILE_H_
