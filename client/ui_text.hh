// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef CLIENT_UI_TEXT_HH
#define CLIENT_UI_TEXT_HH
#include <client/gl_texture.hh>
#include <string>

namespace ui
{
class Text final {
public:
    void create(int width, int height);
    void set(int line, const std::string &value);
    void set(int line, const std::wstring &value);
    void destroy();

    int get_max_text_width() const;
    int get_max_text_height() const;
    int get_texture_width() const;
    int get_texture_height() const;
    const gl::Texture2D &get() const;

private:
    int max_text_width {};
    int max_text_height {};
    int texture_width {};
    int texture_height {};
    gl::Texture2D texture {};
};
} // namespace ui

#endif/* CLIENT_UI_TEXT_HH */
