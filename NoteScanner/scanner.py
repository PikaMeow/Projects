<<<<<<< HEAD
# -*- coding: utf-8 -*-

from argparse import ArgumentParser
import numpy as np
from PIL import Image
from scipy.cluster.vq import kmeans, vq
import sys

def percent(string):
    return float(string) / 100


def get_argument_parser():
    # 处理命令行参数
    parser = ArgumentParser(
        description='convert scanned, hand-written notes to PDF'
    )

    show_default = '(default %(default)s)'

    parser.add_argument('-f', '--filename',
                        metavar='IMAGE',
                        # metavar - 参数的名字，在显示帮助信息时才用到.
                        help='files to convert')
    parser.add_argument('-p', '--samplepercent',
                        metavar='PERCENT',
                        default='5',
                        type=percent,
                        help='%% of pixels to sample ' + show_default  # 效果： % of pixels to sample(default 5)
                        )
    parser.add_argument('-s', '--sat_threshold',
                        metavar='PERCENT',
                        type=percent,
                        default='20',
                        help='background saturation threshold %% ' + show_default
                        )
    parser.add_argument('-v', '--value_threshold',
                        metavar='PERCENT',
                        type=percent,
                        default='25',
                        help='background value threshold %% ' + show_default)
    parser.add_argument('-n', '--num_colors',
                        type=int,
                        default='8',
                        help='number of output colors ' + show_default)
    parser.add_argument('-q','--quantized_bits',
                        type=int,
                        default='6',
                        help='number of bits per channel for quantize '+show_default)
    parser.add_argument('-o', '--output_basename',
                        default='note',
                        help='output PNG basename ' + show_default)
    parser.add_argument('-S', '--saturate',
                        action='store_false',
                        default=True,
                        help='do not saturate colors')
    parser.add_argument('-w', '--white_bg',
                        action='store_true',
                        default=False,
                        help='make background white')
    return parser


def load_image(filename):
    try:
        img = Image.open(filename)
    except IOError:
        sys.stderr.write('warning: error opening {}\n'.format(filename))
        return None

    if img.mode != 'RGB':
        img = img.convert('RGB')
    return np.array(img)


def sample_pixels(image, options):
    pixels = image.reshape((-1, 3))
    pixel_num = pixels.shape[0]
    sample_num = int(pixel_num * options.samplepercent)
    idx = np.arange(pixel_num)
    np.random.shuffle(idx)

    return pixels[idx[:sample_num]]


def quantize(sample, bits_per_channel):
    shift = 8 - bits_per_channel
    halfbin = (1 << shift) >> 1
    return ((sample.astype(int) >> shift) << shift) + halfbin


def pack_rgb(image):
    packed = (image[:, 0] << 16 |
              image[:, 1] << 8 |
              image[:, 2])
    return packed


def unpack_rgb(packed):
    rgb = ((packed >> 16) & 0xff,
           (packed >> 8) & 0xff,
           (packed) & 0xff)
    return np.array(rgb)


def get_bgcolor(sample, bits_per_channel):
    quantized = quantize(sample, bits_per_channel).astype(int)
    packed = pack_rgb(quantized)
    unique, count = np.unique(packed, return_counts=True)
    packed_mode = unique[count.argmax()]
    return unpack_rgb(packed_mode)


def rgb_to_sv(rgb):
    if not isinstance(rgb, np.ndarray):
        rgb = np.array(rgb)
    axis = len(rgb.shape) - 1
    cmax = rgb.max(axis=axis).astype(np.float32)
    cmin = rgb.min(axis=axis).astype(np.float32)
    delta = cmax - cmin
    saturation = delta.astype(np.float32) / cmax
    saturation = np.where(cmax == 0, 0, saturation)
    value = cmax / 255.0
    return saturation, value


def get_fgmask(bgcolor, sample, options):
    s_bg, v_bg = rgb_to_sv(bgcolor)
    s_sample, v_sample = rgb_to_sv(sample)
    s_diff = np.abs(s_bg - s_sample)
    v_diff = np.abs(v_bg - v_sample)

    return ((v_diff >= options.value_threshold) |
            (s_diff >= options.sat_threshold))


