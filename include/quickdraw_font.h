// Mac headers
#include <Fonts.h>
#include <Types.h>

// System headers

// Third-party headers
#include <litehtml.h>

// My headers

class quickdraw_font
{
    public:
        quickdraw_font(const char *name, short size, StyleParameter style = normal);
        quickdraw_font(short id, short size, StyleParameter style = normal);
        ~quickdraw_font();
        void draw(const litehtml::tchar_t *text, const litehtml::position& pos);
        short width(const litehtml::tchar_t *text);
        FontInfo& metrics();

    private:
        short m_id;
        short m_size;
        StyleParameter m_style;
        FontInfo m_metrics;

        void construct();
        void setUpPort();
};
