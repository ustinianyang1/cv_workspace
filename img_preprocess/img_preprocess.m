ROOT_DIR = fileparts(fileparts(mfilename('fullpath')));
IMAGE_DIR = fullfile(ROOT_DIR, "image");
OUTPUT_DIR = fullfile(ROOT_DIR, "processed_images");

main(IMAGE_DIR, OUTPUT_DIR);

function out = preprocess_image(img_path)
    img = imread(img_path);
    img = rgb2gray(img);
    sampled = img(1:4:end, 1:4:end);
    out = uint8(floor(double(sampled) / 16) * 16);
end

function main(image_dir, output_dir)
    if ~exist(output_dir, 'dir')
        mkdir(output_dir);
    end

    files = dir(fullfile(image_dir, '*.jpg'));
    for k = 1:numel(files)
        img_path = fullfile(image_dir, files(k).name);
        processed_img = preprocess_image(img_path);
        out_path = fullfile(output_dir, files(k).name);
        imwrite(processed_img, out_path);
    end
end