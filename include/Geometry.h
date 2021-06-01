#ifndef IMG2STL_GEOMERTY_H_
#define IMG2STL_GEOMERTY_H_


template<typename Digit>
struct Vertex {
    Digit x, y, z;
};

template<typename Digit>
struct Normal {
    Digit x, y, z;
};

template<typename Digit>
struct __attribute__((packed)) Polygon3 {
    Normal<Digit> nrm;
    Vertex<Digit> vx_1,
                  vx_2,
                  vx_3;
    std::uint16_t attr; // must be 0
};

#endif  // IMG2STL_GEOMERTY_H_
