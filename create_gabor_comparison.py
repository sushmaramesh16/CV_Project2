#!/usr/bin/env python3
import cv2
import numpy as np
from pathlib import Path

# Configuration
target = "pic.0535.jpg"
img_dir = Path("src/olympus")

# RESULTS FROM YOUR PROGRAMS
gabor_results = ["pic.0605.jpg", "pic.0778.jpg", "pic.0805.jpg"]
laws_results = ["pic.0285.jpg", "pic.0628.jpg", "pic.0731.jpg"]
sobel_results = ["pic.0171.jpg", "pic.0454.jpg", "pic.0629.jpg"]

# Load images and resize to same height
def load_and_resize(filename, height=200):
    img = cv2.imread(str(img_dir / filename))
    if img is None:
        print(f"Warning: Could not load {filename}")
        return np.zeros((height, height, 3), dtype=np.uint8)
    aspect = img.shape[1] / img.shape[0]
    width = int(height * aspect)
    return cv2.resize(img, (width, height))

height = 200

# Load target (bigger)
target_img = load_and_resize(target, height=300)

# Load all results
gabor_imgs = [load_and_resize(img, height) for img in gabor_results]
laws_imgs = [load_and_resize(img, height) for img in laws_results]
sobel_imgs = [load_and_resize(img, height) for img in sobel_results]

# Create text labels
def create_label(text, width=1200, height=60):
    img = np.ones((height, width, 3), dtype=np.uint8) * 255
    cv2.putText(img, text, (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 
                1.0, (0, 0, 0), 2, cv2.LINE_AA)
    return img

# Concatenate images horizontally with spacing
def hconcat_with_spacing(imgs, spacing=20):
    if not imgs:
        return np.zeros((height, 100, 3), dtype=np.uint8)
    result = imgs[0]
    for img in imgs[1:]:
        spacer = np.ones((img.shape[0], spacing, 3), dtype=np.uint8) * 255
        result = np.concatenate([result, spacer, img], axis=1)
    return result

# Helper functions
def vconcat_imgs(imgs):
    return np.concatenate(imgs, axis=0)

def hconcat_imgs(imgs):
    return np.concatenate(imgs, axis=1)

# Create rows
target_label = create_label("Target: pic.0535.jpg (interior with stone wall)")
gabor_label = create_label("GABOR FILTERS (Extension 2):")
laws_label = create_label("LAWS FILTERS (Extension 1):")
sobel_label = create_label("SOBEL GRADIENTS (Task 4):")

gabor_row = hconcat_with_spacing(gabor_imgs)
laws_row = hconcat_with_spacing(laws_imgs)
sobel_row = hconcat_with_spacing(sobel_imgs)

# Pad all rows to same width
max_width = max(target_label.shape[1], gabor_row.shape[1], 
                laws_row.shape[1], sobel_row.shape[1])

def pad_to_width(img, width):
    if img.shape[1] < width:
        padding = np.ones((img.shape[0], width - img.shape[1], 3), 
                         dtype=np.uint8) * 255
        return hconcat_imgs([img, padding])
    return img

# Center the target image
target_padding = (max_width - target_img.shape[1]) // 2
target_left = np.ones((target_img.shape[0], target_padding, 3), dtype=np.uint8) * 255
target_right = np.ones((target_img.shape[0], max_width - target_img.shape[1] - target_padding, 3), dtype=np.uint8) * 255
target_centered = hconcat_imgs([target_left, target_img, target_right])

# Pad labels and rows
target_label = pad_to_width(target_label, max_width)
gabor_label = pad_to_width(gabor_label, max_width)
gabor_row = pad_to_width(gabor_row, max_width)
laws_label = pad_to_width(laws_label, max_width)
laws_row = pad_to_width(laws_row, max_width)
sobel_label = pad_to_width(sobel_label, max_width)
sobel_row = pad_to_width(sobel_row, max_width)

# Spacing
spacing = np.ones((30, max_width, 3), dtype=np.uint8) * 255
small_spacing = np.ones((10, max_width, 3), dtype=np.uint8) * 255

# Vertical concatenation
final = vconcat_imgs([
    target_label,
    small_spacing,
    target_centered,
    spacing,
    gabor_label,
    small_spacing,
    gabor_row,
    spacing,
    laws_label,
    small_spacing,
    laws_row,
    spacing,
    sobel_label,
    small_spacing,
    sobel_row,
    spacing
])

# Save
cv2.imwrite("gabor_laws_sobel_comparison.png", final)
print("✓ Saved: gabor_laws_sobel_comparison.png")
print(f"  Size: {final.shape[1]}x{final.shape[0]} pixels")