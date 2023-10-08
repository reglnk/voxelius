#ifndef CLIENT_UI_FONT_HH
#define CLIENT_UI_FONT_HH
#include <client/gl_texture.hh>
#include <shared/vfs.hh>

namespace ui
{
class Font final {
public:
    bool load_image(const vfs::path_t &path, int width, int height);
    bool load_vga_rom(const vfs::path_t &path, int width, int height);
    void unload();

    int get_glyph_width() const;
    int get_glyph_height() const;
    int get_texture_cwidth() const;
    int get_texture_cheight() const;
    const gl::Texture2D &get() const;

private:
    int glyph_width {};
    int glyph_height {};
    int texture_cwidth {};
    int texture_cheight {};
    gl::Texture2D texture {};
};
} // namespace ui

#endif/* CLIENT_UI_FONT_HH */
