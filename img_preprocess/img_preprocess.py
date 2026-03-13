import os
import cv2
import numpy as np

IMAGE_DIR = "image"
OUTPUT_DIR = "processed_images"

def img_preprocess(img_path):
    img = cv2.imread(img_path, 0) # 以灰度模式读取图像
    if img is None:
        return None
    sampled = img[::4, ::4] # 每隔4个像素采样一次，降低分辨率
    return (np.floor(sampled / 16) * 16).astype(np.uint8) # 将像素值降低到16的倍数


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    for img_file in os.listdir(IMAGE_DIR):
        img_path = os.path.join(IMAGE_DIR, img_file)
        processed_img = img_preprocess(img_path)
        if processed_img is None:
            continue
        processed_img_path = os.path.join(OUTPUT_DIR, img_file)
        cv2.imwrite(processed_img_path, processed_img)


if __name__ == "__main__":
    main()