def get_featurecolor(sample, options, kmeans_iter=40):
    bgcolor = get_bgcolor(sample,options.quantized_bits)
    fgmask = get_fgmask(bgcolor, sample, options)
    centers, _ = kmeans(sample[fgmask].astype(np.float32),
                        options.num_colors - 1,
                        iter=kmeans_iter)
    featurecolor = np.vstack((bgcolor, centers)).astype(np.uint8)
    return featurecolor


def apply_featurecolor(image, featurecolor, options):
    bgcolor = featurecolor[0]
    orig_shape = image.shape
    pixels = image.reshape((-1, 3))
    fgmask = get_fgmask(bgcolor, pixels, options)
    pixel_num = pixels.shape[0]
    labels = np.zeros(pixel_num, dtype=np.uint8)
    labels[fgmask], _ = vq(pixels[fgmask], featurecolor)
    return labels.reshape(orig_shape[:-1])


def save(basename, labels, featurecolor, options):
    if options.saturate:
        featurecolor = featurecolor.astype(np.float32)
        cmin = featurecolor.min()
        cmax = featurecolor.max()
        featurecolor = 255 * (featurecolor - cmin) / (cmax - cmin)
        featurecolor = featurecolor.astype(np.uint8)
    if options.white_bg:
        featurecolor = featurecolor.copy()
        featurecolor[0] = np.array([255, 255, 255])
    output_img = Image.fromarray(labels, 'P')
    output_img.putpalette(featurecolor.flatten())
    output_img.save(basename + '.png')


def notescan_main(options):
    file = options.filename
    image = load_image(file)
    sample = sample_pixels(image, options)
    featurecolor = get_featurecolor(sample, options)
    labels = apply_featurecolor(image, featurecolor, options)
    save(options.output_basename, labels, featurecolor, options)


def main():
    # 处理参数并调用主函数
    notescan_main(options=get_argument_parser().parse_args())


if __name__ == '__main__':
    main()
=======
# -*- coding: utf-8 -*-

from argparse import ArgumentParser
import numpy as np
from PIL import Image
from scipy.cluster.vq import kmeans, vq
import sys

def percent(string):
    return float(string) / 100


def get_argument_parser():
    # 处理命令行参数
    parser = ArgumentParser(
        description='convert scanned, hand-written notes to PDF'
    )

    show_default = '(default %(default)s)'

    parser.add_argument('-f', '--filename',
                        metavar='IMAGE',
                        # metavar - 参数的名字，在显示帮助信息时才用到.
                        help='files to convert')
    parser.add_argument('-p', '--samplepercent',
                        metavar='PERCENT',
                        default='5',
                        type=percent,
                        help='%% of pixels to sample ' + show_default  # 效果： % of pixels to sample(default 5)
                        )
    parser.add_argument('-s', '--sat_threshold',
                        metavar='PERCENT',
                        type=percent,
                        default='20',
                        help='background saturation threshold %% ' + show_default
                        )
    parser.add_argument('-v', '--value_threshold',
                        metavar='PERCENT',
                        type=percent,
                        default='25',
                        help='background value threshold %% ' + show_default)
    parser.add_argument('-n', '--num_colors',
                        type=int,
                        default='8',
                        help='number of output colors ' + show_default)
    parser.add_argument('-q','--quantized_bits',
                        type=int,
                        default='6',
                        help='number of bits per channel for quantize '+show_default)
    parser.add_argument('-o', '--output_basename',
                        default='note',
                        help='output PNG basename ' + show_default)
    parser.add_argument('-S', '--saturate',
                        action='store_false',
                        default=True,
                        help='do not saturate colors')
    parser.add_argument('-w', '--white_bg',
                        action='store_true',
                        default=False,
                        help='make background white')
    return parser


def load_image(filename):
    try:
        img = Image.open(filename)
    except IOError:
        sys.stderr.write('warning: error opening {}\n'.format(filename))
        return None

    if img.mode != 'RGB':
        img = img.convert('RGB')
    return np.array(img)


