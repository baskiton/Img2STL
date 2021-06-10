#include "wx/log.h"

#include "STLFile.h"


STLFile::STLFile(const char *t_name) : m_name(t_name) {
    if (m_name.length() > 80)
        m_name.resize(80);
}

void STLFile::add_triangle(const Polygon3<float> &t_polygon) {
    m_polygons_list.push_back(t_polygon);
}

void STLFile::add_stl(const STLFile &t_stl) {
    std::size_t start_sz = polygons();
    std::size_t sz = t_stl.polygons();
    m_polygons_list.resize(start_sz + sz);
    for (std::size_t i = 0; i < sz; ++i)
        m_polygons_list.at(start_sz + i) = t_stl.m_polygons_list.at(i);
}

void STLFile::to_ascii(wxString &result) {
    for (auto &c : m_name)
        if (c == ' ' or c == '\n' or c == '\t')
            c = '_';
    result.append(wxString::Format("solid %s\n", m_name));

    for (auto &pg : m_polygons_list) {
        result.append(wxString::Format(" facet normal %e %e %e\n", pg.nrm.x, pg.nrm.y, pg.nrm.z));
        result.append("  outer loop\n");
        result.append(wxString::Format("   vertex %e %e %e\n", pg.vx_1.x, pg.vx_1.y, pg.vx_1.z));
        result.append(wxString::Format("   vertex %e %e %e\n", pg.vx_2.x, pg.vx_2.y, pg.vx_2.z));
        result.append(wxString::Format("   vertex %e %e %e\n", pg.vx_3.x, pg.vx_3.y, pg.vx_3.z));
        result.append("  endloop\n endfacet\n");
    }

    result.append(wxString::Format("endsolid %s", m_name));
}

void STLFile::to_bin(stl_bin **t_result, std::size_t &t_size) {
    if (m_name.size() != 80)
        m_name.resize(80, '\0');

    uint32_t tri_cnt = m_polygons_list.size();
    t_size = sizeof(stl_bin) + sizeof(Polygon3<float>) * tri_cnt;

    auto *result = new(tri_cnt) stl_bin;

    m_name.copy(reinterpret_cast<char *>(result->hdr), 80);
    result->tri_cnt = tri_cnt;

    for (uint32_t i = 0; i < tri_cnt; ++i)
        result->pgs[i] = m_polygons_list[i];

    *t_result = result;
}

void STLFile::set_header(const wxString &str) {
    m_name = str;
    m_name.resize(80, '\0');
}

void STLFile::save_file(STLFile::file_type f_type, const wxFileName &filename) {
    wxFile file(filename.GetFullPath(), wxFile::write);

    if (!file.IsOpened()) {
        wxLogError(wxString::Format("Cannot open file %s to write!", filename.GetFullPath()));
        return;
    }

    if (f_type == STLFile::ASCII) {
        wxString result;
        to_ascii(result);
        file.Write(result);
    } else if (f_type == STLFile::BIN) {
        stl_bin *result = nullptr;
        std::size_t sz;
        to_bin(&result, sz);
        file.Write(result, sz);
        delete result;
    }

    file.Close();
    wxLogInfo(wxString::Format("File %s created.", filename.GetFullName()));
}
