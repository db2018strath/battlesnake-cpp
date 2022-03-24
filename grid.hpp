#ifndef GRID_INCLUDED
#define GRID_INCLUDED

template <typename T>
class Grid {
public:
    Grid(unsigned int t_w, unsigned int t_h) 
        : m_w(t_w)
        , m_grid(t_w * t_h)
    {
        ;
    }

    T& operator()(unsigned int t_x, unsigned int t_y) {
        return m_grid[t_y * get_width() + t_x];
    }

    [[nodiscard]] const T& operator()(unsigned int t_x, unsigned int t_y) const {
        return m_grid[t_y * get_width() + t_x];
    }

    [[nodiscard]] unsigned int get_width() const {
        return m_w;
    }

    [[nodiscard]] unsigned int get_height() const {
        return m_grid.size() / m_w;
    }

    [[nodiscard]] unsigned int size() const {
        return m_grid.size();
    }

    bool operator==(const Grid<T>& t_other) const {
        return
            (m_w == t_other.m_w) &&  
            (m_grid == t_other.m_grid);  
    }
private:
    unsigned int m_w;
    std::vector<T> m_grid;
};

template <>
class Grid<bool> {
public:
    Grid(unsigned int t_w, unsigned int t_h) 
        : m_w(t_w)
        , m_grid(t_w * t_h)
    {
        ;
    }
    
    std::vector<bool>::reference operator()(unsigned int t_x, unsigned int t_y) {
        return m_grid[t_y * get_width() + t_x];
    }
    
    [[nodiscard]] bool operator()(unsigned int t_x, unsigned int t_y) const {
        return m_grid[t_y * get_width() + t_x];
    }
    
    [[nodiscard]] unsigned int get_width() const {
        return m_w;
    }
    
    [[nodiscard]] unsigned int get_height() const {
        return m_grid.size() / m_w;
    }

    [[nodiscard]] unsigned int size() const {
        return m_grid.size();
    }

    bool operator==(const Grid<bool>& t_other) const {
        return
            (m_w == t_other.m_w) &&
            (m_grid == t_other.m_grid);
    }
private:
    unsigned int m_w;
    std::vector<bool> m_grid;
};

#endif
