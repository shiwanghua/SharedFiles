import math
from PIL import Image, ImageDraw, ImageFont
import numpy as np


MM_PER_INCH = 25.4
VIEW_DIST_MM = 300
GRID_MM = VIEW_DIST_MM * 2 * math.tan(3e-4 / 2)

ORIGINAL_CONTENT_PIXEL = (0, 0, 0)
ORIGINAL_BACKGROUND_PIXEL = (255, 255, 255)
HIDDEN_DARK_PIXEL = (0, 0, 0)
HIDDEN_BRIGHT_PIXEL = (255, 255, 255)
HIDDEN_BACKGROUND_PIXEL = (127, 127, 127)


class DisplayConfig:
    def __init__(self, config_filename, font_filename):
        config_file = open(config_filename, 'r')
        screen_size_mm = 0
        for line in config_file.readlines():
            line = line.strip()
            original_line = line

            line = ''.join(line.split(' '))
            setting = line.split('=')
            if setting[0] == 'screen_size(inch)':
                screen_size_mm = float(setting[1]) * MM_PER_INCH
            elif setting[0] == 'screen_resolution':
                width_height = setting[1].split('*')
                width_pixels = int(width_height[0])
                height_pixels = int(width_height[1])
                self.screen_resolution = (width_pixels, height_pixels)
                self.side_pixels = width_pixels // 30
            elif setting[0] == "font_size":
                font_size = int(setting[1])
                self.font = ImageFont.truetype(font_filename, font_size)
                self.font_size = font_size
                all_chr_text = ''.join([chr(chi) for chi in range(0, 256)])
                _, self.line_height_pixels = self.font.getsize(all_chr_text)
            else:
                print("Invalid configuration: \"%s\"", original_line)
                exit(-1)

        screen_ratio = self.screen_resolution[0] / self.screen_resolution[1]  # width/height
        screen_height_mm = math.sqrt(screen_size_mm ** 2 / (1 + screen_ratio ** 2))

        pixel_mm = screen_height_mm / self.screen_resolution[1]
        self.grid_pixels = int(GRID_MM / pixel_mm) + 1

    def get_line_pixels(self):
        return self.screen_resolution[0] - 2 * self.side_pixels


def text2img(text: str, display_config):
    rest_text = text
    line_images = []

    line_image_size = (display_config.screen_resolution[0], display_config.line_height_pixels)

    while rest_text:
        curr_text = ""
        while rest_text and display_config.font.getsize(curr_text + rest_text[0])[0] <= display_config.get_line_pixels():
            curr_text += rest_text[0]
            rest_text = rest_text[1:]
        curr_line_image = Image.new('RGB', line_image_size, ORIGINAL_BACKGROUND_PIXEL)
        draw = ImageDraw.Draw(curr_line_image)
        content_corner = (display_config.side_pixels, 0)
        draw.text(content_corner, curr_text, font=display_config.font, fill=ORIGINAL_CONTENT_PIXEL)
        line_images.append(curr_line_image)

    text_image_size = (display_config.screen_resolution[0],
                       display_config.side_pixels * 2 + len(line_images) * display_config.line_height_pixels)
    text_image = Image.new('RGB', text_image_size, ORIGINAL_BACKGROUND_PIXEL)

    for i in range((len(line_images))):
        line_corner = (0, display_config.side_pixels + i * display_config.line_height_pixels)
        text_image.paste(line_images[i], line_corner)

    return text_image


def detect_boundary(text_image: Image):
    boundary_mark = np.zeros(text_image.size, dtype=int)

    n_boundary = 2
    for bdy in range(1, n_boundary + 1):
        for u in range(1, text_image.width - 1):
            for v in range(1, text_image.height - 1):
                pixel = text_image.getpixel((u, v))
                if pixel != ORIGINAL_BACKGROUND_PIXEL and not boundary_mark[u][v]:
                    if (bdy == 1 and ORIGINAL_BACKGROUND_PIXEL in {text_image.getpixel((u - 1, v - 1)),
                                                                   text_image.getpixel((u - 1, v + 1)),
                                                                   text_image.getpixel((u + 1, v - 1)),
                                                                   text_image.getpixel((u + 1, v + 1))})\
                            or (bdy > 1 and (bdy - 1) in {boundary_mark[u - 1][v - 1],
                                                          boundary_mark[u - 1][v + 1],
                                                          boundary_mark[u + 1][v - 1],
                                                          boundary_mark[u + 1][v + 1]}):
                        boundary_mark[u][v] = bdy

    return boundary_mark


def hide_text_image(text_image: Image, grid_size):
    boundary_mark = detect_boundary(text_image)
    assert boundary_mark.shape == text_image.size

    hidden_text_image = Image.new('RGB', text_image.size)

    layer_colors = [HIDDEN_BACKGROUND_PIXEL,
                    HIDDEN_DARK_PIXEL,
                    HIDDEN_BACKGROUND_PIXEL]

    for u in range(text_image.width):
        for v in range(text_image.height):
            if boundary_mark[u][v]:
                hidden_text_image.putpixel((u, v), layer_colors[boundary_mark[u][v]])
            elif text_image.getpixel((u, v)) == ORIGINAL_BACKGROUND_PIXEL:
                hidden_text_image.putpixel((u, v), HIDDEN_BACKGROUND_PIXEL)
            else:
                if (u // grid_size) % 2 + (v // grid_size) % 2 == 0:
                    hidden_text_image.putpixel((u, v), HIDDEN_BRIGHT_PIXEL)
                else:
                    hidden_text_image.putpixel((u, v), HIDDEN_DARK_PIXEL)

    return hidden_text_image


def main():
    display_config = DisplayConfig("config.txt", "AardvarkBold.ttf")

    text_file = open("text.txt", 'r')
    text = text_file.read()

    text_image = text2img(text, display_config)
    text_image.save("original.png")

    hidden_image = hide_text_image(text_image, display_config.grid_pixels)
    hidden_image.save("hidden.png")


if __name__ == '__main__':
    main()
