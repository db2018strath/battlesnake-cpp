#ifndef GRID_INCLUDED
#define GRID_INCLUDED

namespace Grid {

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

        [[nodiscard]] get_width() const {
            return m_w;
        }

        [[nodiscard]] get_height() const {
            return m_grid.size() / m_w;
        }
    private:
        const unsigned int m_w;
        std::vector<T> m_grid;
    };

}

#endif