def sample_pixels(image, options):
    pixels = image.reshape((-1, 3))
    pixel_num = pixels.shape[0]
    sample_num = int(pixel_num * options.samplepercent)
    idx = np.arange(pixel_num)
    np.random.shuffle(idx)

    return pixels[idx[:sample_num]]


def quantize(sample, bits_per_channel):
    shift = 8 - bits_per_channel
    halfbin = (1 << shift) >> 1
    return ((sample.astype(int) >> shift) << shift) + halfbin


def pack_rgb(image):
    packed = (image[:, 0] << 16 |
              image[:, 1] << 8 |
              image[:, 2])
    return packed


def unpack_rgb(packed):
    rgb = ((packed >> 16) & 0xff,
           (packed >> 8) & 0xff,
           (packed) & 0xff)
    return np.array(rgb)


def get_bgcolor(sample, bits_per_channel):
    quantized = quantize(sample, bits_per_channel).astype(int)
    packed = pack_rgb(quantized)
    unique, count = np.unique(packed, return_counts=True)
    packed_mode = unique[count.argmax()]
    return unpack_rgb(packed_mode)


def rgb_to_sv(rgb):
    if not isinstance(rgb, np.ndarray):
        rgb = np.array(rgb)
    axis = len(rgb.shape) - 1
    cmax = rgb.max(axis=axis).astype(np.float32)
    cmin = rgb.min(axis=axis).astype(np.float32)
    delta = cmax - cmin
    saturation = delta.astype(np.float32) / cmax
    saturation = np.where(cmax == 0, 0, saturation)
    value = cmax / 255.0
    return saturation, value


def get_fgmask(bgcolor, sample, options):
    s_bg, v_bg = rgb_to_sv(bgcolor)
    s_sample, v_sample = rgb_to_sv(sample)
    s_diff = np.abs(s_bg - s_sample)
    v_diff = np.abs(v_bg - v_sample)

    return ((v_diff >= options.value_threshold) |
            (s_diff >= options.sat_threshold))


def get_featurecolor(sample, options, kmeans_iter=40):
    bgcolor = get_bgcolor(sample,options.quantized_bits)
    fgmask = get_fgmask(bgcolor, sample, options)
    centers, _ = kmeans(sample[fgmask].astype(np.float32),
                        options.num_colors - 1,
                        iter=kmeans_iter)
    featurecolor = np.vstack((bgcolor, centers)).astype(np.uint8)
    return featurecolor


def apply_featurecolor(image, featurecolor, options):
    bgcolor = featurecolor[0]
    orig_shape = image.shape
    pixels = image.reshape((-1, 3))
    fgmask = get_fgmask(bgcolor, pixels, options)
    pixel_num = pixels.shape[0]
    labels = np.zeros(pixel_num, dtype=np.uint8)
    labels[fgmask], _ = vq(pixels[fgmask], featurecolor)
    return labels.reshape(orig_shape[:-1])


def save(basename, labels, featurecolor, options):
    if options.saturate:
        featurecolor = featurecolor.astype(np.float32)
        cmin = featurecolor.min()
        cmax = featurecolor.max()
        featurecolor = 255 * (featurecolor - cmin) / (cmax - cmin)
        featurecolor = featurecolor.astype(np.uint8)
    if options.white_bg:
        featurecolor = featurecolor.copy()
        featurecolor[0] = np.array([255, 255, 255])
    output_img = Image.fromarray(labels, 'P')
    output_img.putpalette(featurecolor.flatten())
    output_img.save(basename + '.png')


def notescan_main(options):
    file = options.filename
    image = load_image(file)
    sample = sample_pixels(image, options)
    featurecolor = get_featurecolor(sample, options)
    labels = apply_featurecolor(image, featurecolor, options)
    save(options.output_basename, labels, featurecolor, options)


def main():
    # 处理参数并调用主函数
    notescan_main(options=get_argument_parser().parse_args())


if __name__ == '__main__':
    main()
